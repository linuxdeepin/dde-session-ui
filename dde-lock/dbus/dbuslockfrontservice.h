#ifndef DBUSLOCKFRONTSERVICE_H
#define DBUSLOCKFRONTSERVICE_H

#include "lockframe.h"
#include <QDBusAbstractAdaptor>
#include <QtCore/QObject>
#include <QtDBus/QtDBus>

QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Proxy class for interface com.deepin.dde.lockFront
 */
class DBusLockFrontService: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.lockFront")
    Q_CLASSINFO("D-Bus Introspection", ""
                                       "  <interface name=\"com.deepin.dde.lockFront\">\n"
                                       "    <method name=\"Show\"/>\n"
                                       "  </interface>\n"
                                       "")
public:
    explicit DBusLockFrontService(LockFrame *parent);
    virtual ~DBusLockFrontService();

    inline LockFrame *parent() const
    { return static_cast<LockFrame *>(QObject::parent()); }

public:// PROPERTIES
public Q_SLOTS:// METHODS
    void Show();
    void ShowUserList();
};

#endif // DBUSLOCKFRONTSERVICE_H
