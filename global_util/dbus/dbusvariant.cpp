#include "dbusvariant.h"

Inhibit::Inhibit() {}
Inhibit::~Inhibit() {}

void Inhibit::registerMetaType()
{
    qRegisterMetaType<Inhibit>("Inhibit");
    qDBusRegisterMetaType<Inhibit>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const Inhibit &obj)
{
    argument.beginStructure();
    argument << obj.what << obj.who << obj.why << obj.mode << obj.fd;
    argument.endStructure();
    return argument;
}


const QDBusArgument &operator>>(const QDBusArgument &argument, Inhibit &obj)
{
    argument.beginStructure();
    argument >> obj.what >> obj.who >> obj.why >> obj.mode >> obj.fd;
    argument.endStructure();
    return argument;
}

UserInfo::UserInfo() {}
UserInfo::~UserInfo() {}

void UserInfo::registerMetaType()
{
    qRegisterMetaType<UserInfo>("UserInfo");
    qDBusRegisterMetaType<UserInfo>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const UserInfo &obj)
{
    argument.beginStructure();
    argument << obj.pid << obj.id << obj.userName;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, UserInfo &obj)
{
    argument.beginStructure();
    argument >> obj.pid >> obj.id >> obj.userName;
    argument.endStructure();
    return argument;
}

SeatInfo::SeatInfo() {}
SeatInfo::~SeatInfo() {}

void SeatInfo::registerMetaType() {
    qRegisterMetaType<SeatInfo>("SeatInfo");
    qDBusRegisterMetaType<SeatInfo>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const SeatInfo &obj)
{
    argument.beginStructure();
    argument << obj.id << obj.seat;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, SeatInfo &obj)
{
    argument.beginStructure();
    argument >> obj.id >> obj.seat;
    argument.endStructure();
    return argument;
}

SessionInfo::SessionInfo() {}
SessionInfo::~SessionInfo() {}

void SessionInfo::registerMetaType() {
    qRegisterMetaType<SessionInfo>("SessionInfo");
    qDBusRegisterMetaType<SessionInfo>();
}

QDBusArgument &operator<<(QDBusArgument &argument, const SessionInfo &obj)
{
    argument.beginStructure();
    argument << obj.session << obj.pid << obj.user << obj.id << obj.seat;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, SessionInfo &obj)
{
    argument.beginStructure();
    argument >> obj.session >> obj.pid >> obj.user >> obj.id >> obj.seat;
    argument.endStructure();
    return argument;
}
