#include "dbusmanagement.h"

SessionManageInterfaceManagement::SessionManageInterfaceManagement(QObject *parent)
    : QObject(parent)
{
    m_sessionManagerInterface = new com::deepin::DBusSessionManager("com.deepin.SessionManager",
                                                                       "/com/deepin/SessionManager", QDBusConnection::sessionBus(), this);

}
SessionManageInterfaceManagement::~SessionManageInterfaceManagement()
{
}
void SessionManageInterfaceManagement::RequestLock() {
    m_sessionManagerInterface->RequestLock();
}
void SessionManageInterfaceManagement::RequestReboot() {
    m_sessionManagerInterface->Reboot();
}
void SessionManageInterfaceManagement::RequestShutdown() {
    m_sessionManagerInterface->RequestShutdown();
}
void SessionManageInterfaceManagement::RequestSuspend() {
    m_sessionManagerInterface->RequestSuspend();
}
void SessionManageInterfaceManagement::ForceLogout() {
    m_sessionManagerInterface->ForceLogout();
}
void SessionManageInterfaceManagement::ForceShutdown() {
    m_sessionManagerInterface->ForceShutdown();
}
void SessionManageInterfaceManagement::ForceReboot() {
    m_sessionManagerInterface->ForceReboot();
}
void SessionManageInterfaceManagement::Logout() {
    m_sessionManagerInterface->Logout();
}
void SessionManageInterfaceManagement::Shutdown() {
    m_sessionManagerInterface->Shutdown();
}
void SessionManageInterfaceManagement::Reboot() {
    m_sessionManagerInterface->Reboot();
}
void SessionManageInterfaceManagement::PowerOffChoose() {
    m_sessionManagerInterface->PowerOffChoose();
}
