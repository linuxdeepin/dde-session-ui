#include "authinterface.h"
#include "sessionbasemodel.h"
#include "userinfo.h"

#include <grp.h>
#include <libintl.h>
#include <pwd.h>
#include <unistd.h>

using namespace Auth;

static std::pair<bool, qint64> checkIsPartitionType(const QStringList &list)
{
    std::pair<bool, qint64> result{ false, -1 };

    if (list.length() != 5) {
        return result;
    }

    const QString type{ list[1] };
    const QString size{ list[2] };

    result.first  = type == "partition";
    result.second = size.toLongLong() * 1024.0f;

    return result;
}

static qint64 get_power_image_size()
{
    qint64 size{ 0 };
    QFile  file("/sys/power/image_size");

    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        size = file.readAll().trimmed().toLongLong();
        file.close();
    }

    return size;
}

AuthInterface::AuthInterface(SessionBaseModel *const model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_accountsInter(new AccountsInter(ACCOUNT_DBUS_SERVICE, ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this))
    , m_loginedInter(new LoginedInter(ACCOUNT_DBUS_SERVICE, "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
    , m_lastLogoutUid(0)
    , m_loginUserList(0)
{

}

void AuthInterface::setLayout(std::shared_ptr<User> user, const QString &layout) {
    user->setCurrentLayout(layout);
}

void AuthInterface::onUserListChanged(const QStringList &list)
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

void AuthInterface::onUserAdded(const QString &user)
{
    std::shared_ptr<User> user_ptr(new NativeUser(user));
    user_ptr->setisLogind(isLogined(user_ptr->uid()));
    m_model->userAdd(user_ptr);
}

void AuthInterface::onUserRemove(const QString &user)
{
    QList<std::shared_ptr<User>> list = m_model->userList();

    for (auto u : list) {
        if (u->path() == user && u->type() == User::Native) {
            m_model->userRemoved(u);
            break;
        }
    }
}

void AuthInterface::initData()
{
    onUserListChanged(m_accountsInter->userList());
    onLoginUserListChanged(m_loginedInter->userList());
    onLastLogoutUserChanged(m_loginedInter->lastLogoutUser());
    checkPowerInfo();
    checkVirtualKB();
}

void AuthInterface::initDBus()
{
    m_accountsInter->setSync(true);
    m_loginedInter->setSync(true);

    connect(m_accountsInter, &AccountsInter::UserListChanged, this, &AuthInterface::onUserListChanged, Qt::QueuedConnection);
    connect(m_accountsInter, &AccountsInter::UserAdded, this, &AuthInterface::onUserAdded, Qt::QueuedConnection);
    connect(m_accountsInter, &AccountsInter::UserDeleted, this, &AuthInterface::onUserRemove, Qt::QueuedConnection);

    connect(m_loginedInter, &LoginedInter::LastLogoutUserChanged, this, &AuthInterface::onLastLogoutUserChanged);
    connect(m_loginedInter, &LoginedInter::UserListChanged, this, &AuthInterface::onLoginUserListChanged);
}

void AuthInterface::onLastLogoutUserChanged(uint uid)
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

void AuthInterface::onLoginUserListChanged(const QString &list)
{
    m_loginUserList.clear();

    std::list<uint> availableUidList;
    for (std::shared_ptr<User> user : m_model->userList()) {
        availableUidList.push_back(user->uid());
    }

    QJsonObject userList = QJsonDocument::fromJson(list.toUtf8()).object();
    for (auto it = userList.constBegin(); it != userList.constEnd(); ++it) {
        const bool haveDisplay = checkHaveDisplay(it.value().toArray());
        const uint uid         = it.key().toUInt();

        // skip not have display users
        if (haveDisplay) {
            m_loginUserList.push_back(uid);
        }

        auto find_it = std::find_if(
            availableUidList.begin(), availableUidList.end(),
            [=](const uint find_addomain_uid) { return find_addomain_uid == uid; });

        if (haveDisplay && find_it == availableUidList.end()) {
            // init addoman user
            std::shared_ptr<User> u(new ADDomainUser(uid));
            u->setisLogind(true);

            struct passwd *pws;
            pws = getpwuid(uid);

            static_cast<ADDomainUser *>(u.get())->setUserDisplayName(pws->pw_name);
            static_cast<ADDomainUser *>(u.get())->setUserName(pws->pw_name);

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

        auto find_it =
            std::find_if(m_loginUserList.begin(), m_loginUserList.end(),
                         [=](const uint find_uid) { return find_uid == user->uid(); });

        if (find_it == m_loginUserList.end() &&
            (user->type() == User::ADDomain && user->uid() != 0)) {
            m_model->userRemoved(user);
            it = uList.erase(it);
        }
        else {
            user->setisLogind(isLogined(user->uid()));
            ++it;
        }
    }
}

bool AuthInterface::checkHaveDisplay(const QJsonArray &array)
{
    for (auto it = array.constBegin(); it != array.constEnd(); ++it) {
        const QJsonObject &obj = (*it).toObject();

        // If user without desktop or display, this is system service, need skip.
        if (!obj["Display"].toString().isEmpty() &&
            !obj["Desktop"].toString().isEmpty()) {
            return true;
        }
    }

    return false;
}

bool AuthInterface::isLogined(uint uid)
{
    auto isLogind = std::find_if(m_loginUserList.begin(), m_loginUserList.end(),
                                 [=](uint UID) { return uid == UID; });

    return isLogind != m_loginUserList.end();
}

void AuthInterface::checkPowerInfo()
{
    m_model->setCanSleep(valueByQSettings<bool>("Power", "sleep", true));

    if (valueByQSettings<bool>("Power", "hibernate", false)) {
        checkSwap();
    }
}

void AuthInterface::checkVirtualKB()
{
    m_model->setHasVirtualKB(QProcess::execute("which", QStringList() << "onboard") == 0);
}

void AuthInterface::checkSwap()
{
    QFile file("/proc/swaps");
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        bool           hasSwap{ false };
        const QString &body = file.readAll();
        QTextStream    stream(body.toUtf8());
        while (!stream.atEnd()) {
            const std::pair<bool, qint64> result =
                checkIsPartitionType(stream.readLine().simplified().split(
                    " ", QString::SplitBehavior::SkipEmptyParts));
            qint64 image_size{ get_power_image_size() };

            if (result.first) {
                hasSwap = image_size < result.second;
            }

            qDebug() << "check using swap partition!";
            qDebug() << QString("image_size: %1, swap partition size: %2")
                            .arg(QString::number(image_size))
                            .arg(QString::number(result.second));

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
