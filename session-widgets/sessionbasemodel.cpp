#include "sessionbasemodel.h"

#include <QDebug>

SessionBaseModel::SessionBaseModel(AuthType type, QObject *parent)
    : QObject(parent)
    , m_currentType(type)
{

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

void SessionBaseModel::userAdd(std::shared_ptr<User> user)
{
    m_userList << user;

    emit onUserAdded(user);
}

void SessionBaseModel::userRemoved(std::shared_ptr<User> user)
{
    emit onUserRemoved(user->uid());

    m_userList.removeOne(user);
}

void SessionBaseModel::setCurrentUser(std::shared_ptr<User> user)
{
    m_currentUser = user;

    emit currentUserChanged(user);
}

void SessionBaseModel::setLastLogoutUser(const std::shared_ptr<User> &lastLogoutUser)
{
    m_lastLogoutUser = lastLogoutUser;
}

void SessionBaseModel::setSessionKey(const QString &sessionKey)
{
    m_sessionKey = sessionKey;
}

void SessionBaseModel::setPowerAction(const PowerAction &powerAction)
{
    if (powerAction == m_powerAction) return;

    m_powerAction = powerAction;

    emit onPowerActionChanged(powerAction);
}
