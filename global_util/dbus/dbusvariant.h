/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DBUSVARIANT
#define DBUSVARIANT

#include <QtCore>
#include <QtDBus>
#include <QtDBus/QDBusArgument>

class Inhibit {
public:
    QString what;
    QString who;
    QString why;
    QString dosome;
    quint32 mode;
    quint32 fd;

    Inhibit();
    ~Inhibit();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const Inhibit &obj);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, Inhibit &obj);
    static void registerMetaType();
};

class UserInfo {
public:
    qlonglong pid;
    QString id;
    QString userName;

    UserInfo();
    ~UserInfo();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const UserInfo &obj);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, UserInfo &obj);
    static void registerMetaType();
};

class SeatInfo {
public:
    QString id;
    QString seat;

    SeatInfo();
    ~SeatInfo();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const SeatInfo &obj);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, SeatInfo &obj);
    static void registerMetaType();
};

class SessionInfo {
public:
    QString session;
    qlonglong pid;
    QString user;
    QString id;
    QString seat;

    SessionInfo();
    ~SessionInfo();

    friend QDBusArgument &operator<<(QDBusArgument &argument, const SessionInfo &obj);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, SessionInfo &obj);
    static void registerMetaType();
};


typedef QList<Inhibit> InhibitorsList;
Q_DECLARE_METATYPE(Inhibit)
Q_DECLARE_METATYPE(InhibitorsList)


typedef QList<UserInfo> UserList;
Q_DECLARE_METATYPE(UserInfo)
Q_DECLARE_METATYPE(UserList)

typedef QList<SeatInfo> SeatList;
Q_DECLARE_METATYPE(SeatInfo)
Q_DECLARE_METATYPE(SeatList)

typedef QList<SessionInfo> SessionList;
Q_DECLARE_METATYPE(SessionInfo)
Q_DECLARE_METATYPE(SessionList)
#endif // DBUSVARIANT

