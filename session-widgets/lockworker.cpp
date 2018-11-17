#include "lockworker.h"

#include "sessionbasemodel.h"
#include "userinfo.h"

#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <libintl.h>
#include <QDebug>

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

LockWorker::LockWorker(SessionBaseModel * const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_authenticating(false)
    , m_isThumbAuth(false)
    , m_accountsInter(new AccountsInter(ACCOUNT_DBUS_SERVICE, ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this))
    , m_loginedInter(new LoginedInter(ACCOUNT_DBUS_SERVICE, "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
{
    m_accountsInter->setSync(false);
    m_loginedInter->setSync(true);

    m_currentUserUid = getuid();

    if (model->currentType() == SessionBaseModel::AuthType::LockType) {
        m_lockInter = new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);
        connect(m_lockInter, &DBusLockService::Event, this, &LockWorker::lockServiceEvent);
    }
    else {
        m_greeter = new QLightDM::Greeter(this);

        if (!m_greeter->connectSync())
            qWarning() << "greeter connect fail !!!";

        connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LockWorker::prompt);
        connect(m_greeter, &QLightDM::Greeter::showMessage, this, &LockWorker::message);
        connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LockWorker::authenticationComplete);
    }

    connect(m_loginedInter, &LoginedInter::UserListChanged, this, &LockWorker::onLoginUserListChanged);
    connect(m_loginedInter, &LoginedInter::LastLogoutUserChanged, this, &LockWorker::onLastLogoutUserChanged);
    connect(m_accountsInter, &AccountsInter::UserListChanged, this, &LockWorker::onUserListChanged);
    connect(m_accountsInter, &AccountsInter::UserAdded, this, &LockWorker::onUserAdded);
    connect(m_accountsInter, &AccountsInter::UserDeleted, this, &LockWorker::onUserRemove);

    onLastLogoutUserChanged(m_loginedInter->lastLogoutUser());
    onLoginUserListChanged(m_loginedInter->userList());
    onUserListChanged(m_accountsInter->userList());
}

void LockWorker::switchToUser(std::shared_ptr<User> user)
{
    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        // just switch user
        if (user->isLogin()) {
            // switch to user Xorg
            QProcess::startDetached("dde-switchtogreeter", QStringList() << user->name());
        }
        else {
            m_model->setCurrentUser(user);
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

void LockWorker::authUser(std::shared_ptr<User> user, const QString &password)
{
    // auth interface
    m_authUser = user;

    m_password = password;

    m_authenticating = true;

    if (m_model->currentType() == SessionBaseModel::AuthType::LockType) {
        m_lockInter->AuthenticateUser(user->name());
        m_lockInter->UnlockCheck(user->name(), password);
    }
    else {
        if (m_greeter->inAuthentication()) {
            qDebug() << "start authentication of user: " << m_greeter->authenticationUser();
            m_greeter->respond(password);
        }
        else {
            m_greeter->authenticate(user->name());
        }
    }
}

void LockWorker::onUserListChanged(const QStringList &list)
{
    QStringList tmpList;
    for (std::shared_ptr<User> u : m_model->userList()) {
        tmpList << QString::number(u->uid());
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

    if (user_ptr->uid() == m_currentUserUid) {
        m_model->setCurrentUser(user_ptr);
    }

    if (m_model->currentUser().get() == nullptr && !m_model->userList().isEmpty()) {
        m_model->setCurrentUser(m_model->userList().first());
    }

    if (user_ptr->uid() == m_lastLogoutUid) {
        m_model->setLastLogoutUser(user_ptr);
    }

    user_ptr->setNoPasswdGrp(checkUserIsNoPWGrp(user_ptr));

    m_model->userAdd(user_ptr);
}

void LockWorker::onUserRemove(const QString &user)
{
    QList<std::shared_ptr<User>> list = m_model->userList();

    for (auto u : list) {
        if (u->uid() == user.toUInt()) {
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

    QJsonObject userList = QJsonDocument::fromJson(list.toUtf8()).object();
    for (auto it = userList.constBegin(); it != userList.constEnd(); ++it){
        const bool haveDisplay = checkHaveDisplay(it.value().toArray());
        const uint uid = it.key().toUInt();

        // skip not have display users
        if (haveDisplay) {
            m_loginUserList.push_back(uid);
        }
    }

    QList<std::shared_ptr<User>> uList = m_model->userList();
    for (auto it = uList.constBegin(); it != uList.constEnd(); ++it) {
        (*it)->setisLogind(isLogined((*it)->uid()));
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

bool LockWorker::checkUserIsNoPWGrp(std::shared_ptr<User> user)
{
    if (user->type() == User::ADDomain) {
        return false;
    }

    // Caution: 32 here is unreliable, and there may be more
    // than this number of groups that the user joins.

    int ngroups = 32;
    gid_t groups[32];
    struct passwd pw;
    struct group gr;

    /* Fetch passwd structure (contains first group ID for user) */

    pw = *getpwnam(user->name().toUtf8().data());

    /* Retrieve group list */

    if (getgrouplist(user->name().toUtf8().data(), pw.pw_gid, groups, &ngroups) == -1) {
        fprintf(stderr, "getgrouplist() returned -1; ngroups = %d\n",
                ngroups);
        return false;
    }

    /* Display list of retrieved groups, along with group names */

    for (int i = 0; i < ngroups; i++) {
        gr = *getgrgid(groups[i]);
        if (QString(gr.gr_name) == QString("nopasswdlogin")) {
            return true;
        }
    }

    return false;
}

void LockWorker::lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message)
{
    if (username != m_authUser->name())
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
    if (!unlocked) {
        qDebug() << "Authorization failed!";
        emit m_model->authFaildTipsMessage(tr("Wrong Password"));
        return;
    }

    // Auth success
//    switch (m_action) {
//    case Restart:       m_sessionManagerIter->RequestReboot();    break;
//    case Shutdown:      m_sessionManagerIter->RequestShutdown();    break;
//    case Suspend:       m_sessionManagerIter->RequestSuspend();     break;
//    default: break;
//    }

#ifdef LOCK_NO_QUIT
    m_passwdEditAnim->lineEdit()->setPlaceholderText("");
    m_passwdEditAnim->abortAuth();
    m_passwdEditAnim->hideAlert();
    m_passwdEditAnim->lineEdit()->clear();
    emit checkedHide();
#else
    qApp->exit();
#endif
}

void LockWorker::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "pam prompt: " << text << type;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = text.simplified() == "Password:" ? "" : text;

    switch (type) {
    case QLightDM::Greeter::PromptTypeSecret:
        if (m_isThumbAuth)
            return;

        if (msg.isEmpty() && !m_password.isEmpty())
            m_greeter->respond(m_password);

        if (!msg.isEmpty()) {
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

    if (!m_greeter->isAuthenticated()) {
        if (m_authUser->type() == User::Native) {
            emit m_model->authFaildTipsMessage(tr("Wrong Password"));
        }

        if (m_authUser->type() == User::ADDomain) {
            emit m_model->authFaildTipsMessage(tr("The domain account or password is not correct. Please enter again."));
        }

        return;
    }

//    qDebug() << "start session = " << m_sessionWidget->currentSessionName();

    auto startSessionSync = [=]() {
        QJsonObject json;
        json["Uid"] = static_cast<int>(m_authUser->uid());
        json["Type"] = m_authUser->type();
        m_greeter->startSessionSync(m_model->sessionKey());
    };

#ifndef DISABLE_LOGIN_ANI
    // NOTE(kirigaya): It is not necessary to display the login animation.

    emit m_model->authFinished(true);
    QTimer::singleShot(1000, this, startSessionSync);
#else
    startSessionSync();
#endif
}
