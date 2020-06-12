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

#include <QStringList>
#include <QVariantMap>
#include <QTimer>
#include <QDebug>
#include <QScreen>

#include <algorithm>

#include "bubblemanager.h"
#include "bubble.h"
#include "dbus_daemon_interface.h"
#include "dbuslogin1manager.h"
#include "notificationentity.h"
#include "dbusdisplay.h"
#include "dbusdock.h"
#include "persistence.h"
#include "constants.h"
#include "notifysettings.h"
#include "notification-center/notifycenterwidget.h"

#include <com_deepin_daemon_display.h>
#include <com_deepin_daemon_display_monitor.h>
#include "dbusdock_interface.h"

using DisplayInter = com::deepin::daemon::Display;
using MonitorInter = com::deepin::daemon::display::Monitor;

BubbleManager::BubbleManager(QObject *parent)
    : QObject(parent)
    , m_persistence(new Persistence)
    , m_displayInter(new DBusDisplay)
    , m_dockDeamonInter(new DBusDock)
    , m_userInter(new UserInter("com.deepin.SessionManager",
                                "/com/deepin/SessionManager",
                                QDBusConnection::sessionBus(), this))
    , m_launcherInter(new LauncherInter("com.deepin.dde.daemon.Launcher",
                                        "/com/deepin/dde/daemon/Launcher",
                                        QDBusConnection::sessionBus(), this))
    , m_notifySettings(new NotifySettings(this))
    , m_notifyCenter(new NotifyCenterWidget(m_persistence))
    , m_checkDndTimer(new QTimer(this))
    , m_soundeffectInter(new SoundeffectInter("com.deepin.daemon.SoundEffect",
                                              "/com/deepin/daemon/SoundEffect",
                                              QDBusConnection::sessionBus(), this))
    , m_dockInter(new DBusDockInterface("com.deepin.dde.Dock",
                                        "/com/deepin/dde/Dock",
                                        QDBusConnection::sessionBus(), this))

{
    m_dbusDaemonInterface = new DBusDaemonInterface(DBusDaemonDBusService, DBusDaemonDBusPath,
                                                    QDBusConnection::sessionBus(), this);
    m_login1ManagerInterface = new Login1ManagerInterface(Login1DBusService, Login1DBusPath,
                                                          QDBusConnection::systemBus(), this);

    initConnections();

    m_checkDndTimer->setInterval(900);
    m_checkDndTimer->start();

    updateSysNotifyProperty();

    // get correct value for m_dockGeometry, m_dockPosition, m_ccGeometry
    if (m_dockDeamonInter->isValid()) {
        geometryChanged();
    }
    m_notifyCenter->hide();
    registerAsService();
}

BubbleManager::~BubbleManager()
{
    if (!m_bubbleList.isEmpty()) qDeleteAll(m_bubbleList);

    m_oldEntities.clear();
}

void BubbleManager::CloseNotification(uint id)
{
#ifdef QT_DEBUG
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
    qDebug() << "Notify:" << "appName:" + appName << "replaceID:" + QString::number(replacesId)
             << "appIcon:" + appIcon << "summary:" + summary << "body:" + body
             << "actions:" << actions << "hints:" << hints << "expireTimeout:" << expireTimeout;

    QString strBody = body;
    strBody.replace(QLatin1String("\\\\"), QLatin1String("\\"), Qt::CaseInsensitive);

    EntityPtr notification = std::make_shared<NotificationEntity>(appName, QString(), appIcon,
                                                                  summary, strBody, actions, hints,
                                                                  QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                                  QString::number(replacesId),
                                                                  QString::number(expireTimeout),
                                                                  this);

    AppNotifyProperty appNotifyProperty = getAppNotifyProperty(notification->appName());

    if (!appNotifyProperty.isAllowNotify)
        return 0;

    notification->setShowPreview(appNotifyProperty.isShowNotifyPreview);

    if (!calcReplaceId(notification)) {
        if (isDoNotDisturb()) {
            if (WhiteBoardAppList.contains(notification->appName())) {
                pushBubble(notification);
            } else {
                m_persistence->addOne(notification);
            }
        } else if ((m_userInter->locked() && !appNotifyProperty.isLockShowNotify)
                   || appNotifyProperty.isOnlyInNotifyCenter) {
            m_persistence->addOne(notification);
        } else {
            pushBubble(notification);
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
    // bubble delete itself when aniamtion finished
    refreshBubble();
    popAnimation(bubble);
    m_bubbleList.removeOne(bubble);
}

void BubbleManager::refreshBubble()
{
    if (m_bubbleList.size() < BubbleEntities + BubbleOverLap + 1 && !m_oldEntities.isEmpty()) {
        auto notify = m_oldEntities.takeFirst();
        Bubble *bubble = createBubble(notify, BubbleEntities + BubbleOverLap - 1);
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
        QRect startRect = GetLastStableRect(index - 1);
        QRect endRect = GetBubbleGeometry(index);
        QPointer<Bubble> item = m_bubbleList.at(index);
        if (item->geometry() != endRect) { //动画中
            startRect = item->geometry();
        }
        if (bubble != nullptr) {
            item->setBubbleIndex(index);
            item->startMove(startRect, endRect);
        }
    }
}

void BubbleManager::popAnimation(Bubble *bubble)
{
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)
        return;

    QRect startRect = GetBubbleGeometry(index);
    QRect endRect;
    endRect.setX(startRect.x());
    endRect.setY(BubbleStartPos);
    endRect.setSize(startRect.size());
    bubble->startMove(startRect, endRect, true);// delete itself

    while (index < m_bubbleList.size() - 1) {
        index ++;
        QRect startRect = GetBubbleGeometry(index);
        QRect endRect = GetBubbleGeometry(index - 1);
        QPointer<Bubble> item = m_bubbleList.at(index);
        if (index == BubbleEntities + BubbleOverLap) {
            item->show();
        }
        if (item->geometry() != endRect) { //动画中
            startRect = item->geometry();
        }
        if (bubble != nullptr) {
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
}

QRect BubbleManager::GetBubbleGeometry(int index)
{
    Q_ASSERT(index >= 0 && index <= BubbleEntities + BubbleOverLap);

    QRect display = calcDisplayRect();
    QRect rect;
    if (index >= 0 && index <= BubbleEntities - 1) {
        rect.setX(display.x() + (display.width() - OSD::BubbleWidth(OSD::BUBBLEWINDOW)) / 2);
        rect.setY(display.y() + ScreenPadding + index * (BubbleMargin + OSD::BubbleHeight(OSD::BUBBLEWINDOW)));
        rect.setWidth(OSD::BubbleWidth(OSD::BUBBLEWINDOW));
        rect.setHeight(OSD::BubbleHeight(OSD::BUBBLEWINDOW));
    } else if (index >= BubbleEntities && index <= BubbleEntities + BubbleOverLap) {
        rect = GetBubbleGeometry(index - 1);

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

QRect BubbleManager::GetLastStableRect(int index)
{
    QRect rect = GetBubbleGeometry(0);
    for (int i = index - 1; i > 0; --i) {
        if (i >= m_bubbleList.size() || m_bubbleList.at(i)->geometry() != GetBubbleGeometry(i)) {
            continue;
        }
        rect = GetBubbleGeometry(i);
    }

    return rect;
}

bool BubbleManager::isDoNotDisturb()
{
    if (m_sysNotifyProperty.isDoNotDisturb) {
        //目前无法判断是否有投影仪连接,默认为false
        bool hasProjector = false;
        //目前无法判断是否有应用全屏默认写false
        bool hasAppFull = false;
        bool isLock = m_userInter->locked();
        bool isTimeMeet = false;

        int currentTime = QTime::currentTime().hour();
        int startTime = m_sysNotifyProperty.StartTime;
        int endTime = m_sysNotifyProperty.EndTime;
        //判断当前时间是否再时间段内
        if (startTime < endTime) {
            if (startTime <= currentTime && endTime >= currentTime) {
                isTimeMeet = true;
            } else {
                isTimeMeet = false;
            }
        } else if (startTime > endTime) {
            if (startTime <= currentTime || endTime >= currentTime) {
                isTimeMeet = true;
            } else {
                isTimeMeet = false;
            }
        } else {
            isTimeMeet = true;
        }
        //时间段满足
        if (isTimeMeet && m_sysNotifyProperty.isTimeSlot) {
            return true;
        } else {
            return (hasProjector && m_sysNotifyProperty.isConnectedProjector)
                   || (hasAppFull && m_sysNotifyProperty.isAppsInFullscreen)
                   || (m_sysNotifyProperty.isScreenLocked && isLock);
        }
    } else {
        return false;
    }
}

AppNotifyProperty BubbleManager::getAppNotifyProperty(QString appName)
{
    QJsonObject currentObj = QJsonDocument::fromJson(getAppSetting(appName).toUtf8()).object();
    QJsonObject currentSettingsObj = currentObj.begin().value().toObject();

    AppNotifyProperty appNotifyProperty;
    if (currentSettingsObj.contains(AllowNotifyStr)) {
        appNotifyProperty.isAllowNotify = currentSettingsObj[AllowNotifyStr].toBool();
    } else {
        appNotifyProperty.isAllowNotify = DEFAULT_ALLOW_NOTIFY;
    }

    if (currentSettingsObj.contains(OnlyInNotifyCenterStr)) {
        appNotifyProperty.isOnlyInNotifyCenter = currentSettingsObj[OnlyInNotifyCenterStr].toBool();
    } else {
        appNotifyProperty.isOnlyInNotifyCenter = DEFAULT_ONLY_IN_NOTIFY;
    }

    if (currentSettingsObj.contains(LockShowNotifyStr)) {
        appNotifyProperty.isLockShowNotify = currentSettingsObj[LockShowNotifyStr].toBool();
    } else {
        appNotifyProperty.isLockShowNotify = DEFAULT_LOCK_SHOW_NOTIFY;
    }

    if (currentSettingsObj.contains(NotificationSoundStr)) {
        appNotifyProperty.isNotificationSound = currentSettingsObj[NotificationSoundStr].toBool();
    } else {
        appNotifyProperty.isNotificationSound = DEFAULT_NOTIFY_SOUND;
    }

    if (currentSettingsObj.contains(ShowNotifyPreviewStr)) {
        appNotifyProperty.isShowNotifyPreview = currentSettingsObj[ShowNotifyPreviewStr].toBool();
    } else {
        appNotifyProperty.isShowNotifyPreview = DEFAULT_SHOW_NOTIFY_PREVIEW;
    }

    return appNotifyProperty;
}

void BubbleManager::updateSysNotifyProperty()
{
    QJsonObject currentObj = QJsonDocument::fromJson(getSystemSetting().toUtf8()).object();
    QJsonObject currentSettingsObj = currentObj.begin().value().toObject();
    if (currentSettingsObj.contains(DoNotDisturbStr)) {
        m_sysNotifyProperty.isDoNotDisturb = currentSettingsObj[DoNotDisturbStr].toBool();
    } else {
        m_sysNotifyProperty.isDoNotDisturb = DEFAULT_DO_NOT_DISTURB;
    }

    if (currentSettingsObj.contains(TimeSlotStr)) {
        m_sysNotifyProperty.isTimeSlot = currentSettingsObj[TimeSlotStr].toBool();
    } else {
        m_sysNotifyProperty.isTimeSlot = DEFAULT_TIME_SLOT;
    }

    if (currentSettingsObj.contains(StartTimeStr)) {
        m_sysNotifyProperty.StartTime = currentSettingsObj[StartTimeStr].toInt();
    } else {
        m_sysNotifyProperty.StartTime = DEFAULT_START_TIME;
    }

    if (currentSettingsObj.contains(EndTimeStr)) {
        m_sysNotifyProperty.EndTime = currentSettingsObj[EndTimeStr].toInt();
    } else {
        m_sysNotifyProperty.EndTime = DEFAULT_END_TIME;
    }

    if (currentSettingsObj.contains(AppsInFullscreenStr)) {
        m_sysNotifyProperty.isAppsInFullscreen = currentSettingsObj[AppsInFullscreenStr].toBool();
    } else {
        m_sysNotifyProperty.isAppsInFullscreen = DEFAULT_APP_IN_FULLSCREEN;
    }

    if (currentSettingsObj.contains(AppsInFullscreenStr)) {
        m_sysNotifyProperty.isAppsInFullscreen = currentSettingsObj[AppsInFullscreenStr].toBool();
    } else {
        m_sysNotifyProperty.isAppsInFullscreen = DEFAULT_APP_IN_FULLSCREEN;
    }

    if (currentSettingsObj.contains(ConnectedProjectorStr)) {
        m_sysNotifyProperty.isConnectedProjector = currentSettingsObj[ConnectedProjectorStr].toBool();
    } else {
        m_sysNotifyProperty.isConnectedProjector = DEFAULT_CONNECTED_PROJECTOR;
    }

    if (currentSettingsObj.contains(ScreenLockedStr)) {
        m_sysNotifyProperty.isScreenLocked = currentSettingsObj[ScreenLockedStr].toBool();
    } else {
        m_sysNotifyProperty.isScreenLocked = DEFAULT_SCREEN_LOCKED;
    }

    if (currentSettingsObj.contains(ShowIconOnDockStr)) {
        m_sysNotifyProperty.isShowIconOnDock = currentSettingsObj[ShowIconOnDockStr].toBool();
    } else {
        m_sysNotifyProperty.isShowIconOnDock = DEFAULT_SHOW_ICON_ON_DOCK;
    }
}

QRect BubbleManager::calcDisplayRect()
{
    QRect displayRect;
    QList<QDBusObjectPath> screenList = m_displayInter->monitors();

    for (auto screen : screenList) {
        MonitorInter *monitor = new MonitorInter("com.deepin.daemon.Display", screen.path(), QDBusConnection::sessionBus());
        if (monitor->enabled()
                && QCursor::pos().x() >= monitor->x() && QCursor::pos().x() <= monitor->x() + monitor->width()
                && QCursor::pos().y() >= monitor->y() && QCursor::pos().y() <= monitor->y() + monitor->height()) {
            qDebug() << " screen display : " << screen.path();
            displayRect = QRect(monitor->x(), monitor->y(),
                                monitor->width() / qApp->primaryScreen()->devicePixelRatio(),
                                monitor->height() / qApp->primaryScreen()->devicePixelRatio());
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
    geometryChanged();
    m_notifyCenter->showWidget();
}

uint BubbleManager::recordCount()
{
    return m_persistence->getRecordCount();
}

void BubbleManager::appInfoChanged(QString action, ItemInfo info)
{
    if (action == DeletedAction) {
        m_notifySettings->appRemoved(info.m_key);
        Q_EMIT appRemoved(info.m_key);
    } else if (action == CreatedAction) {
        m_notifySettings->appAdded(info);
        Q_EMIT appAdded(m_notifySettings->getAppSetings(info.m_key));
    }
}

QString BubbleManager::getAllSetting()
{
    return m_notifySettings->getAllSetings();
}

void BubbleManager::setAllSetting(const QString settings)
{
    m_notifySettings->setAllSetting(settings);
}

QString BubbleManager::getAppSetting(QString appName)
{
    return m_notifySettings->getAppSetings(appName);
}

void BubbleManager::setAppSetting(const QString settings)
{
    QJsonObject currentObj = QJsonDocument::fromJson(settings.toUtf8()).object();
    m_notifySettings->setAppSetting(settings);
    Q_EMIT systemSettingChanged(currentObj.begin().key());
}

QString BubbleManager::getSystemSetting()
{
    return m_notifySettings->getSystemSetings();
}

void BubbleManager::setSystemSetting(QString settings)
{
    m_notifySettings->setSystemSetting(settings);
    updateSysNotifyProperty();
    Q_EMIT systemSettingChanged(m_notifySettings->getSystemSetings());
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
    popBubble(bubble);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Expired);
}

void BubbleManager::bubbleDismissed(Bubble *bubble)
{
    popBubble(bubble);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Dismissed);
}

void BubbleManager::bubbleReplacedByOther(Bubble *bubble)
{
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Unknown);
}

void BubbleManager::bubbleActionInvoked(Bubble *bubble, QString actionId)
{
    popBubble(bubble);
    Q_EMIT ActionInvoked(bubble->entity()->id(), actionId);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Closed);
}

void BubbleManager::updateGeometry()
{
    foreach (auto item, m_bubbleList) {
        item->setGeometry(GetBubbleGeometry(item->bubbleIndex()));
        item->updateGeometry();
    }
}

void BubbleManager::initConnections()
{
    connect(m_login1ManagerInterface, SIGNAL(PrepareForSleep(bool)),
            this, SLOT(onPrepareForSleep(bool)));
    connect(m_dbusDaemonInterface, SIGNAL(NameOwnerChanged(QString, QString, QString)),
            this, SLOT(onDbusNameOwnerChanged(QString, QString, QString)));

    connect(m_displayInter, &DBusDisplay::PrimaryRectChanged, this, &BubbleManager::geometryChanged, Qt::QueuedConnection);
    connect(m_dockDeamonInter, &DBusDock::FrontendRectChanged, this, &BubbleManager::geometryChanged, Qt::UniqueConnection);

    connect(qApp, &QApplication::primaryScreenChanged, this, [ = ] {
        updateGeometry();
    });

    connect(qApp->primaryScreen(), &QScreen::geometryChanged, this, [ = ] {
        updateGeometry();
    });

    connect(m_launcherInter, &LauncherInter::ItemChanged, this, &BubbleManager::appInfoChanged);

    connect(m_checkDndTimer, &QTimer::timeout, this, [ = ] {
        m_soundeffectInter->setEnabled(!isDoNotDisturb());
    });
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

bool BubbleManager::checkDockExistence()
{
    return m_dbusDaemonInterface->NameHasOwner(DBbsDockDBusServer).value();
}

void BubbleManager::geometryChanged()
{
    QRect displayRect = calcDisplayRect();
    QRect dock = m_dockInter->geometry();
    if ((dock.topLeft().x() > QCursor::pos().x() || dock.topLeft().x() + dock.width() < QCursor::pos().x())) {
        dock.setHeight(0);
    }

    OSD::DockPosition pos = static_cast<OSD::DockPosition>(m_dockDeamonInter->position());

    int mode = m_dockDeamonInter->displayMode();
    m_notifyCenter->updateGeometry(displayRect, dock, pos, mode);
}

void BubbleManager::onDbusNameOwnerChanged(QString name, QString, QString newName)
{
    if (name == DockDaemonDBusServie && !newName.isEmpty()) {
        geometryChanged();
    }
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
    Bubble *bubble = new Bubble(nullptr, notify);
    connect(bubble, &Bubble::expired, this, &BubbleManager::bubbleExpired);
    connect(bubble, &Bubble::dismissed, this, &BubbleManager::bubbleDismissed);
    connect(bubble, &Bubble::actionInvoked, this, &BubbleManager::bubbleActionInvoked);
    connect(bubble, &Bubble::notProcessedYet, this, [ = ](EntityPtr ptr) {
        m_persistence->addOne(ptr);
    });

    if (index != 0) {
        QRect startRect = GetBubbleGeometry(BubbleEntities + BubbleOverLap);
        QRect endRect = GetBubbleGeometry(BubbleEntities + BubbleOverLap - 1);
        bubble->setBubbleIndex(BubbleEntities + BubbleOverLap - 1);
        bubble->startMove(startRect, endRect);
    } else {
        QRect endRect = GetBubbleGeometry(0);
        QRect startRect;
        startRect.setX(endRect.x());
        startRect.setY(endRect.y() + BubbleStartPos);
        startRect.setWidth(endRect.width());
        startRect.setHeight(endRect.height());

        bubble->setFixedSize(startRect.size());
        bubble->move(startRect.topLeft());

        QTimer::singleShot(0, bubble, [ = ] {
            bubble->show();
        });
        bubble->setBubbleIndex(0);
        bubble->startMove(startRect, endRect);
    }
    return bubble;
}
