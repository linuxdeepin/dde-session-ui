#include "lockworker.h"

#include "sessionbasemodel.h"
#include "userinfo.h"

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
using namespace Auth;

LockWorker::LockWorker(SessionBaseModel * const model, QObject *parent)
    : AuthInterface(model, parent)
    , m_authenticating(false)
    , m_isThumbAuth(false)
    , m_lockInter(new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this))
    , m_hotZoneInter(new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone", QDBusConnection::sessionBus(), this))
    , m_sessionManager(new SessionManager("com.deepin.SessionManager", "/com/deepin/SessionManager", QDBusConnection::sessionBus(), this))
{
    m_authFramework = new DeepinAuthFramework(this, this);

    m_login1ManagerInterface =new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    if (!m_login1ManagerInterface->isValid()) {
        qWarning() <<"m_login1ManagerInterface:" << m_login1ManagerInterface->lastError().type();
    }

    m_currentUserUid = getuid();

    connect(model, &SessionBaseModel::onPowerActionChanged, this, [=] (SessionBaseModel::PowerAction poweraction) {
        switch (poweraction) {
            case SessionBaseModel::PowerAction::RequireSuspend:
                m_login1ManagerInterface->Suspend(true);
                break;
            case SessionBaseModel::PowerAction::RequireHibernate:
                m_login1ManagerInterface->Hibernate(true);
                break;
            default:
                break;
        }
    });

    connect(model, &SessionBaseModel::visibleChanged, this, [=] (bool isVisible) {
        if (!isVisible || model->currentType() != SessionBaseModel::AuthType::LockType || m_authFramework->isAuthenticate()) return;

        std::shared_ptr<User> user_ptr = model->currentUser();
        if (!user_ptr.get()) return;

        if (user_ptr->type() == User::ADDomain && user_ptr->uid() == 0) return;

        if (!user_ptr->isNoPasswdGrp()) {
            if (isDeepin()) {
                m_authFramework->SetUser(user_ptr);
                m_authFramework->Authenticate();
            }
            else {
                m_lockInter->AuthenticateUser(user_ptr->name());
            }
        }
    });

    connect(m_lockInter, &DBusLockService::Event, this, &LockWorker::lockServiceEvent);
    connect(model, &SessionBaseModel::onStatusChanged, this, [=] (SessionBaseModel::ModeStatus status) {
        if (status == SessionBaseModel::ModeStatus::PowerMode) {
            checkPowerInfo();
        }
    });

    connect(m_loginedInter, &LoginedInter::LastLogoutUserChanged, this, &LockWorker::onLastLogoutUserChanged);
    connect(m_loginedInter, &LoginedInter::UserListChanged, this, &LockWorker::onLoginUserListChanged);

    connect(m_sessionManager, &SessionManager::Unlock, this, [=] {
        m_authenticating = false;
        m_password.clear();
        emit m_model->authFinished(true);
    });

    const QString &switchUserButtonValue { valueByQSettings<QString>("Lock", "showSwitchUserButton", "ondemand") };
    m_model->setAlwaysShowUserSwitchButton(switchUserButtonValue == "always");
    m_model->setAllowShowUserSwitchButton(switchUserButtonValue == "ondemand");

    if (valueByQSettings<bool>("", "loginPromptAvatar", true)) {
        initDBus();
        initData();
    }

    // init ADDomain User
    if (valueByQSettings<bool>("", "loginPromptInput", false)) {
        std::shared_ptr<User> user = std::make_shared<ADDomainUser>(0);
        static_cast<ADDomainUser*>(user.get())->setUserDisplayName(tr("Domain account"));
        m_model->userAdd(user);
    }
}

void LockWorker::switchToUser(std::shared_ptr<User> user)
{
    qDebug() << "switch user from" << m_model->currentUser()->name() << " to " << user->name();

    // clear old password
    m_password.clear();

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

    qDebug() << "start authentication of user: " << user->name();

    if (isDeepin()) {
        m_authFramework->Clear();
        m_authFramework->SetUser(user);
        m_authFramework->setPassword(password);
        m_authFramework->Authenticate();
        return;
    }

    m_lockInter->AuthenticateUser(user->name());
    m_lockInter->UnlockCheck(user->name(), password);
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

    if (msg.isEmpty()) {
        //FIXME(lxz): 不知道为什么收不到错误
        onUnlockFinished(false);
    }
    else {
        m_lockInter->AuthenticateUser(user->name());
        m_lockInter->UnlockCheck(user->name(), m_password);
    }
}

void LockWorker::onUserAdded(const QString &user)
{
    std::shared_ptr<User> user_ptr(new NativeUser(user));

    user_ptr->setisLogind(isLogined(user_ptr->uid()));

    if (user_ptr->uid() == m_currentUserUid) {
        m_model->setCurrentUser(user_ptr);
        if (!user_ptr->isNoPasswdGrp()) {
            if (isDeepin()) {
                m_authFramework->SetUser(user_ptr);
                m_authFramework->Authenticate();
            }
            else {
                m_lockInter->AuthenticateUser(user_ptr->name());
            }
        }
    }

    if (user_ptr->uid() == m_lastLogoutUid) {
        m_model->setLastLogoutUser(user_ptr);
    }

    m_model->userAdd(user_ptr);
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

    if (unlocked) {
        m_model->currentUser()->resetLock();
    }

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

bool LockWorker::isDeepin()
{
    // 这是临时的选项，只在Deepin下启用同步认证功能，其他发行版下禁用。
#ifdef QT_DEBUG
    return true;
#else
    return valueByQSettings<bool>("OS", "isDeepin", false);
#endif
}
