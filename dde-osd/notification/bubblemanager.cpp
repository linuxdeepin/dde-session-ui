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
{
    m_persistence = new Persistence;
    m_dockPosition = DockPosition::Bottom;

    m_dbusDaemonInterface = new DBusDaemonInterface(DBusDaemonDBusService, DBusDaemonDBusPath,
                                                    QDBusConnection::sessionBus(), this);
    m_dbusdockinterface = new DBusDockInterface(DBbsDockDBusServer, DBusDockDBusPath,
                                                QDBusConnection::sessionBus(), this);
    m_dockDeamonInter = new DockDaemonInter(DockDaemonDBusServie, DockDaemonDBusPath,
                                            QDBusConnection::sessionBus(), this);
    m_login1ManagerInterface = new Login1ManagerInterface(Login1DBusService, Login1DBusPath,
                                                          QDBusConnection::systemBus(), this);

    connect(m_login1ManagerInterface, SIGNAL(PrepareForSleep(bool)),
            this, SLOT(onPrepareForSleep(bool)));
    connect(m_dbusDaemonInterface, SIGNAL(NameOwnerChanged(QString, QString, QString)),
            this, SLOT(onDbusNameOwnerChanged(QString, QString, QString)));
    connect(m_dbusdockinterface, &DBusDockInterface::geometryChanged, this, &BubbleManager::onDockRectChanged);
    connect(m_dockDeamonInter, &DockDaemonInter::PositionChanged, this, &BubbleManager::onDockPositionChanged);

    // get correct value for m_dockGeometry, m_dockPosition, m_ccGeometry
    if (m_dbusdockinterface->isValid())
        onDockRectChanged(m_dbusdockinterface->geometry());
    if (m_dockDeamonInter->isValid())
        onDockPositionChanged(m_dockDeamonInter->position());

    m_notifyCenter = new NotifyCenterWidget(m_persistence);
    m_notifyCenter->hide();

    registerAsService();
}

BubbleManager::~BubbleManager()
{

}

void BubbleManager::CloseNotification(uint id)
{
    Q_UNUSED(id);
    return;
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

    if (m_bubbleList.size() == BubbleEntities + 2) {
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
    if (m_bubbleList.size() < BubbleEntities + 3 && !m_oldEntities.isEmpty()) {
        auto notify = m_oldEntities.takeFirst();
        Bubble *bubble = createBubble(notify);
        bubble->setPostion(QPoint(bubble->x(), bubble->y() +
                                  OSD::BubbleHeight(bubble->showStyle()) * BubbleEntities +
                                  BubbleMargin * BubbleEntities));
        m_bubbleList.push_back(bubble);
    }
}

void BubbleManager::pushAnimation(Bubble *bubble)
{
    QPoint move_point(bubble->postion());
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)  return;

    qreal m_scalRatio = 1.0;
    while (index < m_bubbleList.size() - 1) {
        index ++;
        // overlap bubble
        if (index > BubbleEntities - 1) {
            m_scalRatio = (m_scalRatio * 19) / 20;
            Bubble *overlap = m_bubbleList.at(index);
            overlap->show();
            Bubble *up_bubble = m_bubbleList.at(index - 1);

            QSize standard_size = OSD::BubbleSize(OSD::BUBBLEWINDOW) * m_scalRatio;
            overlap->setFixedSize(standard_size);
            int lr_margin = (up_bubble->width() - overlap->width()) / 2;

            QPoint up_postion = up_bubble->postion();
            move_point = QPoint(up_postion.x() + lr_margin, up_postion.y() + 10);
        } else {
            move_point.setY(move_point.y() + OSD::BubbleHeight(bubble->showStyle()) + BubbleMargin);
        }

        QPointer<Bubble> item = m_bubbleList.at(index);
        if (bubble != nullptr) item->startMoveAnimation(move_point);
    }
}

void BubbleManager::popAnimation(Bubble *bubble)
{
    QPoint move_point(bubble->postion());
    int index = m_bubbleList.indexOf(bubble);
    if (index == -1)  return;

    qreal m_scalRatio = 1.0;
    while (index < m_bubbleList.size() - 1) {
        index ++;
        QPointer<Bubble> item = m_bubbleList.at(index);
        QPoint up_position = item->postion();
        if (bubble != nullptr) item->startMoveAnimation(move_point);

        // overlap bubble pop
        if (index > BubbleEntities - 1) {
            QPointer<Bubble> overlap = m_bubbleList.at(index);
            if (index == BubbleEntities) {
                overlap->setFixedSize(OSD::BubbleSize(OSD::BUBBLEWINDOW));
                overlap->raise();
            } else {
                m_scalRatio = (m_scalRatio * 19) / 20;
                overlap->setFixedSize(OSD::BubbleSize(OSD::BUBBLEWINDOW) * m_scalRatio);
                overlap->lower();
            }
        }
        move_point  = up_position;
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

    const bool isDockDbusValid = m_dbusdockinterface->isValid();

    // DBus object is invalid, return screen right
    if (!isDockDbusValid)
        return maxX;

    // if dock dbus is valid and dock position is right
    if (isDockDbusValid && m_dockPosition == DockPosition::Right) {
        return maxX - m_dockGeometry.width();
    }

    return maxX;
}

int BubbleManager::getY()
{
    QPair<QRect, bool> pair = screensInfo(QCursor::pos());
    const QRect &rect = pair.first;

    if (!pair.second)
        return  rect.y();

    if (!m_dbusdockinterface->isValid())
        return rect.y();

    if (m_dockPosition == DockPosition::Top)
        return m_dockGeometry.bottom();

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

void BubbleManager::onDockRectChanged(const QRect &geometry)
{
    m_dockGeometry = geometry;

    //m_bubble->setBasePosition(getX(), getY());
}

void BubbleManager::onDockPositionChanged(int position)
{
    m_dockPosition = static_cast<DockPosition>(position);
}

void BubbleManager::onDbusNameOwnerChanged(QString name, QString, QString newName)
{
    if (name == DBbsDockDBusServer && !newName.isEmpty()) {
        onDockRectChanged(m_dbusdockinterface->geometry());
    } else if (name == DockDaemonDBusServie && !newName.isEmpty()) {
        onDockPositionChanged(m_dockDeamonInter->position());
    }
}

Bubble *BubbleManager::createBubble(std::shared_ptr<NotificationEntity> notify)
{
    Bubble *bubble = new Bubble(nullptr, notify);
    connect(bubble, &Bubble::expired, this, &BubbleManager::bubbleExpired);
    connect(bubble, &Bubble::dismissed, this, &BubbleManager::bubbleDismissed);
    connect(bubble, &Bubble::replacedByOther, this, &BubbleManager::bubbleReplacedByOther);
    connect(bubble, &Bubble::actionInvoked, this, &BubbleManager::bubbleActionInvoked);

    QDesktopWidget *desktop = QApplication::desktop();
    int pointerScreen = desktop->screenNumber(QCursor::pos());
    int primaryScreen = desktop->primaryScreen();
    QWidget *pScreenWidget = desktop->screen(primaryScreen);

    if (pointerScreen != primaryScreen)
        pScreenWidget = desktop->screen(pointerScreen);

    bubble->setBasePosition(getX(), getY(), pScreenWidget->geometry());
    return bubble;
}
