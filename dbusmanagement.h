#ifndef DBUSMANAGEMENT_H
#define DBUSMANAGEMENT_H
#include <QObject>
#include <QtDBus/QtDBus>
#include "dbus/dbussessionmanager.h"

class SessionManageInterfaceManagement:public QObject
{
public:
    SessionManageInterfaceManagement(QObject* parent=0);
    ~SessionManageInterfaceManagement();
    com::deepin::DBusSessionManager* m_sessionManagerInterface;
public slots:
    void RequestLock();
    void RequestShutdown();
    void RequestReboot();
    void RequestSuspend();

    void ForceLogout();
    void ForceShutdown();
    void ForceReboot();

    void Logout();
    void Reboot();
    void Shutdown();
    void PowerOffChoose();
};
#endif // DBUSMANAGEMENT_H

