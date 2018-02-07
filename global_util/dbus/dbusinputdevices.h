/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#ifndef DBUSINPUTDEVICES_H_1439802129
#define DBUSINPUTDEVICES_H_1439802129

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

struct InputDevice {
    QString interface;
    QString deviceType;
};

typedef QList<InputDevice> InputDeviceList;

Q_DECLARE_METATYPE(InputDevice)
Q_DECLARE_METATYPE(InputDeviceList)

QDBusArgument &operator<<(QDBusArgument &argument, const InputDevice &device);
const QDBusArgument &operator>>(const QDBusArgument &argument, InputDevice &device);


/*
 * Proxy class for interface com.deepin.daemon.InputDevices
 */
class DBusInputDevices: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
        {
            QList<QVariant> arguments = msg.arguments();
            if (3 != arguments.count())
                return;
            QString interfaceName = msg.arguments().at(0).toString();
            if (interfaceName !="com.deepin.daemon.InputDevices")
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
    { return "com.deepin.daemon.InputDevices"; }
    static inline const char *staticServiceName()
    { return "com.deepin.daemon.InputDevices"; }
    static inline const char *staticObjectPath()
    { return "/com/deepin/daemon/InputDevices"; }

public:
    DBusInputDevices(QObject *parent = 0);

    ~DBusInputDevices();

    Q_PROPERTY(InputDeviceList Infos READ infos NOTIFY InfosChanged)
    inline InputDeviceList infos() const
    { return qvariant_cast< InputDeviceList >(property("Infos")); }

public Q_SLOTS: // METHODS
Q_SIGNALS: // SIGNALS
    // begin property changed signals
    void InfosChanged();
};

namespace com {
  namespace deepin {
    namespace daemon {
      typedef ::DBusInputDevices InputDevices;
    }
  }
}
#endif
