/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "userwidget.h"
#include "constants.h"
#include "dbus/dbuslockservice.h"

#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QSettings>
#include <QJsonObject>
#include <QJsonValue>
#include <unistd.h>
#include <pwd.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

DWIDGET_USE_NAMESPACE

bool userSort(UserButton *left, UserButton *right) {
    User *l = left->userInfo();
    User *r = right->userInfo();

    // AD Login Button is last of list;
    if (l->type() == User::ADDomain && l->uid() == 0) {
        return false;
    }

    if (l->type() != r->type()) {
        return l->type() == User::ADDomain;
    }

    return l->uid() < r->uid();
}

UserWidget::UserWidget(QWidget* parent)
    : QFrame(parent)
    , m_currentUser(nullptr)
    , m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this)
    , m_dbusLogined(new Logined("com.deepin.daemon.Accounts", "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
{
    m_dbusAccounts = new DBusAccounts(ACCOUNT_DBUS_SERVICE,  ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this);

    setFixedWidth(qApp->primaryScreen()->geometry().width());
//    setStyleSheet("background-color: red;");

    initUI();
    initConnections();

    // init native users
    for (const QString &userPath : m_dbusAccounts->userList())
        onNativeUserAdded(userPath);

    onLoginUserListChanged(m_dbusLogined->userList());

    m_adCheckStateTimer = new QTimer(this);
    m_adCheckStateTimer->setInterval(1000);
    m_adCheckStateTimer->setSingleShot(true);
    connect(m_adCheckStateTimer, &QTimer::timeout, this, &UserWidget::checkADState);

    m_adCheckStateTimer->start();

    // If current user is lightdm, here is greeter interface
    // else is dde-lock interface
    if (currentContextUser() == "lightdm") {
        for (UserButton *btn : m_availableUserButtons) {
            if (!btn->userInfo()->isLogin()) {
                m_currentUser = btn->userInfo();
                break;
            }
        }
    } else {
        // select current login user
        const QString &user = currentContextUser();
        for (UserButton *btn : m_availableUserButtons) {
            if (btn->userInfo()->name() == user) {
                m_currentUser = btn->userInfo();
                break;
            }
        }
    }

    // If not select any user
    if (!m_currentUser) {
        UserButton *btn = m_availableUserButtons.first();
        m_currentUser = btn->userInfo();
    }

    QTimer::singleShot(1, this, &UserWidget::updateIconPosition);
}

UserWidget::~UserWidget()
{
//    qDeleteAll(m_userBtns);
    qDeleteAll(m_availableUsers);
}

void UserWidget::initUI()
{
//    setCurrentUser(currentUser());

    const int count = m_availableUserButtons.count();
    const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.

    // Adjust size according to user count.
    if (maxLineCap < count) {
        setFixedSize(width(), USER_ICON_HEIGHT * qCeil(count * 1.0 / maxLineCap));
    } else {
        setFixedHeight(USER_ICON_HEIGHT);
    }
}

void UserWidget::initConnections()
{
    connect(m_dbusAccounts, &DBusAccounts::UserAdded, this, &UserWidget::onNativeUserAdded);
    connect(m_dbusAccounts, &DBusAccounts::UserDeleted, this, &UserWidget::onNativeUserRemoved);
    connect(m_dbusAccounts, &DBusAccounts::UserListChanged, this, [=] {
        emit userCountChanged(m_availableUserButtons.size());

        // keep order: AD User > Native User > AD Login Button
        std::sort(m_availableUserButtons.begin(), m_availableUserButtons.end(), userSort);
    });

    connect(m_dbusLogined, &Logined::UserListChanged, this, &UserWidget::onLoginUserListChanged);
}

//void UserWidget::onUserListChanged()
//{
//    for (const QString &name : m_dbusAccounts->userList())
//        onNativeUserAdded(name);
//}

void UserWidget::onNativeUserAdded(const QString &path)
{
//    if (m_userDbus.contains(path))
//        return;

    // search for exist user
    for (User *user : m_availableUsers)
    {
        if (user->type() != User::Native)
            continue;

        // already exist
        if (static_cast<NativeUser *>(user)->path() == path)
            return;
    }

    // append new user
    NativeUser *nativeUser = new NativeUser(path);

    appendUser(nativeUser);

//    UserInter *user = new UserInter(ACCOUNT_DBUS_SERVICE, path, QDBusConnection::systemBus(), this);
//    user->setSync(true);

//    m_userDbus.insert(path, user);

//    UserButton *userBtn = new UserButton(this);

//    m_userBtns.append(userBtn);

//    userBtn->setVisible(userBtn->name() == m_currentUser);
//    userBtn->setParent(this);

//    connect(userBtn, &UserButton::imageClicked, this, &UserWidget::updateCurrentUser);
//    connect(user, &__User::FullNameChanged, userBtn, &UserButton::updateDisplayName);
//    connect(user, &__User::AutomaticLoginChanged, userBtn, &UserButton::updateAutoLogin);
//    connect(user, &__User::DesktopBackgroundsChanged, userBtn, &UserButton::updateBackgrounds);
//    connect(user, &__User::HistoryLayoutChanged, userBtn, &UserButton::updateKbHistory);
//    connect(user, &__User::LayoutChanged, userBtn, &UserButton::updateKbLayout);
//    connect(user, &__User::IconFileChanged, userBtn, &UserButton::updateAvatar);
//    connect(user, &__User::UserNameChanged, userBtn, &UserButton::updateUserName);
//    connect(user, &__User::GreeterBackgroundChanged, userBtn, &UserButton::updateGreeterWallpaper);

//    userBtn->updateUserName(user->userName());
//    userBtn->updateDisplayName(user->fullName());
//    userBtn->updateGreeterWallpaper(user->greeterBackground());
//    userBtn->updateAutoLogin(user->automaticLogin());
//    userBtn->updateAvatar(user->iconFile());
//    userBtn->updateKbHistory(user->historyLayout());
//    userBtn->updateKbLayout(user->layout());
//    userBtn->updateBackgrounds(user->desktopBackgrounds());
//    userBtn->setDBus(user);

//    emit userCountChanged(m_userBtns.count());

//    onLoginUserListChanged(m_dbusLogined->userList());
}

void UserWidget::onNativeUserRemoved(const QString &name)
{
//    UserInter *user;
//    user = m_userDbus.find(name).value();

//    if (user) {
//        m_userDbus.remove(name);
//        user->deleteLater();

//        UserButton *button;
//        for (int index(0); index != m_userBtns.count(); ++index) {
//            button = m_userBtns.at(index);

//            if (button) {
//                m_userBtns.removeOne(button);
//                button->deleteLater();
//                emit userCountChanged(m_userBtns.count());
//                return;
//            }
//        }
//    }

//    onLoginUserListChanged(m_dbusLogined->userList());

    for (int i = 0; i != m_availableUserButtons.size(); ++i) {
        UserButton *btn = m_availableUserButtons.at(i);
        if (btn->userInfo()->name() == name) {
            m_availableUserButtons.removeAt(i);
            m_availableUsers.removeOne(btn->userInfo());
            btn->deleteLater();
            break;
        }
    }
}

void UserWidget::onLoginUserListChanged(const QString &loginedUserInfo)
{
    if (loginedUserInfo.isEmpty()) {
        return;
    }

    QJsonObject userList = QJsonDocument::fromJson(loginedUserInfo.toUtf8()).object();

    // All users are in m_availableUsers.
    // If there are no users in the login list,
    // they are considered as AD users and need to be added.

    QList<int> availableUidList;
    for (User *user : m_availableUsers) {
        availableUidList << user->uid();
    }

    QList<int> logindUidList;
    for (const QString &userId : userList.keys()) {
        const bool haveDisplay = checkHaveDisplay(userList[userId].toArray());
        if (haveDisplay && !availableUidList.contains(userId.toInt())) {
            // Non existing users, created as AD users
            addAddomainUser(userId.toInt());
            availableUidList << userId.toInt();
        }
        // skip not have display users
        if (haveDisplay) {
            logindUidList << userId.toInt();
        }
    }

    // Remove the nonexistent button from the already-obtained id.
    for (int i = 0; i != availableUidList.size(); ++i) {
        UserButton *btn = m_availableUserButtons.at(i);
        User *user = btn->userInfo();
        const bool isListContains = logindUidList.contains(user->uid());

        // skip fake ADDomain login button
        if (!isListContains && user->type() == User::ADDomain && user->uid() != 0) {
            m_availableUserButtons.removeAt(i);
            m_availableUsers.removeOne(user);
            user->deleteLater();
            btn->deleteLater();
        } else {
            user->setisLogind(isListContains);
        }
    }

    // keep order: AD User > Native User > AD Login Button
    std::sort(m_availableUserButtons.begin(), m_availableUserButtons.end(), userSort);

    updateAllADUserInfo();

    QTimer::singleShot(1, this, [=] {
        emit userCountChanged(m_availableUserButtons.size());
    });

    return;


    // NOTE(kirigaya): check in AD Domain
//    QProcess *process = new QProcess(this);
//    connect(process, &QProcess::readyReadStandardOutput, this, [=] {
//        QRegularExpression re("Name:\\s");
//        QRegularExpressionMatchIterator i = re.globalMatch(process->readAll());

//        if (i.hasNext()) {
//            initADLogin();

//            struct passwd *pws;
//            pws = getpwuid(getuid());

//            if (QString(pws->pw_name) != "lightdm") {
//                UserButton *button = getUserByName(pws->pw_name);

//                if (!button)
//                    initOtherUser(pws->pw_name);
//            }
//        }
//    });

//    process->start("/opt/pbis/bin/enum-users");

    // NOTE(kirigaya): wait for some time to update the position;
//    QTimer::singleShot(100, this, [=] {
//        updateCurrentUserPos();
//        setCurrentUser(currentUser());
//    });

//    for (UserInter *user : m_userDbus.values()) {
//        const QJsonArray &array = obj[user->uid()].toArray();
//        if (array.isEmpty())
//            continue;

//        for (int i(0); i != array.count(); ++i) {
//            const QJsonObject &obj = array.at(i).toObject();
//            if (obj["Display"].toString().isEmpty() || obj["Desktop"].toString().isEmpty())
//                continue;

//            m_loggedInUsers << user->userName();
//        }
//    }

//    for (UserInter *inter : m_userDbus.values()) {
//        if (!inter->locked() && !m_whiteList.contains(inter->userName()))
//            m_whiteList.append(inter->userName());
//    }
}

//UserButton *UserWidget::getUserByName(const QString &username)
//{
//    for (UserButton *button : m_userBtns)
//        if (button->name() == username)
//            return button;

//    return nullptr;
//}

//void UserWidget::initOtherUser(const QString &username)
//{
//    UserButton *user = new UserButton(this);
//    user->updateUserName(username);
//    user->updateDisplayName(username);
//    user->updateGreeterWallpaper("/usr/share/backgrounds/deepin/desktop.jpg");
//    user->updateAutoLogin(false);
//    user->updateAvatar(":/img/default_avatar.png");
//    user->updateKbHistory(QStringList());
//    user->updateKbLayout("");
//    user->updateBackgrounds(QStringList() << "/usr/share/backgrounds/deepin/desktop.jpg");
//    user->setDBus(nullptr);

//    connect(user, &UserButton::imageClicked, this, &UserWidget::updateCurrentUser);

//    m_userBtns << user;
//}

void UserWidget::initADLogin()
{
//    if (m_adLoginBtn)
//        return;

//    const QString &username = tr("AD Domain");

//    m_adLoginBtn = new UserButton(this);
//    m_adLoginBtn->updateUserName(username);
//    m_adLoginBtn->updateDisplayName(username);
//    m_adLoginBtn->updateGreeterWallpaper("/usr/share/backgrounds/deepin/desktop.jpg");
//    m_adLoginBtn->updateAutoLogin(false);
//    m_adLoginBtn->updateAvatar(":/img/default_avatar.png");
//    m_adLoginBtn->updateKbHistory(QStringList());
//    m_adLoginBtn->updateKbLayout("");
//    m_adLoginBtn->updateBackgrounds(QStringList() << "/usr/share/backgrounds/deepin/desktop.jpg");
//    m_adLoginBtn->setDBus(nullptr);

//    connect(m_adLoginBtn, &UserButton::imageClicked, this, [=] {
//        releaseKeyboard();
//        setCurrentUser(username);
//        updateCurrentUserPos(200);
//        emit otherUserLogin();
//    });

//    m_whiteList << username;

//    m_userBtns << m_adLoginBtn;

    //    emit userCountChanged(m_userBtns.count());
}

void UserWidget::addAddomainUser(int uid)
{
    ADDomainUser *user = new ADDomainUser(uid);
    appendUser(user);
}

void UserWidget::onUserChoosed()
{
    // hide buttons
    for (UserButton *ub : m_availableUserButtons) {
        ub->move(rect().center() - ub->rect().center());
        ub->setLoginIconVisible(false);
        ub->hide();
    }

    UserButton *clickedButton = qobject_cast<UserButton *>(sender());
    Q_ASSERT(clickedButton);

    releaseKeyboard();

    // If user is login, switch to this user
    if (clickedButton->userInfo()->isLogin()) {
        emit switchToLogindUser(clickedButton->userInfo());
    } else {
        // update current user
        m_currentUser = clickedButton->userInfo();
        emit currentUserChanged(m_currentUser);
    }

    for (UserButton *btn : m_availableUserButtons) {
        if (btn->userInfo() == m_currentUser) {
            btn->show();
            btn->setImageSize(UserButton::AvatarLargerSize);
            btn->setSelected(false);
            btn->setLoginIconVisible(false);
            break;
        }
    }
}

//void UserWidget::setCurrentUser(const QString &username)
//{
//    qDebug() << username << sender();

//    m_currentUser = username;

//    isChooseUserMode = false;

//    for (UserButton *user : m_userBtns) {
//        if (user->name() == username) {
//            user->showButton();
//            user->setImageSize(user->AvatarLargerSize);
//            user->setButtonChecked(false);
//            user->setSelected(false);
//            user->show();
//            m_currentBtns = user;
//        } else
//            user->hide(180);
//    }

//    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
//}

void UserWidget::removeUser(QString name)
{
//    qDebug() << "remove user: " << name;

//    for (int i(0); i < m_userBtns.count(); ++i) {
//        if (m_userBtns[i]->name() == name) {
//            UserButton *btn = m_userBtns[i];
//            m_userBtns.removeAt(i);
//            btn->deleteLater();
//            break;
//        }
//    }
}

void UserWidget::expandWidget()
{
    isChooseUserMode = true;

    const int count = m_availableUserButtons.size();
//    const int count = m_userBtns.count();
    const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.
    const int offset = (width() - USER_ICON_WIDTH * qMin(count, maxLineCap)) / 2;

    // Adjust size according to user count.
    if (maxLineCap < count) {
        setFixedSize(width(), USER_ICON_HEIGHT * qCeil(count * 1.0 / maxLineCap));
    }

    for (int i = 0; i != count; ++i)
    {
        UserButton *userButton = m_availableUserButtons.at(i);
//        const QString username = userButton->name();

//        userButton->setSelected(username == m_currentUser);

//        if (m_loggedInUsers.contains(username)) {
//            userButton->setButtonChecked(true);
//        } else {
//            userButton->setButtonChecked(false);
//        }

        userButton->setSelected(userButton->userInfo() == m_currentUser);

        // Allow user button draw logind icon
        userButton->setLoginIconVisible(true);

        userButton->stopAnimation();
        userButton->show();
        userButton->show();
        userButton->setImageSize(UserButton::AvatarSmallSize);
        if (i + 1 <= maxLineCap) {
            // the first line.
            userButton->move(QPoint(offset + i * USER_ICON_WIDTH, 0));
        } else {
            // the second line.
            userButton->move(QPoint(offset + (i - maxLineCap) * USER_ICON_WIDTH, USER_ICON_HEIGHT));
        }
    }

    setFocus();
    QTimer::singleShot(100, this, &UserWidget::grabKeyboard);

//    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
}

void UserWidget::saveLastUser()
{
//    m_lockInter.SwitchToUser(currentUser()).waitForFinished();
}

void UserWidget::saveADUser(const QString &username)
{
    QDir dir(DDESESSIONCC::LAST_USER_CONFIG);

    if (!dir.exists())
       qDebug() << dir.mkpath(DDESESSIONCC::LAST_USER_CONFIG);

    QFile file(DDESESSIONCC::LAST_USER_CONFIG + QDir::separator() + "LAST_USER");

    // NOTE(kirigaya): If file is not exist, create it.
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
    }

    QSettings setting(DDESESSIONCC::LAST_USER_CONFIG + QDir::separator() + "LAST_USER", QSettings::IniFormat);
    setting.beginGroup("ADDOMAIN");
    setting.setValue("USERNAME", username);
    setting.endGroup();
    setting.sync();
}

void UserWidget::restoreUser(User *user)
{
   for (UserButton *btn : m_availableUserButtons) {
       // move to center
       btn->move(rect().center() - btn->rect().center());
       btn->setImageSize(UserButton::AvatarLargerSize);

       if (btn->userInfo() == user) {
           m_currentUser = user;
           btn->show();
           btn->setSelected(false);
           btn->setLoginIconVisible(false);
       } else {
           btn->hide();
       }
   }
}

void UserWidget::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);

    QTimer::singleShot(100, this, &UserWidget::updateIconPosition);
}

void UserWidget::keyReleaseEvent(QKeyEvent *event)
{
    QFrame::keyReleaseEvent(event);

    switch (event->key()) {
    case Qt::Key_Left:
        switchPreviousUser();
        break;
    case Qt::Key_Right:
        switchNextUser();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        for (UserButton *ub : m_availableUserButtons) {
            if (ub->selected()) {
                ub->click();
                break;
            }
        }
        break;
    case Qt::Key_Escape:

        break;
    default:
        break;
    }
}

//void UserWidget::chooseButtonChecked() {
//    bool checkedBtsExist = false;
//    for (UserButton* user: m_userBtns) {
//        if (user->selected()) {
//            updateCurrentUser(user->name());
//            checkedBtsExist = true;
//        }
//    }
//    if (!checkedBtsExist) {
//        updateCurrentUser(m_currentUser);
//    }

//    releaseKeyboard();
//}

void UserWidget::appendUser(User *user)
{
    UserButton *userButton = new UserButton(user, this);

    userButton->hide();
    userButton->setLoginIconVisible(false);
    userButton->setImageSize(UserButton::AvatarLargerSize);

    m_availableUsers << user;
    m_availableUserButtons << userButton;

    connect(userButton, &UserButton::clicked, this, &UserWidget::onUserChoosed);
}

void UserWidget::switchPreviousUser()
{
    for (int i = 0; i != m_availableUserButtons.size(); ++i) {
        if (m_availableUserButtons[i]->selected()) {
            m_availableUserButtons[i]->setSelected(false);
            if (i == 0) {
                m_availableUserButtons.last()->setSelected(true);
            } else {
                m_availableUserButtons[i - 1]->setSelected(true);
            }
            break;
        }
    }
}

void UserWidget::switchNextUser()
{
    for (int i = 0; i != m_availableUserButtons.size(); ++i) {
        if (m_availableUserButtons[i]->selected()) {
            m_availableUserButtons[i]->setSelected(false);
            if (i == (m_availableUserButtons.size() - 1)) {
                m_availableUserButtons.first()->setSelected(true);
            } else {
                m_availableUserButtons[i + 1]->setSelected(true);
            }
            break;
        }
    }
}

bool UserWidget::checkHaveDisplay(const QJsonArray &array)
{
    for (int i(0); i != array.count(); ++i) {
        const QJsonObject &obj = array.at(i).toObject();

        // If user without desktop or display, this is system service, need skip.
        if (!obj["Display"].toString().isEmpty() && !obj["Desktop"].toString().isEmpty()) {
            return true;
        }
    }

    return false;
}

void UserWidget::updateAllADUserInfo()
{
    // update AD User Info
    QProcess process(this);
    process.start("/opt/pbis/bin/enum-users");
    process.waitForFinished();

    const QString &output = process.readAll();

    QRegularExpression re("Name:\\s+(.+?)\nUid:\\s+(.+?)\n");
    QRegularExpressionMatchIterator match = re.globalMatch(output, 0, QRegularExpression::NormalMatch);

    QMap<int, QString> adUserList;
    while (match.hasNext()) {
        QRegularExpressionMatch m = match.next();
        if (m.isValid()) {
            adUserList.insert(m.captured(2).toInt(), m.captured(1));
        }
    }

    // update all ad domain type info
    for (UserButton *btn : m_availableUserButtons) {
        User *user = btn->userInfo();
        if (user->type() == User::ADDomain) {
            ADDomainUser *adUser = qobject_cast<ADDomainUser*>(user);
            if (adUserList.keys().contains(adUser->uid())) {
                adUser->setUserDisplayName(adUserList[adUser->uid()]);
            }
        }
    }
}

void UserWidget::updateIconPosition()
{
    // update buttons position
    if (isChooseUserMode)
    {
        expandWidget();
    } else {
        // move all buttons to center
        for (UserButton *btn : m_availableUserButtons) {
            btn->move(rect().center() - btn->rect().center(), true);
            if (btn->userInfo() == m_currentUser) {
                btn->show();
            }
        }
    }
}

void UserWidget::checkADState()
{
    // check AD Domain
    QProcess process(this);
    process.start("/opt/pbis/bin/enum-users");
    process.waitForFinished();

    if (process.readAll().contains("Name:")) {
        ADDomainUser *user = new ADDomainUser(0);
        user->setUserDisplayName(tr("Domain account"));
        user->setisLogind(false);
        appendUser(user);

        emit userCountChanged(m_availableUserButtons.size());

        // keep order: AD User > Native User > AD Login Button
        std::sort(m_availableUserButtons.begin(), m_availableUserButtons.end(), userSort);

        m_adCheckStateTimer->stop();
        return;
    }
    qDebug() << "Checking AD state!!!!";
    m_adCheckStateTimer->start();
}

const QString UserWidget::currentContextUser()
{
    struct passwd *pws;
    pws = getpwuid(getuid());
    return QString(pws->pw_name);
}

const QList<User *> UserWidget::loginedUsers() const
{
    QList<User *> list;

    for (User *user : m_availableUsers) {
        if (user->isLogin()) {
            list << user;
        }
    }

    return list;
}

const QList<User *> UserWidget::allUsers() const
{
    return m_availableUsers;
}

//const QString UserWidget::currentUser()
//{
//    return m_availableUserButtons.first()->userInfo()->name();

//    qDebug() << Q_FUNC_INFO << m_currentUser;

//    if (!m_currentUser.isEmpty() && m_whiteList.contains(m_currentUser)) {
//        return m_currentUser;
//    }

//    struct passwd *pws;
//    pws = getpwuid(getuid());
//    const QString currentLogin(pws->pw_name);

//    //except the current-user named lightdm
//    if (!currentLogin.isEmpty() && currentLogin!="lightdm")
//        return currentLogin;

//    if (!m_whiteList.isEmpty())
//        return m_whiteList.first();

//    // return first user
////    if (m_userDbus.count() > 0) {
////        const QString tmpUsername = m_userDbus.first()->userName();
////        return tmpUsername;
////    }

//    qWarning() << "no users !!!";
//    return QString();
//}

//const QString UserWidget::getUserAvatar(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    if (user)
//        return user->avatar();
//    return QString();
//}

//bool UserWidget::getUserIsAutoLogin(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->automaticLogin() : false;
//}

//const QString UserWidget::getUserGreeterBackground(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->greeter() : QString();
//}

//const QStringList UserWidget::getUserKBHistory(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->kbHistory() : QStringList();
//}

//const QString UserWidget::getUserKBLayout(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->kblayout() : QString();
//}

//const QString UserWidget::getDisplayName(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->displayName() : QString();
//}

//const QStringList UserWidget::getUserDesktopBackground(const QString &username)
//{
//    UserButton *user = getUserByName(username);
//    return user ? user->backgrounds() : QStringList();
//}

//const QStringList UserWidget::users() const
//{
//    QStringList list;
//    for (UserButton *btn : m_userBtns) {
//        if (btn->dbus()) {
//            list << btn->name();
//        }
//    }
//    return list;
//}

//void UserWidget::setUserKBlayout(const QString &username, const QString &layout)
//{
//    UserButton *user = getUserByName(username);
//    if (user && user->dbus())
//        user->dbus()->SetLayout(layout);
//}

User::User(QObject *parent) : QObject(parent)
{

}

bool User::operator==(const User &user) const
{
    return type() == user.type() &&
            m_uid == user.m_uid;
}

void User::setisLogind(bool isLogind) {
    if (m_isLogind == isLogind) {
        return;
    }

    m_isLogind = isLogind;

    emit logindChanged(isLogind);
}

NativeUser::NativeUser(const QString &path, QObject *parent)
    : User(parent)

    , m_userInter(new UserInter(ACCOUNT_DBUS_SERVICE, path, QDBusConnection::systemBus(), this))
{
    connect(m_userInter, &UserInter::IconFileChanged, this, &NativeUser::avatarChanged);
    connect(m_userInter, &UserInter::FullNameChanged, this, [=] (const QString &fullname) {
        emit displayNameChanged(fullname.isEmpty() ? m_userName : fullname);
    });

    m_userName = m_userInter->userName();
    m_uid = m_userInter->uid().toInt();
}

void NativeUser::setCurrentLayout(const QString &layout)
{
    m_userInter->SetLayout(layout);
}

QString NativeUser::displayName() const
{
    const QString &fullname = m_userInter->fullName();
    return fullname.isEmpty() ? name() : fullname;
}

QString NativeUser::avatarPath() const
{
    return m_userInter->iconFile();
}

QString NativeUser::greeterBackgroundPath() const
{
    QUrl url(m_userInter->greeterBackground());

    if (url.isLocalFile()) {
        return url.path();
    }

    return url.url();
}

QString NativeUser::desktopBackgroundPath() const
{
    const QStringList &list = m_userInter->desktopBackgrounds();
    QString background;

    if (list.isEmpty()) {
        background = m_userInter->backgroundFile();
    } else {
        background = list.first();
    }

    QUrl url(background);
    if (url.isLocalFile()) {
        return url.path();
    }

    return url.url();
}

QStringList NativeUser::kbLayoutList()
{
    return m_userInter->historyLayout();
}

QString NativeUser::currentKBLayout()
{
    return m_userInter->layout();
}

ADDomainUser::ADDomainUser(int uid, QObject *parent)
    : User(parent)
{
    m_uid = uid;
}

void ADDomainUser::setUserDisplayName(const QString &name)
{
    if (m_userName == name) {
        return;
    }

    m_userName = name;

    emit displayNameChanged(name);
}

QString ADDomainUser::avatarPath() const
{
    return QString(":/img/default_avatar.png");
}

QString ADDomainUser::greeterBackgroundPath() const
{
    return QString("/usr/share/backgrounds/deepin/desktop.jpg");
}

QString ADDomainUser::desktopBackgroundPath() const
{
    return QString("/usr/share/backgrounds/deepin/desktop.jpg");
}
