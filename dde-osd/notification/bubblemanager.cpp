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
#include <QStringList>
#include <QVariantMap>
#include <QTimer>
#include <algorithm>
#include "bubble.h"
#include "dbus_daemon_interface.h"
#include "dbuslogin1manager.h"
#include "notificationentity.h"
#include "notification-center/notifycenterwidget.h"

#include "persistence.h"

#include <QTimer>
#include <QDebug>
#include <QXmlStreamReader>
#include <QFocusEvent>

static QString removeHTML(const QString &source)
{
    QXmlStreamReader xml(source);
    QString textString;
    while (!xml.atEnd()) {
        if (xml.readNext() == QXmlStreamReader::Characters) {
            textString += xml.text();
        }
    }

    return textString.isEmpty() ? source : textString;
}

BubbleManager::BubbleManager(QObject *parent)
    : QObject(parent)
    , m_persistence(new Persistence)
    , m_notifyCenter(new NotifyCenterWidget(m_persistence))
{
    m_dockPosition = OSD::DockPosition::Bottom;

    m_dbusDaemonInterface = new DBusDaemonInterface(DBusDaemonDBusService, DBusDaemonDBusPath,
                                                    QDBusConnection::sessionBus(), this);
    m_dockDeamonInter = new DockDaemonInter(DockDaemonDBusServie, DockDaemonDBusPath,
                                            QDBusConnection::sessionBus(), this);
    m_login1ManagerInterface = new Login1ManagerInterface(Login1DBusService, Login1DBusPath,
                                                          QDBusConnection::systemBus(), this);

    connect(m_login1ManagerInterface, SIGNAL(PrepareForSleep(bool)),
            this, SLOT(onPrepareForSleep(bool)));
    connect(m_dbusDaemonInterface, SIGNAL(NameOwnerChanged(QString, QString, QString)),
            this, SLOT(onDbusNameOwnerChanged(QString, QString, QString)));
    connect(m_dockDeamonInter, &DockDaemonInter::PositionChanged, this, &BubbleManager::onDockPositionChanged);
    connect(m_dockDeamonInter, &DockDaemonInter::WindowSizeChanged, this, &BubbleManager::onDockSizeChanged);

    // get correct value for m_dockGeometry, m_dockPosition, m_ccGeometry
    if (m_dockDeamonInter->isValid()) {
        onDockPositionChanged(m_dockDeamonInter->position());
        onDockSizeChanged(m_dockDeamonInter->windowSize());
    }

    m_notifyCenter->hide();
    registerAsService();
}

BubbleManager::~BubbleManager()
{

}

void BubbleManager::CloseNotification(uint id)
{
    QString str_id = QString::number(id);
    foreach (auto bubble, m_bubbleList) {
        if (bubble->entity()->replacesId() == str_id) {
            bubble->close();
        }
    }

    foreach (auto notify, m_oldEntities) {
        if (notify->replacesId() == str_id) {
            m_oldEntities.removeOne(notify);
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
#ifdef QT_DEBUG
    qDebug() << "a new Notify:" << "appName:" + appName << "replaceID:" + QString::number(replacesId)
             << "appIcon:" + appIcon << "summary:" + summary << "body:" + body
             << "actions:" << actions << "hints:" << hints << "expireTimeout:" << expireTimeout;
#endif

    std::shared_ptr<NotificationEntity> notification = std::make_shared<NotificationEntity>(appName, QString(), appIcon,
                                                                                            summary, removeHTML(body), actions, hints,
                                                                                            QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                                                            QString::number(replacesId),
                                                                                            QString::number(expireTimeout),
                                                                                            this);
    m_persistence->addOne(notification);
    pushBubble(notification);
    m_messageCount++;

    // If replaces_id is 0, the return value is a UINT32 that represent the notification.
    // If replaces_id is not 0, the returned value is the same value as replaces_id.
    return replacesId == 0 ? notification->id() : replacesId;
}

void BubbleManager::pushBubble(std::shared_ptr<NotificationEntity> notify)
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
    refreshBubble();
    popAnimation(bubble);
    bubble->deleteLater();
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
        QRect startRect = GetBubbleGeometry(index - 1);
        QRect endRect = GetBubbleGeometry(index);
        QPointer<Bubble> item = m_bubbleList.at(index);
        PrepareAnimation(item, index, endRect);
        if (bubble != nullptr)
            item->startMoveAnimation(startRect, endRect);
    }
}

void BubbleManager::popAnimation(Bubble *bubble)
{
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)
        return;
    while (index < m_bubbleList.size() - 1) {
        index ++;
        QRect startRect = GetBubbleGeometry(index);
        QRect endRect = GetBubbleGeometry(index - 1);
        QPointer<Bubble> item = m_bubbleList.at(index);
        PrepareAnimation(item, index - 1, endRect);
        if (bubble != nullptr)
            item->startMoveAnimation(startRect, endRect);
    }

    //确定层次关系
    for (int i = m_bubbleList.size() - 1; i >= BubbleEntities - 1; --i) {
        Bubble *b = m_bubbleList[i];
        if (b) {
            b->raise();
        }
    }
}

QRect BubbleManager::GetBubbleGeometry(int index)
{
    Q_ASSERT(index >= 0 && index <= BubbleEntities + BubbleOverLap);
    QRect rect;
    if (index >= 0 && index <= BubbleEntities - 1) {
        rect.setX((getX() - OSD::BubbleWidth(OSD::BUBBLEWINDOW)) / 2);
        rect.setY(ScreenPadding + index * (BubbleMargin + OSD::BubbleHeight(OSD::BUBBLEWINDOW)));
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

void BubbleManager::PrepareAnimation(Bubble *bubble, int index/*0-4*/, const QRect &endRect)
{
    Q_ASSERT(index >= 0 && index <= BubbleEntities + BubbleOverLap);
    if (index >= BubbleEntities && index <= BubbleEntities + BubbleOverLap) {
        QSize size = bubble->size();
        bubble->setMinimumSize(0, 0);
        bubble->setMaximumSize(16777215, 16777215);
        bubble->setEnabled(false);
        bubble->resize(size);
    } else {
        bubble->setFixedSize(endRect.width(), endRect.height());
        bubble->setEnabled(true);
    }
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
    m_notifyCenter->showWidget();
}

uint BubbleManager::recordCount()
{
    return m_messageCount;
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
    Q_EMIT ActionInvoked(bubble->entity()->id(), actionId);
    Q_EMIT NotificationClosed(bubble->entity()->id(), BubbleManager::Closed);
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

void BubbleManager::onFocusTabed(Bubble *bubble)
{
    qDebug() << __FUNCTION__;
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)
        return;

    index ++;
    if (index > BubbleEntities - 1) {
        index = 0;
    }

    bubble->clearFocus();
    Bubble *next = m_bubbleList[index];
    next->setFocus();
//    QFocusEvent inEvent(QEvent::FocusIn, Qt::TabFocusReason);
//    qApp->sendEvent(next, &inEvent);
    qDebug() << next->hasFocus();
}

bool BubbleManager::checkDockExistence()
{
    return m_dbusDaemonInterface->NameHasOwner(DBbsDockDBusServer).value();
}

int BubbleManager::getX()
{
    QPair<QRect, bool> pair = screensInfo(QCursor::pos());
    const QRect &rect = pair.first;
    const int maxX = rect.x() + rect.width();

    // directly show the notify on the screen containing mouse,
    // because dock  will only be displayed on the primary screen.
    if (!pair.second)
        return  maxX;

    return maxX;
}

int BubbleManager::getY()
{
    QPair<QRect, bool> pair = screensInfo(QCursor::pos());
    const QRect &rect = pair.first;

    if (!pair.second)
        return  rect.y();

    return rect.y();
}

QPair<QRect, bool> BubbleManager::screensInfo(const QPoint &point) const
{
    QDesktopWidget *desktop = QApplication::desktop();
    int pointScreen = desktop->screenNumber(point);
    int primaryScreen = desktop->primaryScreen();

    QRect rect = desktop->screenGeometry(pointScreen);

    return QPair<QRect, bool>(rect, (pointScreen == primaryScreen));
}

void BubbleManager::onDockPositionChanged(int position)
{
    m_dockPosition = static_cast<OSD::DockPosition>(position);
    m_notifyCenter->updateGeometry(m_dockPosition, m_dockSize);
}

void BubbleManager::onDockSizeChanged(uint size)
{
    m_dockSize = size;
    m_notifyCenter->updateGeometry(m_dockPosition, size);
}

void BubbleManager::onDbusNameOwnerChanged(QString name, QString, QString newName)
{
    if (name == DockDaemonDBusServie && !newName.isEmpty()) {
        onDockPositionChanged(m_dockDeamonInter->position());
    }
}

Bubble *BubbleManager::createBubble(std::shared_ptr<NotificationEntity> notify, int index)
{
    Bubble *bubble = new Bubble(nullptr, notify);
    connect(bubble, &Bubble::expired, this, &BubbleManager::bubbleExpired);
    connect(bubble, &Bubble::dismissed, this, &BubbleManager::bubbleDismissed);
    connect(bubble, &Bubble::replacedByOther, this, &BubbleManager::bubbleReplacedByOther);
    connect(bubble, &Bubble::actionInvoked, this, &BubbleManager::bubbleActionInvoked);
    connect(bubble, &Bubble::focusTabed, this, &BubbleManager::onFocusTabed);

    if (index != 0) {
        QRect startRect = GetBubbleGeometry(5);
        QRect endRect = GetBubbleGeometry(4);
        PrepareAnimation(bubble, 4, endRect);
        bubble->StartMoveIn(startRect, endRect);
    } else {
        bubble->StartMoveIn(getX(), getY() + ScreenPadding);
    }

    return bubble;
}
