#include "sessionbasemodel.h"

#include <QDebug>

SessionBaseModel::SessionBaseModel(AuthType type, QObject *parent)
    : QObject(parent)
    , m_currentType(type)
{

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
