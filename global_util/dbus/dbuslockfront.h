#ifndef DBUSLOCKFRONT_H
#define DBUSLOCKFRONT_H

#include <QDBusAbstractInterface>
#include <QDBusPendingReply>
#include <QMetaObject>
#include <QMetaProperty>

/*
 * Proxy class for interface com.deepin.dde.lockFront
 */
class DBusLockFront: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
    {
        QList<QVariant> arguments = msg.arguments();
        if (3 != arguments.count())
            return;
        QString interfaceName = msg.arguments().at(0).toString();
        if (interfaceName !="com.deepin.dde.lockFront")
            return;
        QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
        foreach(const QString &prop, changedProps.keys()) {
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
    { return "com.deepin.dde.lockFront"; }

public:
    explicit DBusLockFront(QObject *parent = 0);

    virtual ~DBusLockFront();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Hide()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Hide"), argumentList);
    }

};

namespace com {
  namespace deepin {
    namespace dde {
      typedef ::DBusLockFront LockFront;
    }
  }
}
#endif // DBUSLOCKFRONT_H
