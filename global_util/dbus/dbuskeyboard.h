// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSKEYBOARD_H_1439802333
#define DBUSKEYBOARD_H_1439802333

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

typedef QMap<QString, QString> KeyboardLayoutList;

Q_DECLARE_METATYPE(KeyboardLayoutList)

/*
 * Proxy class for interface com.deepin.daemon.InputDevice.Keyboard
 */
class DBusKeyboard: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
        {
            QList<QVariant> arguments = msg.arguments();
            if (3 != arguments.count())
                return;
            QString interfaceName = msg.arguments().at(0).toString();
            if (interfaceName !="com.deepin.daemon.InputDevice.Keyboard")
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
    { return "com.deepin.daemon.InputDevice.Keyboard"; }
    static inline const char *staticObjectPath()
    { return "/com/deepin/daemon/InputDevice/Keyboard"; }
    static inline const char *staticServiceName()
    { return "com.deepin.daemon.InputDevices"; }

public:
    explicit DBusKeyboard(QObject *parent = nullptr);

    ~DBusKeyboard();

    Q_PROPERTY(bool CapslockToggle READ capslockToggle WRITE setCapslockToggle NOTIFY CapslockToggleChanged)
    inline bool capslockToggle() const
    { return qvariant_cast< bool >(property("CapslockToggle")); }
    inline void setCapslockToggle(bool value)
    { setProperty("CapslockToggle", QVariant::fromValue(value)); }

    Q_PROPERTY(QString CurrentLayout READ currentLayout WRITE setCurrentLayout NOTIFY CurrentLayoutChanged)
    inline QString currentLayout() const
    { return qvariant_cast< QString >(property("CurrentLayout")); }
    inline void setCurrentLayout(const QString &value)
    { setProperty("CurrentLayout", QVariant::fromValue(value)); }

    Q_PROPERTY(int CursorBlink READ cursorBlink WRITE setCursorBlink NOTIFY CursorBlinkChanged)
    inline int cursorBlink() const
    { return qvariant_cast< int >(property("CursorBlink")); }
    inline void setCursorBlink(int value)
    { setProperty("CursorBlink", QVariant::fromValue(value)); }

    Q_PROPERTY(uint RepeatDelay READ repeatDelay WRITE setRepeatDelay NOTIFY RepeatDelayChanged)
    inline uint repeatDelay() const
    { return qvariant_cast< uint >(property("RepeatDelay")); }
    inline void setRepeatDelay(uint value)
    { setProperty("RepeatDelay", QVariant::fromValue(value)); }

    Q_PROPERTY(bool RepeatEnabled READ repeatEnabled WRITE setRepeatEnabled NOTIFY RepeatEnabledChanged)
    inline bool repeatEnabled() const
    { return qvariant_cast< bool >(property("RepeatEnabled")); }
    inline void setRepeatEnabled(bool value)
    { setProperty("RepeatEnabled", QVariant::fromValue(value)); }

    Q_PROPERTY(uint RepeatInterval READ repeatInterval WRITE setRepeatInterval NOTIFY RepeatIntervalChanged)
    inline uint repeatInterval() const
    { return qvariant_cast< uint >(property("RepeatInterval")); }
    inline void setRepeatInterval(uint value)
    { setProperty("RepeatInterval", QVariant::fromValue(value)); }

    Q_PROPERTY(QStringList UserLayoutList READ userLayoutList NOTIFY UserLayoutListChanged)
    inline QStringList userLayoutList() const
    { return qvariant_cast< QStringList >(property("UserLayoutList")); }

    Q_PROPERTY(QStringList UserOptionList READ userOptionList NOTIFY UserOptionListChanged)
    inline QStringList userOptionList() const
    { return qvariant_cast< QStringList >(property("UserOptionList")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AddLayoutOption(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("AddLayoutOption"), argumentList);
    }

    inline QDBusPendingReply<> AddUserLayout(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("AddUserLayout"), argumentList);
    }

    inline QDBusPendingReply<> ClearLayoutOption()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("ClearLayoutOption"), argumentList);
    }

    inline QDBusPendingReply<> DeleteLayoutOption(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("DeleteLayoutOption"), argumentList);
    }

    inline QDBusPendingReply<> DeleteUserLayout(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("DeleteUserLayout"), argumentList);
    }

    inline QDBusPendingReply<QString> GetLayoutDesc(const QString &in0)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(in0);
        return asyncCallWithArgumentList(QStringLiteral("GetLayoutDesc"), argumentList);
    }

    inline QDBusPendingReply<KeyboardLayoutList> LayoutList()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("LayoutList"), argumentList);
    }

    inline QDBusPendingReply<> Reset()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Reset"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    // begin property changed signals
    void CapslockToggleChanged();
    void CurrentLayoutChanged();
    void CursorBlinkChanged();
    void RepeatDelayChanged();
    void RepeatEnabledChanged();
    void RepeatIntervalChanged();
    void UserLayoutListChanged();
    void UserOptionListChanged();
};

namespace com {
  namespace deepin {
    namespace daemon {
      namespace InputDevice {
        typedef ::DBusKeyboard Keyboard;
      }
    }
  }
}
#endif
