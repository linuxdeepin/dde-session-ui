#ifndef AUTHINTERFACE_H
#define AUTHINTERFACE_H

#include "public_func.h"
#include "constants.h"

#include <com_deepin_daemon_accounts.h>
#include <com_deepin_daemon_logined.h>

#include <QJsonArray>
#include <QObject>
#include <memory>

using AccountsInter = com::deepin::daemon::Accounts;
using LoginedInter = com::deepin::daemon::Logined;

class User;
class SessionBaseModel;

namespace Auth {
class AuthInterface : public QObject {
    Q_OBJECT
public:
    explicit AuthInterface(SessionBaseModel *const model, QObject *parent = nullptr);

    virtual void switchToUser(std::shared_ptr<User> user) = 0;
    virtual void authUser(const QString &password)        = 0;

    virtual void setLayout(std::shared_ptr<User> user, const QString &layout);
    virtual void onUserListChanged(const QStringList &list);
    virtual void onUserAdded(const QString &user);
    virtual void onUserRemove(const QString &user);

protected:
    void initDBus();
    void initData();
    void onLastLogoutUserChanged(uint uid);
    void onLoginUserListChanged(const QString &list);

    bool checkHaveDisplay(const QJsonArray &array);
    bool isLogined(uint uid);
    void checkPowerInfo();
    void checkVirtualKB();
    void checkSwap();

    template <typename T>
    T valueByQSettings(const QString & group,
                       const QString & key,
                       const QVariant &failback) {
        return findValueByQSettings<T>(DDESESSIONCC::session_ui_configs,
                                       group,
                                       key,
                                       failback);
    }

protected:
    SessionBaseModel* m_model;
    AccountsInter *   m_accountsInter;
    LoginedInter*     m_loginedInter;
    uint              m_lastLogoutUid;
    uint              m_currentUserUid;
    std::list<uint>   m_loginUserList;
};
}  // namespace Auth

#endif  // AUTHINTERFACE_H
