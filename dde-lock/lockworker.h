#ifndef LOCKWORKER_H
#define LOCKWORKER_H

#include "dbus/dbuslockservice.h"
#include "dbus/dbuslogin1manager.h"
#include "dbus/dbushotzone.h"
#include "userinfo.h"
#include "../libdde-auth/interface/deepinauthinterface.h"
#include "../libdde-auth/deepinauthframework.h"
#include "authinterface.h"

#include <QObject>
#include <QWidget>

#include <com_deepin_daemon_logined.h>
#include <com_deepin_daemon_accounts.h>
#include <com_deepin_sessionmanager.h>

using LoginedInter = com::deepin::daemon::Logined;
using AccountsInter = com::deepin::daemon::Accounts;
using SessionManager = com::deepin::SessionManager;

class SessionBaseModel;
class LockWorker : public Auth::AuthInterface, public DeepinAuthInterface
{
    Q_OBJECT
public:
    explicit LockWorker(SessionBaseModel * const model, QObject *parent = nullptr);

    void switchToUser(std::shared_ptr<User> user) override;
    void authUser(const QString &password) override;

    void enableZoneDetected(bool disable);

    void onDisplayErrorMsg(const QString &msg) override;
    void onDisplayTextInfo(const QString &msg) override;
    void onPasswordResult(const QString &msg) override;

private:
    void onUserAdded(const QString &user) override;

    void saveNumlockStatus(std::shared_ptr<User> user, const bool &on);
    void recoveryUserKBState(std::shared_ptr<User> user);

    // lock
    void lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message);
    void onUnlockFinished(bool unlocked);

    Q_DECL_DEPRECATED bool isDeepin();
private:
    bool m_authenticating;
    bool m_isThumbAuth;
    DBusLockService *m_lockInter;
    DBusLogin1Manager* m_login1ManagerInterface;
    DBusHotzone *m_hotZoneInter;
    DeepinAuthFramework *m_authFramework;
    QString m_password;
    QMap<std::shared_ptr<User>, bool> m_lockUser;
    SessionManager* m_sessionManager;
};

#endif // LOCKWORKER_H
