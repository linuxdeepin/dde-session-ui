#include "lockworker.h"

#include "sessionbasemodel.h"
#include "userinfo.h"

#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include <QDebug>

LockWorker::LockWorker(SessionBaseModel * const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_accountsInter(new AccountsInter(ACCOUNT_DBUS_SERVICE, ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this))
    , m_loginedInter(new LoginedInter(ACCOUNT_DBUS_SERVICE, "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
{
    m_accountsInter->setSync(false);
    m_loginedInter->setSync(true);

    m_currentUserUid = getuid();

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
        }
        else {
            m_model->setCurrentUser(user);
        }
        return;
    }

    // if type is lock, switch to greeter

}

void LockWorker::authUser(std::shared_ptr<User> user, const QString &password)
{
    // auth interface
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
