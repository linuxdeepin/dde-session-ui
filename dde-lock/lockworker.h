#ifndef LOCKWORKER_H
#define LOCKWORKER_H

#include <QObject>
#include <QWidget>

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

private:
    void onUserListChanged(const QStringList &list);
    void onUserAdded(const QString &user);
    void onUserRemove(const QString &user);

private:
    SessionBaseModel *m_model;
    AccountsInter *m_accountsInter;
    uint m_currentUserUid;
};

#endif // LOCKWORKER_H
