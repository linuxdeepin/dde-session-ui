#include "lockworker.h"

#include "sessionbasemodel.h"
#include "userinfo.h"
#include "keyboardmonitor.h"
#include "constants.h"
#include "public_func.h"

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <libintl.h>
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QRegularExpression>

#include <com_deepin_daemon_power.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

using PowerInter = com::deepin::daemon::Power;

static std::pair<bool, qint64> checkIsPartitionType(const QStringList &list) {
    std::pair<bool, qint64> result { false, -1 };

    if (list.length() != 5) {
        return result;
    }

    const QString type { list[1] };
    const QString size { list[2] };

    result.first = type == "partition";
    result.second = size.toLongLong() * 1024.0f;

    return result;
}

static qint64 get_power_image_size() {
    qint64 size { 0 };
    QFile file("/sys/power/image_size");

    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        size = file.readAll().trimmed().toLongLong();
        file.close();
    }

    return size;
}

class UserNumlockSettings
{
public:
    UserNumlockSettings(const QString &username)
        : m_username(username)
        , m_settings(QSettings::UserScope, "deepin", "greeter")
    {
    }

    bool get(const bool defaultValue) { return m_settings.value(m_username, defaultValue).toBool(); }
    void set(const bool value) { m_settings.setValue(m_username, value); }

private:
    QString m_username;
    QSettings m_settings;
};

LockWorker::LockWorker(SessionBaseModel * const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_authenticating(false)
    , m_isThumbAuth(false)
    , m_accountsInter(new AccountsInter(ACCOUNT_DBUS_SERVICE, ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this))
    , m_loginedInter(new LoginedInter(ACCOUNT_DBUS_SERVICE, "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
    , m_lockInter(new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this))
    , m_hotZoneInter(new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone", QDBusConnection::sessionBus(), this))
    , m_settings("/usr/share/dde-session-ui/dde-session-ui.conf", QSettings::IniFormat)
{
    m_authFramework = new DeepinAuthFramework(this, this);

    m_login1ManagerInterface =new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    if (!m_login1ManagerInterface->isValid()) {
        qWarning() <<"m_login1ManagerInterface:" << m_login1ManagerInterface->lastError().type();
    }

    m_accountsInter->setSync(true);
    m_loginedInter->setSync(true);

    m_currentUserUid = getuid();

    if (model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        m_greeter = new QLightDM::Greeter(this);

        if (!m_greeter->connectSync())
            qWarning() << "greeter connect fail !!!";

        connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LockWorker::prompt);
        connect(m_greeter, &QLightDM::Greeter::showMessage, this, &LockWorker::message);
        connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LockWorker::authenticationComplete);

        connect(model, &SessionBaseModel::currentUserChanged, this, &LockWorker::recoveryUserKBState);
        connect(KeyboardMonitor::instance(), &KeyboardMonitor::numlockStatusChanged, this, [=] (bool on) {
            saveNumlockStatus(model->currentUser(), on);
        });
    }

    connect(model, &SessionBaseModel::onPowerActionChanged, this, [=] (SessionBaseModel::PowerAction poweraction) {
        if (poweraction == SessionBaseModel::PowerAction::RequireSuspend) {
            m_login1ManagerInterface->Suspend(true);
        }
        if (poweraction == SessionBaseModel::PowerAction::RequireHibernate) {
            m_login1ManagerInterface->Hibernate(true);
        }
        if (model->currentType() == SessionBaseModel::AuthType::LightdmType) {
            switch (poweraction) {
            case SessionBaseModel::PowerAction::RequireShutdown:
                m_login1ManagerInterface->PowerOff(true);
                break;
            case SessionBaseModel::PowerAction::RequireRestart:
                m_login1ManagerInterface->Reboot(true);
            default:
                break;
            }
        }
    });

    connect(model, &SessionBaseModel::visibleChanged, this, [=] (bool isVisible) {
        if (!isVisible || model->currentType() != SessionBaseModel::AuthType::LockType || m_authFramework->isAuthenticate()) return;

        std::shared_ptr<User> user_ptr = model->currentUser();
        if (!user_ptr.get()) return;

        if (user_ptr->type() == User::ADDomain && user_ptr->uid() == 0) return;

        if (!user_ptr->isNoPasswdGrp()) {
            if (isDeepin()) {
                m_authFramework->SetUser(user_ptr->name());
                m_authFramework->Authenticate();
            }
            else {
                m_lockInter->AuthenticateUser(user_ptr->name());
            }
        }
    });

    connect(m_lockInter, &DBusLockService::UserChanged, this, &LockWorker::onCurrentUserChanged);
    connect(m_lockInter, &DBusLockService::Event, this, &LockWorker::lockServiceEvent);
    connect(model, &SessionBaseModel::onStatusChanged, this, [=] (SessionBaseModel::ModeStatus status) {
        if (status == SessionBaseModel::ModeStatus::PowerMode) {
            checkPowerInfo();
        }
    });

    connect(m_loginedInter, &LoginedInter::LastLogoutUserChanged, this, &LockWorker::onLastLogoutUserChanged);
    connect(m_loginedInter, &LoginedInter::UserListChanged, this, &LockWorker::onLoginUserListChanged);

    if (valueByQSettings<bool>("", "loginPromptAvatar", true)) {
        connect(m_accountsInter, &AccountsInter::UserListChanged, this, &LockWorker::onUserListChanged);
        connect(m_accountsInter, &AccountsInter::UserAdded, this, &LockWorker::onUserAdded);
        connect(m_accountsInter, &AccountsInter::UserDeleted, this, &LockWorker::onUserRemove);
        connect(m_accountsInter, &AccountsInter::serviceValidChanged, this, &LockWorker::checkDBusServer);

        checkDBusServer(m_accountsInter->isValid());
        onCurrentUserChanged(m_lockInter->CurrentUser());
    }

    onLoginUserListChanged(m_loginedInter->userList());
    onLastLogoutUserChanged(m_loginedInter->lastLogoutUser());

    checkPowerInfo();
    checkVirtualKB();

    const QString &switchUserButtonValue = showSwitchUserButtonValue();
    m_model->setAlwaysShowUserSwitchButton(switchUserButtonValue == "always");
    m_model->setAllowShowUserSwitchButton(switchUserButtonValue == "ondemand");

    // init ADDomain User
    if (valueByQSettings<bool>("", "loginPromptInput", false)) {
        std::shared_ptr<User> user = std::make_shared<ADDomainUser>(0);
        static_cast<ADDomainUser*>(user.get())->setUserDisplayName(tr("Domain account"));
        m_model->userAdd(user);
        if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
            m_model->setCurrentUser(user);
        }
    }
}

void LockWorker::switchToUser(std::shared_ptr<User> user)
{
    qDebug() << "switch user from" << m_model->currentUser()->name() << " to " << user->name();

    // clear old password
    m_password.clear();

    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        // just switch user
        if (user->isLogin()) {
            // switch to user Xorg
            QProcess::startDetached("dde-switchtogreeter", QStringList() << user->name());
        }
        else {
            m_model->setCurrentUser(user);

            // Skip LDAP login inputbox
            if (user->type() == User::ADDomain && user->uid() == 0) {
                return;
            }

            if (isDeepin()) {
                // reset fprintd
                m_greeter->cancelAuthentication();
                m_authFramework->Clear();

                m_authFramework->SetUser(user->name());

                if (!user->isNoPasswdGrp()) {
                    m_greeter->authenticate(user->name());
                    m_authFramework->Authenticate();
                }
            }
            else {
                userAuthForLightdm(user);
            }
        }
        return;
    }

    // if type is lock, switch to greeter
    QJsonObject json;
    json["Uid"] = static_cast<int>(user->uid());
    json["Type"] = user->type();

    m_lockInter->SwitchToUser(QString(QJsonDocument(json).toJson(QJsonDocument::Compact))).waitForFinished();

    if (user->isLogin()) {
        QProcess::startDetached("dde-switchtogreeter", QStringList() << user->name());
    } else {
        QProcess::startDetached("dde-switchtogreeter");
    }
}

void LockWorker::authUser(const QString &password)
{
    if (m_authenticating) return;

    m_authenticating = true;

    // auth interface
    std::shared_ptr<User> user = m_model->currentUser();

    m_password = password;

    if (isDeepin()) {
        if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
            if (m_greeter->authenticationUser() != user->name()) {
                m_greeter->cancelAuthentication();
                QTimer::singleShot(100, this, [=] {
                    // greeter reauth need time!
                    m_greeter->authenticate(user->name());
                });
                m_authFramework->Clear();
                m_authFramework->SetUser(user->name());
                m_authFramework->setPassword(password);
                m_authFramework->Authenticate();
                return;
            }
        }

        m_authFramework->Clear();
        m_authFramework->SetUser(user->name());
        m_authFramework->setPassword(password);
        m_authFramework->Authenticate();
        return;
    }

    if (m_model->currentType() == SessionBaseModel::AuthType::LockType) {
        qDebug() << "start authentication of user: " << user->name();

        m_lockInter->AuthenticateUser(user->name());
        m_lockInter->UnlockCheck(user->name(), password);
    }
    else {
        qDebug() << "start authentication of user: " << user->name();

        if (m_greeter->authenticationUser() != user->name()) {
            m_greeter->cancelAuthentication();
            QTimer::singleShot(100, this, [=] {
                m_greeter->authenticate(user->name());
            });
        }
        else {
            if (m_greeter->inAuthentication()) {
                m_greeter->respond(password);
            }
            else {
                m_greeter->authenticate(user->name());
            }
        }

    }
}

/* NOTE(lxz): Why do this,
   because the meaning of worker is to do all the setup work,
   and should not be handled by the interface.
*/
void LockWorker::setLayout(std::shared_ptr<User> user, const QString &layout)
{
    user->setCurrentLayout(layout);
}

void LockWorker::enableZoneDetected(bool disable)
{
    m_hotZoneInter->EnableZoneDetected(disable);
}

void LockWorker::onDisplayErrorMsg(const QString &msg)
{
    emit m_model->authFaildTipsMessage(msg);
}

void LockWorker::onDisplayTextInfo(const QString &msg)
{
    emit m_model->authFaildMessage(msg);
}

void LockWorker::onPasswordResult(const QString &msg)
{
    m_password = msg;
    std::shared_ptr<User> user = m_model->currentUser();

    switch (m_model->currentType()) {
        case SessionBaseModel::AuthType::LockType: {
            if (msg.isEmpty()) {
                //FIXME(lxz): 不知道为什么收不到错误
                onUnlockFinished(false);
            }
            else {
                m_lockInter->AuthenticateUser(user->name());
                m_lockInter->UnlockCheck(user->name(), m_password);
            }
        } break;
        case SessionBaseModel::AuthType::LightdmType: {
            if (m_greeter->inAuthentication()) {
                m_greeter->respond(m_password);
            }
            else {
                if (m_greeter->inAuthentication()) {
                    m_greeter->cancelAuthentication();
                }

                m_greeter->authenticate(user->name());
                m_greeter->respond(m_password);
            }
        } break;
        default: break;
    }
}

void LockWorker::checkDBusServer(bool isvalid)
{
    if (isvalid) {
        onUserListChanged(m_accountsInter->userList());
    }
    else {
        // FIXME: 我不希望这样做，但是QThread::msleep会导致无限递归
        QTimer::singleShot(300, this, [=] {
            qWarning() << "com.deepin.daemon.Accounts is not start, rechecking!";
            checkDBusServer(m_accountsInter->isValid());
        });
    }
}

void LockWorker::onUserListChanged(const QStringList &list)
{
    QStringList tmpList;
    for (std::shared_ptr<User> u : m_model->userList()) {
        tmpList << QString("/com/deepin/daemon/Accounts/User%1").arg(u->uid());
    }

    for (const QString &u : list) {
        if (!tmpList.contains(u)) {
            tmpList << u;
            onUserAdded(u);
        }
    }

    for (const QString &u : tmpList) {
        if (!list.contains(u)) {
            onUserRemove(u);
        }
    }
}

void LockWorker::onUserAdded(const QString &user)
{
    std::shared_ptr<User> user_ptr(new NativeUser(user));

    user_ptr->setisLogind(isLogined(user_ptr->uid()));

    if (m_model->currentType() == SessionBaseModel::AuthType::LockType) {
        if (user_ptr->uid() == m_currentUserUid) {
            m_model->setCurrentUser(user_ptr);
            if (!user_ptr->isNoPasswdGrp()) {
                if (isDeepin()) {
                    m_authFramework->SetUser(user_ptr->name());
                    m_authFramework->Authenticate();
                }
                else {
                    m_lockInter->AuthenticateUser(user_ptr->name());
                }
            }
        }
    }

    if (m_model->currentUser().get() == nullptr) {
        if (m_model->userList().isEmpty() || m_model->userList().first()->type() == User::ADDomain) {
            m_model->setCurrentUser(user_ptr);

            if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
                if (isDeepin()) {
                    m_authFramework->SetUser(user_ptr->name());

                    if (!user_ptr->isNoPasswdGrp()) {
                        m_authFramework->Authenticate();
                    }
                }
                else {
                    userAuthForLightdm(user_ptr);
                }
            }
        }
    }

    if (user_ptr->uid() == m_lastLogoutUid) {
        m_model->setLastLogoutUser(user_ptr);
    }

    m_model->userAdd(user_ptr);
}

void LockWorker::onUserRemove(const QString &user)
{
    QList<std::shared_ptr<User>> list = m_model->userList();

    for (auto u : list) {
        if (u->path() == user && u->type() == User::Native) {
            m_model->userRemoved(u);
            break;
        }
    }
}

void LockWorker::onLastLogoutUserChanged(uint uid)
{
    m_lastLogoutUid = uid;

    QList<std::shared_ptr<User>> userList = m_model->userList();
    for (auto it = userList.constBegin(); it != userList.constEnd(); ++it) {
        if ((*it)->uid() == uid) {
            m_model->setLastLogoutUser((*it));
            return;
        }
    }

    m_model->setLastLogoutUser(std::shared_ptr<User>(nullptr));
}

void LockWorker::onLoginUserListChanged(const QString &list)
{
    m_loginUserList.clear();

    std::list<uint> availableUidList;
    for (std::shared_ptr<User> user : m_model->userList()) {
        availableUidList.push_back(user->uid());
    }

    QJsonObject userList = QJsonDocument::fromJson(list.toUtf8()).object();
    for (auto it = userList.constBegin(); it != userList.constEnd(); ++it){
        const bool haveDisplay = checkHaveDisplay(it.value().toArray());
        const uint uid = it.key().toUInt();

        // skip not have display users
        if (haveDisplay) {
            m_loginUserList.push_back(uid);
        }

        auto find_it = std::find_if(availableUidList.begin(), availableUidList.end(), [=] (const uint find_addomain_uid) {
            return find_addomain_uid == uid;
        });

        if (haveDisplay && find_it == availableUidList.end()) {
            // init addoman user
            std::shared_ptr<User> u(new ADDomainUser(uid));
            u->setisLogind(true);

            struct passwd *pws;
            pws = getpwuid(uid);

            static_cast<ADDomainUser*>(u.get())->setUserDisplayName(pws->pw_name);
            static_cast<ADDomainUser*>(u.get())->setUserName(pws->pw_name);

            if (uid == m_currentUserUid && m_model->currentUser().get() == nullptr) {
                m_model->setCurrentUser(u);
            }
            m_model->userAdd(u);
            availableUidList.push_back(uid);
        }
    }

    QList<std::shared_ptr<User>> uList = m_model->userList();
    for (auto it = uList.begin(); it != uList.end();) {
        std::shared_ptr<User> user = *it;

        auto find_it = std::find_if(m_loginUserList.begin(), m_loginUserList.end(), [=] (const uint find_uid) {
            return find_uid == user->uid();
        });

        if (find_it == m_loginUserList.end() && (user->type() == User::ADDomain && user->uid() != 0)) {
            m_model->userRemoved(user);
            it = uList.erase(it);
        }
        else {
            user->setisLogind(isLogined(user->uid()));
            ++it;
        }
    }
}

bool LockWorker::checkHaveDisplay(const QJsonArray &array)
{
    for (auto it = array.constBegin(); it != array.constEnd(); ++it){
        const QJsonObject &obj = (*it).toObject();

        // If user without desktop or display, this is system service, need skip.
        if (!obj["Display"].toString().isEmpty() && !obj["Desktop"].toString().isEmpty()) {
            return true;
        }
    }

    return false;
}

bool LockWorker::isLogined(uint uid)
{
    auto isLogind = std::find_if(m_loginUserList.begin(), m_loginUserList.end(), [=] (uint UID) {
        return uid == UID;
    });

    return isLogind != m_loginUserList.end();
}

void LockWorker::onCurrentUserChanged(const QString &user)
{
    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        const QJsonObject obj = QJsonDocument::fromJson(user.toUtf8()).object();
        m_currentUserUid = obj["Uid"].toInt();

        for (std::shared_ptr<User> user : m_model->userList()) {
            if (user->uid() == m_currentUserUid) {
                m_model->setCurrentUser(user);
                userAuthForLightdm(user);
                break;
            }
        }
    }
}

void LockWorker::saveNumlockStatus(std::shared_ptr<User> user, const bool &on) {
    UserNumlockSettings(user->name()).set(on);
}

void LockWorker::recoveryUserKBState(std::shared_ptr<User> user)
{
    //FIXME(lxz)
//    PowerInter powerInter("com.deepin.system.Power", "/com/deepin/system/Power", QDBusConnection::systemBus(), this);
//    const BatteryPresentInfo info = powerInter.batteryIsPresent();
//    const bool defaultValue = !info.values().first();
    const bool enabled = UserNumlockSettings(user->name()).get(false);

    qDebug() << "restore numlock status to " << enabled;
    KeyboardMonitor::instance()->setNumlockStatus(enabled);
}

void LockWorker::lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message)
{
    if (!m_model->currentUser()) return;

    if (username != m_model->currentUser()->name())
        return;

    qDebug() << eventType << pid << username << message;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = message.simplified() == "Password:" ? "" : message;

    m_authenticating = false;

    if (msg == "Verification timed out") {
        m_isThumbAuth = true;
        emit m_model->authFaildMessage(tr("Fingerprint verification timed out, please enter your password manually"));
        return;
    }

    switch (eventType) {
    case DBusLockService::PromptQuestion:
        qDebug() << "prompt quesiton from pam: " << message;
        emit m_model->authFaildMessage(message);
        break;
    case DBusLockService::PromptSecret:
        qDebug() << "prompt secret from pam: " << message;
        if (m_isThumbAuth && !msg.isEmpty()) {
            emit m_model->authFaildMessage(msg);
        }
        break;
    case DBusLockService::ErrorMsg:
        qWarning() << "error message from pam: " << message;
        if (msg == "Failed to match fingerprint") {
            emit m_model->authFaildTipsMessage(tr("Failed to match fingerprint"));
            emit m_model->authFaildMessage("");
        }
        break;
    case DBusLockService::TextInfo:
        emit m_model->authFaildMessage(QString(dgettext("fprintd", message.toLatin1())));
        break;
    case DBusLockService::Failure:
        onUnlockFinished(false);
        break;
    case DBusLockService::Success:
        onUnlockFinished(true);
        break;
    default:
        break;
    }
}

void LockWorker::onUnlockFinished(bool unlocked)
{
    // only unlock succeed will close fprint device
    if (unlocked && isDeepin()) {
        m_authFramework->Clear();
    }

    emit m_model->authFinished(unlocked);

    m_authenticating = false;

    if (!unlocked) {
        qDebug() << "Authorization failed!";
        emit m_model->authFaildTipsMessage(tr("Wrong Password"));

        if (m_model->currentUser()->isLockForNum()) {
            m_model->currentUser()->startLock();
        }
        return;
    }

    switch (m_model->powerAction()) {
    case SessionBaseModel::PowerAction::RequireRestart:
        m_login1ManagerInterface->Reboot(true);
        return;
    case SessionBaseModel::PowerAction::RequireShutdown:
        m_login1ManagerInterface->PowerOff(true);
        return;
    default:
        break;
    }
}

void LockWorker::userAuthForLightdm(std::shared_ptr<User> user)
{
    if (!user->isNoPasswdGrp()) {
        if (m_greeter->inAuthentication()) {
            m_greeter->cancelAuthentication();
        }
        QTimer::singleShot(100, this, [=] {
            m_greeter->authenticate(user->name());
            m_greeter->respond(m_password);
        });
    }
}

void LockWorker::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "pam prompt: " << text << type;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = text.simplified() == "Password:" ? "" : text;

    switch (type) {
    case QLightDM::Greeter::PromptTypeSecret:
        if (m_isThumbAuth || m_password.isEmpty())
            break;

        if (msg.isEmpty()) {
            m_greeter->respond(m_password);
        }
        else {
            emit m_model->authFaildMessage(msg);
        }
        break;
    case QLightDM::Greeter::PromptTypeQuestion:
        // trim the right : in the message if exists.
        emit m_model->authFaildMessage(text.replace(":", ""));
        break;
    default:
        break;
    }
}

// TODO(justforlxz): 错误信息应该存放在User类中, 切换用户后其他控件读取错误信息，而不是在这里分发。
void LockWorker::message(QString text, QLightDM::Greeter::MessageType type)
{
    qDebug() << "pam message: " << text << type;

    if (text == "Verification timed out") {
        m_isThumbAuth = true;
        emit m_model->authFaildMessage(tr("Fingerprint verification timed out, please enter your password manually"));
        return;
    }

    switch (type) {
    case QLightDM::Greeter::MessageTypeInfo:
        if (m_isThumbAuth)
            break;

        emit m_model->authFaildMessage(QString(dgettext("fprintd", text.toLatin1())));
        break;
    case QLightDM::Greeter::MessageTypeError:
        qWarning() << "error message from lightdm: " << text;
        if (text == "Failed to match fingerprint") {
            emit m_model->authFaildMessage("");
            emit m_model->authFaildTipsMessage(tr("Failed to match fingerprint"));
        }
        break;
    default:
        break;
    }
}

void LockWorker::authenticationComplete()
{
    qDebug() << "authentication complete, authenticated " << m_greeter->isAuthenticated();

    m_authenticating = false;

    if (isDeepin()) {
        m_authFramework->Clear();
    }

    if (!m_greeter->isAuthenticated()) {
        if (m_password.isEmpty()) {
            return;
        }

        if (m_model->currentUser()->type() == User::Native) {
            emit m_model->authFaildTipsMessage(tr("Wrong Password"));
        }

        if (m_model->currentUser()->type() == User::ADDomain) {
            emit m_model->authFaildTipsMessage(tr("The domain account or password is not correct. Please enter again."));
        }

        if (m_model->currentUser()->isLockForNum()) {
            m_model->currentUser()->startLock();
        }

        return;
    }

    switch (m_model->powerAction()) {
    case SessionBaseModel::PowerAction::RequireRestart:
        m_login1ManagerInterface->Reboot(true);
        return;
    case SessionBaseModel::PowerAction::RequireShutdown:
        m_login1ManagerInterface->PowerOff(true);
        return;
    default:
        break;
    }

    qDebug() << "start session = " << m_model->sessionKey();

    auto startSessionSync = [=]() {
        QJsonObject json;
        json["Uid"] = static_cast<int>(m_model->currentUser()->uid());
        json["Type"] = m_model->currentUser()->type();
        m_lockInter->SwitchToUser(QString(QJsonDocument(json).toJson(QJsonDocument::Compact))).waitForFinished();

        m_greeter->startSessionSync(m_model->sessionKey());
    };

#ifndef DISABLE_LOGIN_ANI
    // NOTE(kirigaya): It is not necessary to display the login animation.
    emit requestUpdateBackground(m_model->currentUser()->desktopBackgroundPath());
    emit m_model->authFinished(true);
    QTimer::singleShot(1000, this, startSessionSync);
#else
    startSessionSync();
#endif
}

void LockWorker::checkPowerInfo()
{
    m_model->setCanSleep(valueByQSettings<bool>("POWER", "sleep", true));

    if (valueByQSettings<bool>("POWER", "hibernate", false)) {
        checkSwap();
    }
}

void LockWorker::checkVirtualKB()
{
    m_model->setHasVirtualKB(QProcess::execute("which", QStringList() << "onboard") == 0);
}

void LockWorker::checkSwap()
{
    QFile file("/proc/swaps");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        bool hasSwap { false };
        const QString &body = file.readAll();
        QTextStream stream(body.toUtf8());
        while(!stream.atEnd()) {
            const std::pair<bool, qint64> result = checkIsPartitionType(stream.readLine().
                                                                     simplified().
                                                                     split(" ",
                                                                           QString::SplitBehavior::SkipEmptyParts));
            qint64 image_size { get_power_image_size() };

            if (result.first) {
                hasSwap = image_size < result.second;
            }

            qDebug() << "check using swap partition!";
            qDebug() << QString("image_size: %1, swap partition size: %2").
                        arg(QString::number(image_size)).
                        arg(QString::number(result.second));

            if (hasSwap) {
                break;
            }
        }

        m_model->setHasSwap(hasSwap);
        file.close();
    }
    else {
        qWarning() << "open /proc/swaps failed! please check permission!!!";
    }
}

const QString LockWorker::showSwitchUserButtonValue()
{
    return valueByQSettings<QString>("Lock", "showSwitchUserButton", "ondemand");
}

bool LockWorker::isDeepin()
{
    // 这是临时的选项，只在Deepin下启用同步认证功能，其他发行版下禁用。
#ifdef QT_DEBUG
    return true;
#else
    // Disable for greeter
    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        return false;
    }
    return valueByQSettings<bool>("OS", "isDeepin", false);
#endif
}

template<typename T>
T LockWorker::valueByQSettings(const QString &group, const QString &key, const QVariant &failback)
{
    return findValueByQSettings<T>(DDESESSIONCC::session_ui_configs, group, key, failback);
}
