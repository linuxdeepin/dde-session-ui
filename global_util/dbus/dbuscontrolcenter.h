// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSCONTROLCENTER_H_1442200179
#define DBUSCONTROLCENTER_H_1442200179

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface com.deepin.dde.ControlCenter
 */
class DBusControlCenter: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
    {
        QList<QVariant> arguments = msg.arguments();
        if (3 != arguments.count())
            return;
        QString interfaceName = msg.arguments().at(0).toString();
        if (interfaceName !="com.deepin.dde.ControlCenter")
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
    { return "com.deepin.dde.ControlCenter"; }

public:
    explicit DBusControlCenter(QObject *parent = nullptr);

    ~DBusControlCenter();

    Q_PROPERTY(bool ShowInRight READ showInRight NOTIFY ShowInRightChanged)
    inline bool showInRight() const
    { return qvariant_cast< bool >(property("ShowInRight")); }

    Q_PROPERTY(int X READ x NOTIFY XChanged)
    inline int x() const
    { return qvariant_cast< int >(property("X")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Hide()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Hide"), argumentList);
    }

    inline QDBusPendingReply<> HideImmediately()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("HideImmediately"), argumentList);
    }

    inline QDBusPendingReply<> Show()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Show"), argumentList);
    }

    inline QDBusPendingReply<> ShowImmediately()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ShowImmediately"), argumentList);
    }

    inline QDBusPendingReply<> ShowModule(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QStringLiteral("ShowModule"), argumentList);
    }

    inline QDBusPendingReply<> Toggle()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Toggle"), argumentList);
    }

    inline QDBusPendingReply<> ToggleInLeft()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ToggleInLeft"), argumentList);
    }

    inline QDBusPendingReply<bool> isNetworkCanShowPassword()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("isNetworkCanShowPassword"), argumentList);
    }

Q_SIGNALS: // SIGNALS
// begin property changed signals
void ShowInRightChanged();
void XChanged();
};

namespace com {
  namespace deepin {
    namespace dde {
      typedef ::DBusControlCenter ControlCenter;
    }
  }
}
#endif
