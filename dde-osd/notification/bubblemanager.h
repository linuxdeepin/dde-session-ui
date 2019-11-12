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

#ifndef BUBBLEMANAGER_H
#define BUBBLEMANAGER_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QQueue>
#include <QDesktopWidget>
#include <QApplication>
#include <QGuiApplication>
#include "bubble.h"
#include "constants.h"
#include <com_deepin_dde_daemon_dock.h>

using DockDaemonInter =  com::deepin::dde::daemon::Dock;

static const QString DBbsDockDBusServer = "com.deepin.dde.Dock";
static const QString DBusDockDBusPath = "/com/deepin/dde/Dock";
static const QString DBusDaemonDBusService = "org.freedesktop.DBus";
static const QString DBusDaemonDBusPath = "/org/freedesktop/DBus";
static const QString NotificationsDBusService = "org.freedesktop.Notifications";
static const QString NotificationsDBusPath = "/org/freedesktop/Notifications";
static const QString DDENotifyDBusServer = "com.deepin.dde.Notification";
static const QString DDENotifyDBusPath = "/com/deepin/dde/Notification";
static const QString Login1DBusService = "org.freedesktop.login1";
static const QString Login1DBusPath = "/org/freedesktop/login1";
static const QString DockDaemonDBusServie = "com.deepin.dde.daemon.Dock";
static const QString DockDaemonDBusPath = "/com/deepin/dde/daemon/Dock";
static const int ControlCenterWidth = 400;

class DBusControlCenter;
class DBusDaemonInterface;
class Login1ManagerInterface;
class DBusDockInterface;
class Persistence;
class NotifyCenterWidget;
class BubbleTemplate;
class BubbleManager : public QObject
{
    Q_OBJECT
public:
    explicit BubbleManager(QObject *parent = nullptr);
    ~BubbleManager();

    enum ClosedReason {
        Expired = 1,
        Dismissed = 2,
        Closed = 3,
        Unknown = 4
    };

    enum AnimationPath {
        Down = 0,
        Up = 1
    };

Q_SIGNALS:
    // Standard Notifications dbus implementation
    void ActionInvoked(uint, const QString &);
    void NotificationClosed(uint, uint);

    // Extra DBus APIs
    void RecordAdded(const QString &);

public Q_SLOTS:
    // Standard Notifications dbus implementation
    void CloseNotification(uint);
    QStringList GetCapabilities();
    QString GetServerInformation(QString &, QString &, QString &);
    // new notify will be received by this slot
    uint Notify(const QString &, uint replacesId, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap, int);

    // Extra DBus APIs
    QString GetAllRecords();
    QString GetRecordById(const QString &id);
    QString GetRecordsFromId(int rowCount, const QString &offsetId);
    void RemoveRecord(const QString &id);
    void ClearRecords();
    void Toggle();
    uint recordCount();

private Q_SLOTS:
    void onDockPositionChanged(int position);
    void onDockSizeChanged(uint size);
    void onDbusNameOwnerChanged(QString, QString, QString);
    void onPrepareForSleep(bool);

    void bubbleExpired(Bubble *);
    void bubbleDismissed(Bubble *);
    void bubbleReplacedByOther(Bubble *);
    void bubbleActionInvoked(Bubble *, QString);

private:
    void registerAsService();

    bool checkDockExistence();
    bool checkControlCenterExistence();

    int getX();
    int getY();

    // return geometry of the containing specified point screen,
    // and return true if primary-screen and specified-point-screen are the same screen,
    // or return false.
    QPair<QRect, bool> screensInfo(const QPoint &point) const;

    Bubble *createBubble(std::shared_ptr<NotificationEntity> notify, int index = 0);
    void pushBubble(std::shared_ptr<NotificationEntity> notify);
    void popBubble(Bubble *);
    void refreshBubble();

    void pushAnimation(Bubble *bubble);
    void popAnimation(Bubble *bubble);

    QRect GetBubbleGeometry(int index/*0-4*/);
    void PrepareAnimation(Bubble *bubble, int index/*0-4*/, const QRect &endRect);

private:
    Persistence *m_persistence;
    DBusControlCenter *m_dbusControlCenter;
    DBusDaemonInterface *m_dbusDaemonInterface;
    Login1ManagerInterface *m_login1ManagerInterface;
    DockDaemonInter *m_dockDeamonInter;

    QList<std::shared_ptr<NotificationEntity>> m_oldEntities;
    QList<QPointer<Bubble>> m_bubbleList;

    OSD::DockPosition m_dockPosition;
    uint m_dockSize;
    NotifyCenterWidget *m_notifyCenter;

    quint32 m_messageCount = 0;
};

#endif // BUBBLEMANAGER_H
