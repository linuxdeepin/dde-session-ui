/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "bubblemanager.h"
#include "bubble.h"
#include "dbus_daemon_interface.h"
#include "dbuslogin1manager.h"
#include "notificationentity.h"
#include "persistence.h"
#include "constants.h"
#include "notifysettings.h"
#include "notification-center/notifycenterwidget.h"
#include "dbusdockinterface.h"
#include "signalbridge.h"

#include <DDesktopServices>

#include <QStringList>
#include <QVariantMap>
#include <QTimer>
#include <QDebug>
#include <QScreen>
#include <QDBusContext>
#include <QDateTime>
#include <QGSettings>
#include <QDBusAbstractInterface>

#include <algorithm>

#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_display_monitor.h>

using DisplayInter = com::deepin::daemon::Display;
using MonitorInter = com::deepin::daemon::display::Monitor;

BubbleManager::BubbleManager(AbstractPersistence *persistence, AbstractNotifySetting *setting, QObject *parent)
    : QObject(parent)
    , m_persistence(persistence)
    , m_login1ManagerInterface(new Login1ManagerInterface(Login1DBusService, Login1DBusPath,
                                                          QDBusConnection::systemBus(), this))
    , m_displayInter(new DisplayInter(DisplayDaemonDBusServie, DisplayDaemonDBusPath,
                                      QDBusConnection::sessionBus(), this))
    , m_dockDeamonInter(new DockInter(DockDaemonDBusServie, DockDaemonDBusPath,
                                      QDBusConnection::sessionBus(), this))
    , m_userInter(new UserInter(SessionDBusServie, SessionDaemonDBusPath,
                                QDBusConnection::sessionBus(), this))
    , m_soundeffectInter(new SoundeffectInter(SoundEffectDaemonDBusServie, SoundEffectDaemonDBusPath,
                                              QDBusConnection::sessionBus(), this))
    , m_notifySettings(setting)
    , m_notifyCenter(new NotifyCenterWidget(m_persistence))
    , m_appearance(new Appearance("com.deepin.daemon.Appearance", "/com/deepin/daemon/Appearance", QDBusConnection::sessionBus(), this))
    , m_gestureInter(new GestureInter("com.deepin.daemon.Gesture"
                                      , "/com/deepin/daemon/Gesture"
                                      , QDBusConnection::systemBus()
                                      , this))
    , m_dockInter(new DBusDockInterface(this))
    , m_trickTimer(new QTimer(this))
{
    m_trickTimer->setInterval(300);
    m_trickTimer->setSingleShot(true);

    qDebug() << Q_FUNC_INFO
             << "m_login1ManagerInterface is valid: " << m_login1ManagerInterface->isValid()
             << "m_displayInter is valid : " << m_displayInter->isValid()
             << "m_dockDeamonInter is valid : " << m_dockDeamonInter->isValid()
             << "m_userInter is valid : " << m_userInter->isValid()
             << "m_soundeffectInter is valid : " << m_soundeffectInter->isValid()
             << "m_appearance is valid : " << m_appearance->isValid()
             << "m_gestureInter is valid : " << m_gestureInter->isValid()
             << "m_dockInter is valid : " << m_dockInter->isValid();


    initConnections();
    geometryChanged();

    m_notifyCenter->setMaskAlpha(m_appearance->opacity() * 255);
    m_notifyCenter->hide();
    registerAsService();

    // 任务栏在左侧时，触屏划入距离需要超过100
    m_slideWidth = (m_dockDeamonInter->position() == OSD::DockPosition::Right) ? 100 : 0;

    connect(m_userInter, &__SessionManager::LockedChanged, this, [ this ] {
        // 当锁屏状态发生变化时立即隐藏所有通知并插入到通知中心（根据通知的实际情况决定），桌面和锁屏的通知不交叉显示
        popAllBubblesImmediately();
    });
}

BubbleManager::~BubbleManager()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_bubbleList.isEmpty()) qDeleteAll(m_bubbleList);

    m_oldEntities.clear();
    delete m_notifyCenter;
    m_notifyCenter = nullptr;
}

void BubbleManager::CloseNotification(uint id)
{
#if defined (QT_DEBUG) && !defined (GTEST)
    QDBusReply<uint> reply = connection().interface()->servicePid(message().service());
    qDebug() << "PID:" << reply.value();//关闭通知的进程
#endif

    QString str_id = QString::number(id);
    foreach (auto bubble, m_bubbleList) {
        if (bubble->entity()->replacesId() == str_id) {
            //m_persistence->addOne(bubble->entity());
            bubble->close();
            m_bubbleList.removeOne(bubble);
            qDebug() << "CloseNotification : id" << str_id;
        }
    }

    foreach (auto notify, m_oldEntities) {
        if (notify->replacesId() == str_id) {
            //m_persistence->addOne(notify);
            m_oldEntities.removeOne(notify);
            qDebug() << "CloseNotification : id" << str_id;
        }
    }
}

QStringList BubbleManager::GetCapabilities()
{
    QStringList result;
    result << "action-icons" << "actions" << "body" << "body-hyperlinks" << "body-markup";

    return result;
}

QString BubbleManager::GetServerInformation(QString &name, QString &vender, QString &version)
{
    name = QString("DeepinNotifications");
    vender = QString("Deepin");
    version = QString("2.0");

    return QString("1.2");
}

uint BubbleManager::Notify(const QString &appName, uint replacesId,
                           const QString &appIcon, const QString &summary,
                           const QString &body, const QStringList &actions,
                           const QVariantMap hints, int expireTimeout)
{
#ifndef GTEST
    QGSettings oem_setting("com.deepin.dde.notifications", "/com/deepin/dde/notifications/");
    if (oem_setting.keys().contains("notifycationClosed") && oem_setting.get("notifycationClosed").toBool())
        return 0;

    QGSettings setting("com.deepin.dde.osd", "/com/deepin/dde/osd/");
    if (setting.keys().contains("bubbleDebugPrivacy") && setting.get("bubble-debug-privacy").toBool()) {
        qDebug() << "Notify:" << "appName:" + appName << "replaceID:" + QString::number(replacesId)
                 << "appIcon:" + appIcon << "summary:" + summary << "body:" + body
                 << "actions:" << actions << "hints:" << hints << "expireTimeout:" << expireTimeout;

        // 记录通知发送方
        QString cmd = QString("grep \"Name:\" /proc/%1/status").arg(QString::number(connection().interface()->servicePid(message().service())));
        QProcess process;
        QStringList args;
        args << "-c";
        args << cmd;
        process.start("sh", args);
        process.waitForReadyRead();
        QString result = QString::fromUtf8(process.readAllStandardOutput());
        qDebug() << "notify called by :" << result;
        process.close();
    }
#endif

    // 应用通知功能未开启不做处理
    bool enableNotificaion = m_notifySettings->getAppSetting(appName, NotifySettings::ENABELNOTIFICATION).toBool();

    if (!enableNotificaion && !IgnoreList.contains(appName)) {
        return 0;
    }

    QString strBody = body;
    strBody.replace(QLatin1String("\\\\"), QLatin1String("\\"), Qt::CaseInsensitive);

    EntityPtr notification = std::make_shared<NotificationEntity>(appName, QString(), appIcon,
                                                                  summary, strBody, actions, hints,
                                                                  QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                                  QString::number(replacesId),
                                                                  QString::number(expireTimeout));

    bool enablePreview = true;
    bool showInNotifyCenter = true;
    bool playsound = true;
    bool lockscreeshow = true;
    bool dndmode = isDoNotDisturb();
    bool systemNotification = IgnoreList.contains(notification->appName());
    bool lockscree = m_userInter->locked();

    if (!systemNotification) {
        enablePreview = m_notifySettings->getAppSetting(appName, NotifySettings::ENABELPREVIEW).toBool();
        showInNotifyCenter = m_notifySettings->getAppSetting(appName, NotifySettings::SHOWINNOTIFICATIONCENTER).toBool();
        playsound = m_notifySettings->getAppSetting(appName, NotifySettings::ENABELSOUND).toBool();
        lockscreeshow = m_notifySettings->getAppSetting(appName, NotifySettings::LOCKSCREENSHOWNOTIFICATION).toBool();
    }

    notification->setShowPreview(enablePreview);
    notification->setShowInNotifyCenter(showInNotifyCenter);

    if (playsound && !dndmode) {
        QString action;
        //接收蓝牙文件时，只在发送完成后才有提示音,"cancel"表示正在发送文件
        if (actions.contains("cancel")) {
            if (hints.contains("x-deepin-action-_view")) {
                action = hints["x-deepin-action-_view"].toString();
                if (action.contains("xdg-open"))
                    DDesktopServices::playSystemSoundEffect(DDesktopServices::SSE_Notifications);
            }
        } else {
            DDesktopServices::playSystemSoundEffect(DDesktopServices::SSE_Notifications);
        }
    }

    if (systemNotification && dndmode) {
        DDesktopServices::playSystemSoundEffect(DDesktopServices::SSE_Notifications);
    }

    if (!calcReplaceId(notification)) {
        if (systemNotification) { // 系统通知
            pushBubble(notification);
        } else if (lockscree && !lockscreeshow) { // 锁屏不显示通知
            if (showInNotifyCenter) { // 开启在通知中心显示才加入通知中心
                m_persistence->addOne(notification);
            }
        } else { // 锁屏显示通知或者未锁屏状态
            if (!systemNotification && !dndmode && enableNotificaion) { // 普通应用非勿扰模式并且开启通知选项
                pushBubble(notification);
            } else if (showInNotifyCenter) {
                m_persistence->addOne(notification);
            }
        }
    }

    // If replaces_id is 0, the return value is a UINT32 that represent the notification.
    // If replaces_id is not 0, the returned value is the same value as replaces_id.
    return replacesId == 0 ? notification->id() : replacesId;
}

void BubbleManager::pushBubble(EntityPtr notify)
{
    if (notify == nullptr) return;

    Bubble *bubble = createBubble(notify);
    if (!bubble)
        return;
 
    if (m_bubbleList.size() == BubbleEntities + BubbleOverLap) {
        m_oldEntities.push_front(m_bubbleList.last()->entity());
        m_bubbleList.last()->setVisible(false);
        m_bubbleList.last()->deleteLater();
        m_bubbleList.removeLast();
    }

    m_bubbleList.push_front(bubble);
    pushAnimation(bubble);
}

void BubbleManager::popBubble(Bubble *bubble)
{
    qDebug() << Q_FUNC_INFO << ", body: " << bubble->entity()->body();
    // bubble delete itself when aniamtion finished
    refreshBubble();
    popAnimation(bubble);
    m_bubbleList.removeOne(bubble);
}

void BubbleManager::popAllBubblesImmediately()
{
    qDebug() << Q_FUNC_INFO;
    for (QPointer<Bubble> &bubble : m_bubbleList) {
        if (bubble) {
            if (bubble->entity()->isShowInNotifyCenter())
                m_persistence->addOne(bubble->entity());

            bubble->hide();
            bubble->close();
        }
    }

    m_bubbleList.clear();
}

void BubbleManager::refreshBubble()
{
    if (m_bubbleList.size() < BubbleEntities + BubbleOverLap + 1 && !m_oldEntities.isEmpty()) {
        auto notify = m_oldEntities.takeFirst();
        Bubble *bubble = createBubble(notify, BubbleEntities + BubbleOverLap - 1);
        if (bubble)
            m_bubbleList.push_back(bubble);
    }
}

void BubbleManager::pushAnimation(Bubble *bubble)
{
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)
        return;

    while (index < m_bubbleList.size() - 1) {
        index ++;
        QRect startRect = getLastStableRect(index - 1);
        QRect endRect = getBubbleGeometry(index);
        QPointer<Bubble> item = m_bubbleList.at(index);
        if (item->geometry() != endRect) { //动画中
            startRect = item->geometry();
        }
        if (bubble != nullptr) {
            item->setBubbleIndex(index);
            qDebug() << "pushAnimation, body: " << bubble->entity()->body();
            item->startMove(startRect, endRect);
        }
    }
}

void BubbleManager::popAnimation(Bubble *bubble)
{
    qDebug() << Q_FUNC_INFO << "start ------- body: " << bubble->entity()->body();
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)
        return;

    QRect startRect = getBubbleGeometry(index);
    QRect endRect = getBubbleGeometry(0);

    bubble->startMove(startRect, endRect, true);// delete itself

    while (index < m_bubbleList.size() - 1) {
        index ++;
        QRect startRect = getBubbleGeometry(index);
        QRect endRect = getBubbleGeometry(index - 1);
        QPointer<Bubble> item = m_bubbleList.at(index);
        if (index == BubbleEntities + BubbleOverLap) {
            item->show();
        }
        if (item->geometry() != endRect) { //动画中
            startRect = item->geometry();
        }
        if (bubble != nullptr) {
            qDebug() << "popAnimation, body: " << bubble->entity()->body();
            item->setBubbleIndex(index);
            item->startMove(startRect, endRect);
        }
    }

    //确定层次关系
    for (int i = m_bubbleList.size() - 1; i >= 0 ; --i) {
        Bubble *b = m_bubbleList[i];
        if (b) {
            b->raise();
        }
    }
    qDebug() << Q_FUNC_INFO << "end ------ body: " << bubble->entity()->body();
}

QRect BubbleManager::getBubbleGeometry(int index)
{
    Q_ASSERT(index >= 0 && index <= BubbleEntities + BubbleOverLap);

    QRect rect;
    if (index >= 0 && index <= BubbleEntities - 1) {
        int y = 0;
        if (m_dockPos == OSD::Top) {
            if (m_dockMode == OSD::DockModel::Fashion) {
                y = m_currentDisplayRect.y() + m_currentDockRect.height() + OSD::DockMargin * 2;
            } else {
                y = m_currentDisplayRect.y() + m_currentDockRect.height();
            }
        }
        rect.setX(m_currentDisplayRect.x() + (m_currentDisplayRect.width() - OSD::BubbleWidth(OSD::BUBBLEWINDOW)) / 2);
        rect.setY(y + ScreenPadding + index * BubbleMargin + getBubbleHeightBefore(index));
        rect.setWidth(OSD::BubbleWidth(OSD::BUBBLEWINDOW));
        rect.setHeight(OSD::BubbleHeight(OSD::BUBBLEWINDOW));
    } else if (index >= BubbleEntities && index <= BubbleEntities + BubbleOverLap) {
        rect = getBubbleGeometry(index - 1);

        int x = rect.x() + rect.width() / 20;
        int y = rect.y() + rect.height() / 3;
        int width = rect.width() * 18 / 20;
        int height = rect.height() * 19 / 20;

        rect.setX(x);
        rect.setY(y);
        rect.setWidth(width);
        rect.setHeight(height);
    }

    return rect;
}

int BubbleManager::getBubbleHeightBefore(const int index)
{
    int totalHeight = 0;
    for (int i = 0; i < index; i++) {
        if (m_bubbleList[i]) {
            totalHeight += m_bubbleList[i]->height();
        }
    }

    return totalHeight;
}

QRect BubbleManager::getLastStableRect(int index)
{
    QRect rect = getBubbleGeometry(0);
    for (int i = index - 1; i > 0; --i) {
        if (i >= m_bubbleList.size() || m_bubbleList.at(i)->geometry() != getBubbleGeometry(i)) {
            continue;
        }
        rect = getBubbleGeometry(i);
    }

    return rect;
}

bool BubbleManager::isDoNotDisturb()
{
    if (!m_notifySettings->getSystemSetting(NotifySettings::DNDMODE).toBool())
        return false;

    // 未点击按钮  任何时候都勿扰模式
    if (!m_notifySettings->getSystemSetting(NotifySettings::OPENBYTIMEINTERVAL).toBool() && !m_notifySettings->getSystemSetting(NotifySettings::LOCKSCREENOPENDNDMODE).toBool()) {
        return true;
    }

    bool lockScreen = m_userInter->locked();
    // 点击锁屏时 并且 锁屏状态 任何时候都勿扰模式
    if (m_notifySettings->getSystemSetting(NotifySettings::LOCKSCREENOPENDNDMODE).toBool() && lockScreen)
        return true;

    QTime currentTime = QTime::fromString(QDateTime::currentDateTime().toString("hh:mm"));
    QTime startTime = QTime::fromString(m_notifySettings->getSystemSetting(NotifySettings::STARTTIME).toString());
    QTime endTime = QTime::fromString(m_notifySettings->getSystemSetting(NotifySettings::ENDTIME).toString());

    bool dndMode = false;
    if (startTime < endTime) {
        dndMode = startTime <= currentTime && endTime >= currentTime ? true : false;
    } else if (startTime > endTime) {
        dndMode = startTime <= currentTime || endTime >= currentTime ? true : false;
    } else {
        dndMode = true;
    }

    if (dndMode && m_notifySettings->getSystemSetting(NotifySettings::OPENBYTIMEINTERVAL).toBool()) {
        return dndMode;
    } else {
        return false;
    }
}

QRect BubbleManager::calcDisplayRect()
{
    qreal ratio = qApp->primaryScreen()->devicePixelRatio();
    QRect displayRect = m_displayInter->primaryRect();
    QList<QDBusObjectPath> screenList = m_displayInter->monitors();

    for (const auto &screen : screenList) {
        MonitorInter monitor("com.deepin.daemon.Display", screen.path(), QDBusConnection::sessionBus());
        QRect monitorRect(monitor.x(), monitor.y(), monitor.width(), monitor.height());
        QRect dockRect(m_dockInter->geometry());
        if (monitor.enabled() && monitorRect.contains(dockRect.center())) {
            displayRect = QRect(monitorRect.x(), monitorRect.y(),
                                monitorRect.width() / ratio, monitorRect.height() / ratio);
            break;
        }
    }
    return displayRect;
}

QString BubbleManager::GetAllRecords()
{
    return m_persistence->getAll();
}

QString BubbleManager::GetRecordById(const QString &id)
{
    return m_persistence->getById(id);
}

QString BubbleManager::GetRecordsFromId(int rowCount, const QString &offsetId)
{
    return m_persistence->getFrom(rowCount, offsetId);
}

void BubbleManager::RemoveRecord(const QString &id)
{
    m_persistence->removeOne(id);

    QFile file(CachePath + id + ".png");
    file.remove();
}

void BubbleManager::ClearRecords()
{
    m_persistence->removeAll();

    QDir dir(CachePath);
    dir.removeRecursively();
}

void BubbleManager::Toggle()
{
    if (m_trickTimer->isActive()) {
        return;
    }

    m_trickTimer->start();

    geometryChanged();
    m_notifyCenter->showWidget();
}

void BubbleManager::Show()
{
    if (m_trickTimer->isActive()) {
        return;
    }

    m_trickTimer->start();
    geometryChanged();

    m_notifyCenter->onlyShowWidget();
}

void BubbleManager::Hide()
{
    if (m_trickTimer->isActive()) {
        return;
    }

    m_trickTimer->start();
    geometryChanged();

    m_notifyCenter->onlyHideWidget();
}

uint BubbleManager::recordCount()
{
    return uint(m_persistence->getRecordCount());
}

QStringList BubbleManager::GetAppList()
{
    return m_notifySettings->getAppLists();
}

QDBusVariant BubbleManager::GetAppInfo(const QString id, const uint item)
{
    return QDBusVariant(m_notifySettings->getAppSetting(id, static_cast<NotifySettings::AppConfigurationItem>(item)));
}

QDBusVariant BubbleManager::GetSystemInfo(uint item)
{
    return QDBusVariant(m_notifySettings->getSystemSetting(static_cast<NotifySettings::SystemConfigurationItem>(item)));
}

void BubbleManager::SetAppInfo(const QString id, const uint item, const QDBusVariant var)
{
    m_notifySettings->setAppSetting(id, static_cast<NotifySettings::AppConfigurationItem>(item), var.variant());
}

void BubbleManager::SetSystemInfo(uint item, const QDBusVariant var)
{
    m_notifySettings->setSystemSetting(static_cast<NotifySettings::SystemConfigurationItem>(item), var.variant());
    Q_EMIT systemSettingChanged(m_notifySettings->getSystemSetings_v1());
}

void BubbleManager::appInfoChanged(QString action, LauncherItemInfo info)
{
    if (action == DeletedAction) {
        m_notifySettings->appRemoved(info.ID);
        Q_EMIT appRemoved(info.ID);
    } else if (action == CreatedAction) {
        m_notifySettings->appAdded(info);
        Q_EMIT appAdded(m_notifySettings->getAppSettings_v1(info.ID));
    }
}

void BubbleManager::onOpacityChanged(double value)
{
    m_notifyCenter->setMaskAlpha(value * 255);
}

QString BubbleManager::getAllSetting()
{
    return m_notifySettings->getAllSetings_v1();
}

void BubbleManager::setAllSetting(const QString settings)
{
    m_notifySettings->setAllSetting_v1(settings);
}

QString BubbleManager::getAppSetting(QString appName)
{
    return m_notifySettings->getAppSettings_v1(appName);
}

void BubbleManager::setAppSetting(const QString settings)
{
    QJsonObject currentObj = QJsonDocument::fromJson(settings.toUtf8()).object();
    m_notifySettings->setAppSetting_v1(settings);
    Q_EMIT systemSettingChanged(currentObj.begin().key());
}

QString BubbleManager::getSystemSetting()
{
    return m_notifySettings->getSystemSetings_v1();
}

void BubbleManager::setSystemSetting(QString settings)
{
    m_notifySettings->setSystemSetting_v1(settings);
    Q_EMIT systemSettingChanged(m_notifySettings->getSystemSetings_v1());
}

void BubbleManager::registerAsService()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.interface()->registerService(NotificationsDBusService,
                                            QDBusConnectionInterface::ReplaceExistingService,
                                            QDBusConnectionInterface::AllowReplacement);
    connection.registerObject(NotificationsDBusPath, this);

    QDBusConnection ddenotifyConnect = QDBusConnection::sessionBus();
    ddenotifyConnect.interface()->registerService(DDENotifyDBusServer,
                                                  QDBusConnectionInterface::ReplaceExistingService,
                                                  QDBusConnectionInterface::AllowReplacement);
    ddenotifyConnect.registerObject(DDENotifyDBusPath, this);
}


void BubbleManager::bubbleExpired(Bubble *bubble)
{
    qDebug() << Q_FUNC_INFO << ", body: " << bubble->entity()->body();
    popBubble(bubble);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Expired);
}

void BubbleManager::bubbleDismissed(Bubble *bubble)
{
    qDebug() << Q_FUNC_INFO << ", body: " << bubble->entity()->body();
    popBubble(bubble);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Dismissed);
}

void BubbleManager::bubbleReplacedByOther(Bubble *bubble)
{
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Unknown);
}

void BubbleManager::bubbleActionInvoked(Bubble *bubble, QString actionId)
{
    qDebug() << Q_FUNC_INFO << ", body: " << bubble->entity()->body();
    popBubble(bubble);
    uint id = bubble->entity()->id();
    uint replacesId = bubble->entity()->replacesId().toUInt();
    Q_EMIT ActionInvoked(replacesId == 0 ? id : replacesId, actionId);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Closed);
}

void BubbleManager::updateGeometry()
{
    foreach (auto item, m_bubbleList) {
        item->setGeometry(getBubbleGeometry(item->bubbleIndex()));
        item->updateGeometry();
    }
}

void BubbleManager::initConnections()
{
    connect(m_login1ManagerInterface, SIGNAL(PrepareForSleep(bool)),
            this, SLOT(onPrepareForSleep(bool)));

    connect(m_displayInter, &DisplayInter::PrimaryRectChanged, this, &BubbleManager::geometryChanged, Qt::QueuedConnection);
    connect(m_dockInter, &DBusDockInterface::geometryChanged, this, &BubbleManager::geometryChanged, Qt::UniqueConnection);
    connect(m_dockDeamonInter, &DockInter::serviceValidChanged, this, &BubbleManager::geometryChanged, Qt::UniqueConnection);

    connect(qApp, &QApplication::primaryScreenChanged, this, [ = ] {
        updateGeometry();
    });

    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, [ = ] {
        updateGeometry();
    });

    connect(m_notifySettings, &NotifySettings::appSettingChanged, this, [ = ] (const QString &id, const uint &item, QVariant var) {
        Q_EMIT AppInfoChanged(id, item, QDBusVariant(var));
    });
    connect(m_notifySettings, &NotifySettings::systemSettingChanged, this, [ = ] (const uint &item, QVariant var) {
        Q_EMIT SystemInfoChanged(item, QDBusVariant(var));
    });
    connect(m_notifySettings, &NotifySettings::appAddedSignal, this, [ = ] (const QString &id) {
        Q_EMIT AppAddedSignal(id);
        Q_EMIT appAdded(m_notifySettings->getAppSettings_v1(id));
    });
    connect(m_notifySettings, &NotifySettings::appRemovedSignal, this, [ = ] (const QString &id) {
        Q_EMIT AppRemovedSignal(id);
        Q_EMIT appRemoved(id);
    });

    // 响应任务栏方向改变信号，更新额外触屏划入距离
    connect(m_dockDeamonInter, &DockInter::PositionChanged, this, [ this ] {
        m_slideWidth = (m_dockDeamonInter->position() == OSD::DockPosition::Right) ? 100 : 0;
    });

    connect(m_appearance, &Appearance::OpacityChanged, this,  &BubbleManager::onOpacityChanged);

    connect(SignalBridge::instance(), &SignalBridge::actionInvoked, this , &BubbleManager::ActionInvoked);
}

void BubbleManager::onPrepareForSleep(bool sleep)
{
    // workaround to avoid the "About to suspend..." notifications still
    // hanging there on restoring from sleep confusing users.
    if (!sleep) {
        qDebug() << "Quit on restoring from sleep.";
        qApp->quit();
    }
}

void BubbleManager::geometryChanged()
{
    m_currentDisplayRect = calcDisplayRect();
    // dock未启动时，不要调用其接口，会导致系统刚启动是任务栏被提前启动（比窗管还早），造成显示异常，后续应该改成通知中心显示时才调用任务栏的接口，否则不应调用
    if (m_dockInter->isValid()) {
        m_currentDockRect = m_dockInter->geometry();
    }

    m_dockPos = static_cast<OSD::DockPosition>(m_dockDeamonInter->position());
    m_dockMode = m_dockDeamonInter->displayMode();
    m_notifyCenter->updateGeometry(m_currentDisplayRect, m_currentDockRect, m_dockPos, m_dockMode);
    updateGeometry();
}

bool BubbleManager::calcReplaceId(EntityPtr notify)
{
    bool find = false;

    if (notify->replacesId() == NoReplaceId) {
        notify->setId(QString::number(++m_replaceCount));
        notify->setReplacesId(QString::number(m_replaceCount));
    } else {
        for (int i = 0; i < m_bubbleList.size(); ++i) {
            Bubble *bubble = m_bubbleList.at(i);
            if (bubble->entity()->replacesId() == notify->replacesId()
                    && bubble->entity()->appName() == notify->appName()) {
                m_persistence->addOne(m_bubbleList.at(0)->entity());
                if (i != 0) {
                    bubble->setEntity(m_bubbleList.at(0)->entity());
                }
                m_bubbleList.at(0)->setEntity(notify);
                find = true;
            }
        }

        for (int i = 0; i < m_oldEntities.size(); ++i) {
            if (m_oldEntities.at(i)->replacesId() == notify->replacesId()
                    && m_oldEntities.at(i)->appName() == notify->appName()) {
                m_oldEntities.removeAt(i);
            }
        }
    }

    return find;
}

Bubble *BubbleManager::createBubble(EntityPtr notify, int index)
{
    qDebug() << Q_FUNC_INFO << notify->body() << ", index: " << index;
    Bubble *bubble = new Bubble(nullptr, notify);
    bubble->setMaskAlpha(static_cast<quint8>(m_appearance->opacity() * 255));
    connect(m_appearance, &Appearance::OpacityChanged, bubble, &Bubble::onOpacityChanged);
    connect(bubble, &Bubble::expired, this, &BubbleManager::bubbleExpired);
    connect(bubble, &Bubble::dismissed, this, &BubbleManager::bubbleDismissed);
    connect(bubble, &Bubble::actionInvoked, this, &BubbleManager::bubbleActionInvoked);
    connect(bubble, &Bubble::notProcessedYet, this, [ this ](EntityPtr ptr) {
        qDebug() << "notProcessedYet, isShowInNotifyCenter: " << ptr->isShowInNotifyCenter();
        if (ptr->isShowInNotifyCenter())
            m_persistence->addOne(ptr);
    });

    if (index != 0) {
        QRect startRect = getBubbleGeometry(BubbleEntities + BubbleOverLap);
        QRect endRect = getBubbleGeometry(BubbleEntities + BubbleOverLap - 1);
        bubble->setBubbleIndex(BubbleEntities + BubbleOverLap - 1);
        qDebug() << "createBubble, body: " << bubble->entity()->body();
        bubble->startMove(startRect, endRect);
    } else {
        QRect endRect = getBubbleGeometry(0);
        QRect startRect = endRect;
        startRect.setHeight(0);

        bubble->setProperty("geometry",0);
        bubble->show();

        QPropertyAnimation *ani = new QPropertyAnimation(bubble, "geometry", this);
        ani->setStartValue(startRect);
        ani->setEndValue(endRect);

        int animationTime = int(endRect.height() * 1.0 / 72 * AnimationTime);
        ani->setDuration(animationTime);

        bubble->setBubbleIndex(0);
        ani->start(QPropertyAnimation::DeleteWhenStopped);
    }

    qDebug() << Q_FUNC_INFO << "end ------- body: " <<notify->body();
    return bubble;
}
