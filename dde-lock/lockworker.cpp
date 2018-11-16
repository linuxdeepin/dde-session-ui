#include "lockworker.h"

#include "sessionbasemodel.h"
#include "userinfo.h"

#include <unistd.h>
#include <QDebug>

LockWorker::LockWorker(SessionBaseModel * const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_accountsInter(new AccountsInter(ACCOUNT_DBUS_SERVICE, ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this))
{
    m_accountsInter->setSync(false);

    m_currentUserUid = getuid();

    connect(m_accountsInter, &AccountsInter::UserListChanged, this, &LockWorker::onUserListChanged);
    connect(m_accountsInter, &AccountsInter::UserAdded, this, &LockWorker::onUserAdded);
    connect(m_accountsInter, &AccountsInter::UserDeleted, this, &LockWorker::onUserRemove);

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

    if (user_ptr->uid() == m_currentUserUid) {
        m_model->setCurrentUser(user_ptr);
    }

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
