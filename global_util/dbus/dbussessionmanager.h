// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSSESSIONMANAGER_H_1441531428
#define DBUSSESSIONMANAGER_H_1441531428

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusAbstractInterface>
/*
 * Proxy class for interface com.deepin.SessionManager
 */
class DBusSessionManagerInterface: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
    {
        QList<QVariant> arguments = msg.arguments();
        if (3 != arguments.count())
            return;
        QString interfaceName = msg.arguments().at(0).toString();
        if (interfaceName !="com.deepin.SessionManager")
            return;
        QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
        QStringList keys = changedProps.keys();
        foreach(const QString &prop, keys) {
        const QMetaObject* self = metaObject();
            for (int i=self->propertyOffset(); i < self->propertyCount(); ++i) {
                QMetaProperty p = self->property(i);
                if (p.name() == prop) {
                Q_EMIT p.notifySignal().invoke(this);
                }
            }
        }
   }


public:
    static inline const char *staticInterfaceName()
    { return "com.deepin.SessionManager"; }

public:
    DBusSessionManagerInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusSessionManagerInterface();

    Q_PROPERTY(QString CurrentUid READ currentUid)
    inline QString currentUid() const
    { return qvariant_cast< QString >(property("CurrentUid")); }

    Q_PROPERTY(int Stage READ stage)
    inline int stage() const
    { return qvariant_cast< int >(property("Stage")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<bool> CanHibernate()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanHibernate"), argumentList);
    }

    inline QDBusPendingReply<bool> CanLogout()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanLogout"), argumentList);
    }

    inline QDBusPendingReply<bool> CanReboot()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanReboot"), argumentList);
    }

    inline QDBusPendingReply<bool> CanShutdown()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanShutdown"), argumentList);
    }

    inline QDBusPendingReply<bool> CanSuspend()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("CanSuspend"), argumentList);
    }

    inline QDBusPendingReply<> ForceLogout()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ForceLogout"), argumentList);
    }

    inline QDBusPendingReply<> ForceReboot()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ForceReboot"), argumentList);
    }

    inline QDBusPendingReply<> ForceShutdown()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ForceShutdown"), argumentList);
    }

    inline QDBusPendingReply<> Logout()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Logout"), argumentList);
    }

    inline QDBusPendingReply<> PowerOffChoose()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("PowerOffChoose"), argumentList);
    }

    inline QDBusPendingReply<> Reboot()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Reboot"), argumentList);
    }

    inline QDBusPendingReply<bool> Register(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("Register"), argumentList);
    }

    inline QDBusPendingReply<> RequestHibernate()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestHibernate"), argumentList);
    }

    inline QDBusPendingReply<> RequestLock()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestLock"), argumentList);
    }

    inline QDBusPendingReply<> RequestLogout()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestLogout"), argumentList);
    }

    inline QDBusPendingReply<> RequestReboot()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestReboot"), argumentList);
    }

    inline QDBusPendingReply<> RequestShutdown()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestShutdown"), argumentList);
    }

    inline QDBusPendingReply<> RequestSuspend()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("RequestSuspend"), argumentList);
    }

    inline QDBusPendingReply<bool> ShowGuideOnce()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ShowGuideOnce"), argumentList);
    }

    inline QDBusPendingReply<> Shutdown()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Shutdown"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace deepin {
    typedef ::DBusSessionManagerInterface DBusSessionManager;
  }
}
#endif
