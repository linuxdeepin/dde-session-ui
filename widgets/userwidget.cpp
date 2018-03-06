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

UserWidget::UserWidget(QWidget* parent)
    : QFrame(parent),

      m_currentUser(nullptr),
      m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this),
      m_dbusLogined(new Logined("com.deepin.daemon.Accounts", "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
{
//    m_currentUser = m_lockInter.CurrentUser();
//    qDebug() << Q_FUNC_INFO << m_currentUser;

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedWidth(qApp->desktop()->width());

    m_dbusAccounts = new DBusAccounts(ACCOUNT_DBUS_SERVICE,  ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this);

    m_dbusLogined->setSync(false);
    m_dbusLogined->userList();

    initUI();
    initConnections();
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
//    connect(m_dbusAccounts, &DBusAccounts::UserListChanged, this, &UserWidget::onUserListChanged);
    connect(m_dbusAccounts, &DBusAccounts::UserAdded, this, &UserWidget::onNativeUserAdded);
    connect(m_dbusAccounts, &DBusAccounts::UserDeleted, this, &UserWidget::onUserRemoved);

    connect(m_dbusLogined, &Logined::UserListChanged, this, &UserWidget::onLoginUserListChanged);
//    connect(this, &UserWidget::userChanged, this, [=] {
//        updateCurrentUserPos(200);
//    });

    connect(m_dbusAccounts, &DBusAccounts::UserListChanged, this, &UserWidget::onNativeUserListChanged);

    onNativeUserListChanged();
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

void UserWidget::onUserRemoved(const QString &name)
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
}

void UserWidget::onLoginUserListChanged(const QString &loginedUserInfo)
{
    QJsonObject userList = QJsonDocument::fromJson(loginedUserInfo.toUtf8()).object();

    for (const auto &userId : userList.keys())
    {
        // TODO: add domain user
        qDebug() << userId;
    }

    return;


    // NOTE(kirigaya): check in AD Domain
    QProcess *process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, [=] {
        QRegularExpression re("Name:\\s");
        QRegularExpressionMatchIterator i = re.globalMatch(process->readAll());

        if (i.hasNext()) {
            initADLogin();

            struct passwd *pws;
            pws = getpwuid(getuid());

            if (QString(pws->pw_name) != "lightdm") {
//                UserButton *button = getUserByName(pws->pw_name);

//                if (!button)
//                    initOtherUser(pws->pw_name);
            }
        }
    });

    process->start("/opt/pbis/bin/enum-users");

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

//void UserWidget::updateCurrentUserPos(const int duration) const
//{
//    for (UserButton *user : m_userBtns)
//        user->move(rect().center() - user->rect().center(), duration);
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

void UserWidget::onUserChoosed()
{
    // hide buttons
    for (UserButton *ub : m_availableUserButtons)
        ub->hide();

    UserButton *clickedButton = qobject_cast<UserButton *>(sender());
    Q_ASSERT(clickedButton);

    m_currentUser = clickedButton->userInfo();

    emit currentUserChanged(m_currentUser);
}

void UserWidget::setCurrentUser(const QString &username)
{
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
}

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

        userButton->stopAnimation();
        userButton->show();
        userButton->show();
        userButton->setImageSize(UserButton::AvatarSmallSize);
        if (i + 1 <= maxLineCap) {
            // the first line.
            userButton->move(QPoint(offset + i * USER_ICON_WIDTH, 0), 200);
        } else {
            // the second line.
            userButton->move(QPoint(offset + (i - maxLineCap) * USER_ICON_WIDTH, USER_ICON_HEIGHT), 200);
        }
    }

    setFocus();
    grabKeyboard();

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
    setting.beginGroup("USER");
    setting.setValue("USERNAME", username);
    setting.endGroup();
    setting.sync();
}

void UserWidget::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);

    if (isChooseUserMode) {
        // rearrange the user icons.
        expandWidget();
    } else {
//        updateCurrentUserPos();
    }
}

void UserWidget::keyReleaseEvent(QKeyEvent *event)
{
    QFrame::keyReleaseEvent(event);

    switch (event->key()) {
    case Qt::Key_Left:
        leftKeySwitchUser();
        break;
    case Qt::Key_Right:
        rightKeySwitchUser();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
    case Qt::Key_Escape:
//        chooseButtonChecked();
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

void UserWidget::onNativeUserListChanged()
{
    for (const QString &userPath : m_dbusAccounts->userList())
        onNativeUserAdded(userPath);
}

void UserWidget::appendUser(User *user)
{
    UserButton *userButton = new UserButton(user, this);

    m_availableUsers << user;
    m_availableUserButtons << userButton;

    if (!m_currentUser)
        m_currentUser = user;

    connect(userButton, &UserButton::clicked, this, &UserWidget::onUserChoosed);

    // TODO: emit changed signals
}

void UserWidget::leftKeySwitchUser()
{
//    if (isChooseUserMode) {
//        if (!m_currentUserIndex)
//            m_currentUserIndex = m_userBtns.length() - 1;
//        else
//            m_currentUserIndex -= 1;

//        m_currentUser = m_userBtns.at(m_currentUserIndex)->name();

//        for (int i(0); i < m_userBtns.count(); i++)
//            m_userBtns.at(i)->setSelected(i == m_currentUserIndex);
//    }
}

void UserWidget::rightKeySwitchUser()
{
//    if (isChooseUserMode) {
//        if (m_currentUserIndex ==  m_userBtns.length() - 1)
//            m_currentUserIndex = 0;
//        else
//            m_currentUserIndex = m_currentUserIndex + 1;

//        m_currentUser = m_userBtns.at(m_currentUserIndex)->name();

//        for (int i(0); i < m_userBtns.count(); i++)
//            m_userBtns.at(i)->setSelected(i == m_currentUserIndex);
//    }
}

const QString UserWidget::loginUser()
{
    struct passwd *pws;
    pws = getpwuid(getuid());
    return QString(pws->pw_name);
}

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

void UserWidget::setUserKBlayout(const QString &username, const QString &layout)
{
//    UserButton *user = getUserByName(username);
//    if (user && user->dbus())
//        user->dbus()->SetLayout(layout);
}

User::User(QObject *parent) : QObject(parent)
{

}

bool User::operator==(const User &user) const
{
    return type() == user.type() &&
           m_userName == user.m_userName;
}

NativeUser::NativeUser(const QString &path, QObject *parent)
    : User(parent)

    , m_userInter(new UserInter(ACCOUNT_DBUS_SERVICE, path, QDBusConnection::systemBus(), this))
{
}

ADDomainUser::ADDomainUser(QObject *parent)
    : User(parent)
{
}
