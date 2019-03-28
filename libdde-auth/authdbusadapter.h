#ifndef AUTHDBUSADAPTER_H
#define AUTHDBUSADAPTER_H

#include "authagent.h"

#include <QDBusAbstractAdaptor>

class AuthDBusAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.daemon.Authority.Agent")

public:
    explicit AuthDBusAdaptor(QObject *parent)
        : QDBusAbstractAdaptor(parent) {}

public Q_SLOTS:
    const QString RequestEchoOff(const QString &msg);
    const QString RequestEchoOn(const QString &msg);
    void DisplayErrorMsg(const QString &msg);
    void DisplayTextInfo(const QString &msg);
    void RespondResult(const QString &msg);

private:
    AuthAgent *parent() { return qobject_cast<AuthAgent*>(QObject::parent()); }
};

#endif // AUTHDBUSADAPTER_H
