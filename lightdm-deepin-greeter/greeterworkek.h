#ifndef GREETERWORKEK_H
#define GREETERWORKEK_H

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>
#include <QObject>

#include "dbus/dbuslockservice.h"
#include "authinterface.h"
#include "dbus/dbuslogin1manager.h"

class GreeterWorkek : public Auth::AuthInterface {
    Q_OBJECT
public:
    explicit GreeterWorkek(SessionBaseModel *const model, QObject *parent = nullptr);

    void switchToUser(std::shared_ptr<User> user) override;
    void authUser(const QString &password) override;
    void onUserAdded(const QString &user) override;

signals:
    void requestUpdateBackground(const QString& path);

private:
    void checkDBusServer(bool isvalid);
    void onCurrentUserChanged(const QString &user);
    void userAuthForLightdm(std::shared_ptr<User> user);
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void message(QString text, QLightDM::Greeter::MessageType type);
    void authenticationComplete();
    void saveNumlockStatus(std::shared_ptr<User> user, const bool &on);
    void recoveryUserKBState(std::shared_ptr<User> user);

private:
    QLightDM::Greeter *m_greeter;
    DBusLogin1Manager *m_login1ManagerInterface;
    DBusLockService   *m_lockInter;
    bool               m_isThumbAuth;
    bool               m_authenticating;
    QString            m_password;
};

#endif  // GREETERWORKEK_H
