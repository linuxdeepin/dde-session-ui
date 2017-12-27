/**
 * Copyright (C) 2014 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
#include "dbusdock_interface.h"

static const QString ControlCenterDBusService = "com.deepin.dde.ControlCenter";
static const QString ControlCenterDBusPath = "/com/deepin/dde/ControlCenter";
static const QString DBbsDockDBusServer = "com.deepin.dde.Dock";
static const QString DBusDockDBusPath = "/com/deepin/dde/Dock";
static const QString DBusDaemonDBusService = "org.freedesktop.DBus";
static const QString DBusDaemonDBusPath = "/org/freedesktop/DBus";
static const QString NotificationsDBusService = "org.freedesktop.Notifications";
static const QString NotificationsDBusPath = "/org/freedesktop/Notifications";
static const QString Login1DBusService = "org.freedesktop.login1";
static const QString Login1DBusPath = "/org/freedesktop/login1";
static const int ControlCenterWidth = 400;
class DBusControlCenter;
class DBusDaemonInterface;
class Login1ManagerInterface;
class DBusDockInterface;

#ifndef DISABLE_DB
class Persistence;
#endif

class BubbleManager : public QObject
{
    Q_OBJECT
public:
    explicit BubbleManager(QObject *parent = 0);
    ~BubbleManager();

    enum ClosedReason {
        Expired = 1,
        Dismissed = 2,
        Closed = 3,
        Unknown = 4
    };

signals:
    void ActionInvoked(uint, const QString &);
    void NotificationClosed(uint, uint);

    // Extra DBus APIs
signals:
    void RecordAdded(const QString &);

public slots:
    // Notifications dbus implementation
    void CloseNotification(uint);
    QStringList GetCapbilities();
    QString GetServerInformation(QString &, QString &, QString &);
    uint Notify(const QString &, uint, const QString &, const QString &, const QString &, const QStringList &, const QVariantMap, int);

    // Extra DBus APIs
    QString GetAllRecords();
    void RemoveRecord(const QString &id);
    void ClearRecords();
    void AddOneRecord(NotificationEntity *entity);

    void registerAsService();

    void controlCenterRectChangedSlot(const QRect &rect);
    void dockchangedSlot(const QRect &geometry);
    void dbusNameOwnerChangedSlot(QString, QString, QString);

    void bubbleExpired(int);
    void bubbleDismissed(int);
    void bubbleReplacedByOther(int);
    void bubbleActionInvoked(int, QString);
    void bubbleAboutToQuit();

    void onPrepareForSleep(bool);


private:
    Bubble *m_bubble;
#ifndef DISABLE_DB
    Persistence *m_persistence;
#endif
    QQueue<NotificationEntity*> m_entities;
    DBusControlCenter *m_dbusControlCenter;
    DBusDaemonInterface *m_dbusDaemonInterface;
    Login1ManagerInterface *m_login1ManagerInterface;
    DBusDockInterface *m_dbusdockinterface;

    int   m_dccX;
    QRect m_dockGeometry;
    QTimer *m_quitTimer;

    bool checkDockExistence();
    bool checkControlCenterExistence();

    int getX();
    int getY();

    void bindControlCenterX();
    void consumeEntities();

};

#endif // BUBBLEMANAGER_H
