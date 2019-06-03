#include "sessionbasemodel.h"

#include <QDebug>

#define SessionManagerService "com.deepin.SessionManager"
#define SessionManagerPath "/com/deepin/SessionManager"

using namespace com::deepin;

SessionBaseModel::SessionBaseModel(AuthType type, QObject *parent)
    : QObject(parent)
    , m_sessionManagerInter(nullptr)
    , m_hasSwap(false)
    , m_isShow(false)
    , m_canSleep(false)
    , m_currentType(type)
    , m_currentUser(nullptr)
    , m_powerAction(PowerAction::RequireNormal)
{
    if (m_currentType == LockType) {
        m_sessionManagerInter = new SessionManager(SessionManagerService, SessionManagerPath, QDBusConnection::sessionBus(), this);
    }
}

std::shared_ptr<User> SessionBaseModel::findUserByUid(const uint uid) const
{
    for (auto user : m_userList) {
        if (user->uid() == uid) {
            return user;
        }
    }

    qWarning() << "Wrong, you shouldn't be here!";
    return std::shared_ptr<User>(nullptr);
}

const QList<std::shared_ptr<User> > SessionBaseModel::logindUser()
{
    QList<std::shared_ptr<User>> userList;
    for (auto user : m_userList) {
        if (user->isLogin()) {
            userList << user;
        }
    }

    return userList;
}

void SessionBaseModel::userAdd(std::shared_ptr<User> user)
{
    m_userList << user;

    emit onUserAdded(user);
    emit onUserListChanged(m_userList);

    // NOTE(justforlxz): If all users are login and new users
    // are added at this time, switch to the user
    bool isNeed = true;
    for (auto it = m_userList.cbegin(); it != m_userList.cend(); ++it) {
        if (!(*it)->isLogin()) {
            isNeed = false;
            break;
        }
    }

    if (isNeed) {
        setCurrentUser(user);
    }
}

void SessionBaseModel::userRemoved(std::shared_ptr<User> user)
{
    emit onUserRemoved(user->uid());

    m_userList.removeOne(user);
    emit onUserListChanged(m_userList);

    // NOTE(justforlxz): If the current user is deleted, switch to the
    // first unlogin user. If it does not exist, switch to the first login user.
    if (user == m_currentUser) {
        QList<std::shared_ptr<User>> logindUserList;
        QList<std::shared_ptr<User>> unloginUserList;
        for (auto it = m_userList.cbegin(); it != m_userList.cend(); ++it) {
            if ((*it)->isLogin()) {
                logindUserList << (*it);
            }
            else {
                unloginUserList << (*it);
            }
        }

        if (unloginUserList.isEmpty()) {
            if (!logindUserList.isEmpty()) {
                setCurrentUser(logindUserList.first());
            }
        }
        else {
            setCurrentUser(unloginUserList.first());
        }
    }
}

void SessionBaseModel::setCurrentUser(std::shared_ptr<User> user)
{
    if (m_currentUser == user) return;

    m_currentUser = user;

    emit currentUserChanged(user);
}

void SessionBaseModel::setLastLogoutUser(const std::shared_ptr<User> &lastLogoutUser)
{
    m_lastLogoutUser = lastLogoutUser;
}

void SessionBaseModel::setSessionKey(const QString &sessionKey)
{
    if (m_sessionKey == sessionKey) return;

    m_sessionKey = sessionKey;

    emit onSessionKeyChanged(sessionKey);
}

void SessionBaseModel::setPowerAction(const PowerAction &powerAction)
{
    if (powerAction == m_powerAction) return;

    m_powerAction = powerAction;

    emit onPowerActionChanged(powerAction);
}

void SessionBaseModel::setCurrentModeState(const ModeStatus &currentModeState)
{
    if (m_currentModeState == currentModeState) return;

    m_currentModeState = currentModeState;

    emit onStatusChanged(currentModeState);
}

void SessionBaseModel::setHasVirtualKB(bool hasVirtualKB)
{
    if (m_hasVirtualKB == hasVirtualKB) return;

    m_hasVirtualKB = hasVirtualKB;

    emit hasVirtualKBChanged(hasVirtualKB);
}

void SessionBaseModel::setHasSwap(bool hasSwap) {
    if (m_hasSwap == hasSwap) return;

    m_hasSwap = hasSwap;

    emit onHasSwapChanged(hasSwap);
}

void SessionBaseModel::setIsShow(bool isShow)
{
    if (m_isShow == isShow) return;

    m_isShow = isShow;

#ifndef QT_DEBUG
    if (m_sessionManagerInter && m_currentType == LockType) {
        m_sessionManagerInter->SetLocked(m_isShow);
    }
#endif

    emit visibleChanged(isShow);
}

void SessionBaseModel::setCanSleep(bool canSleep)
{
    if (m_canSleep == canSleep) return;

    m_canSleep = canSleep;

    emit canSleepChanged(canSleep);
}

void SessionBaseModel::setAllowShowUserSwitchButton(bool allowShowUserSwitchButton)
{
    if (m_allowShowUserSwitchButton == allowShowUserSwitchButton) return;

    m_allowShowUserSwitchButton = allowShowUserSwitchButton;

    emit allowShowUserSwitchButtonChanged(allowShowUserSwitchButton);
}

void SessionBaseModel::setAlwaysShowUserSwitchButton(bool alwaysShowUserSwitchButton)
{
    m_alwaysShowUserSwitchButton = alwaysShowUserSwitchButton;
}
