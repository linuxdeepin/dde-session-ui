#ifndef LOCKWORKER_H
#define LOCKWORKER_H

#include "dbus/dbuslockservice.h"

#include <QObject>
#include <QWidget>

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include <com_deepin_daemon_logined.h>
#include <com_deepin_daemon_accounts.h>

using LoginedInter = com::deepin::daemon::Logined;
using AccountsInter = com::deepin::daemon::Accounts;

class SessionBaseModel;
class User;
class LockWorker : public QObject
{
    Q_OBJECT
public:
    explicit LockWorker(SessionBaseModel * const model, QObject *parent = nullptr);

    void switchToUser(std::shared_ptr<User> user);

    void authUser(std::shared_ptr<User> user, const QString &password);

private:
    void onUserListChanged(const QStringList &list);
    void onUserAdded(const QString &user);
    void onUserRemove(const QString &user);
    void onLastLogoutUserChanged(uint uid);
    void onLoginUserListChanged(const QString &list);
    bool checkHaveDisplay(const QJsonArray &array);
    bool isLogined(uint uid);
    bool checkUserIsNoPWGrp(std::shared_ptr<User> user);

    // lock
    void lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message);
    void onUnlockFinished(bool unlocked);
    // greeter
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void message(QString text, QLightDM::Greeter::MessageType type);
    void authenticationComplete();

private:
    SessionBaseModel *m_model;
    AccountsInter *m_accountsInter;
    LoginedInter *m_loginedInter;

    // lock
    bool m_authenticating;
    bool m_isThumbAuth;
    DBusLockService *m_lockInter;

    // greeter
    QLightDM::Greeter *m_greeter;

    uint m_currentUserUid;
    uint m_lastLogoutUid;
    std::shared_ptr<User> m_authUser;
    std::list<uint> m_loginUserList;
    QString m_password;
};

#endif // LOCKWORKER_H
