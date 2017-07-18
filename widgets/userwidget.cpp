/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "userwidget.h"
#include "constants.h"
#include "dbus/dbususer.h"
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
    m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this),
    m_dbusLogined(new Logined("com.deepin.daemon.Accounts", "/com/deepin/daemon/Logined", QDBusConnection::systemBus(), this))
{
    m_currentUser = m_lockInter.CurrentUser();
    qDebug() << Q_FUNC_INFO << m_currentUser;

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFixedWidth(qApp->desktop()->width());

    m_dbusAccounts = new DBusAccounts(ACCOUNT_DBUS_SERVICE,  ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this);

    onUserListChanged();

    m_dbusLogined->setSync(false);
    m_dbusLogined->userList();

    initUI();
    initConnections();
}

UserWidget::~UserWidget()
{
    qDeleteAll(m_userBtns);
}

void UserWidget::initUI()
{
    //Get the username list for the first time!
    countNum = 3;
    getUsernameList();

    QPixmap loading(":/img/action_icons/facelogin_animation.png");
    QSize size(110, 110);
    m_loadingAni = new DLoadingIndicator(this);
    m_loadingAni->setLoading(true);
    m_loadingAni->setSmooth(true);
    m_loadingAni->setAniDuration(600);
    m_loadingAni->setAniEasingCurve(QEasingCurve::Linear);
    m_loadingAni->move(rect().center().x() - UserButton::AvatarLargerSize / 2,
                       rect().center().y() - UserButton::AvatarLargerSize / 2 - 14);
    m_loadingAni->setFixedSize(size);
    m_loadingAni->setImageSource(loading.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    m_loadingAni->hide();

    setCurrentUser(currentUser());

    const int count = m_userBtns.count();
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
    connect(&m_lockInter, &DBusLockService::UserChanged, this, &UserWidget::setCurrentUser);

    connect(m_dbusAccounts, &DBusAccounts::UserListChanged, this, &UserWidget::onUserListChanged);
    connect(m_dbusAccounts, &DBusAccounts::UserAdded, this, &UserWidget::onUserAdded);
    connect(m_dbusAccounts, &DBusAccounts::UserDeleted, this, &UserWidget::onUserRemoved);

    connect(m_dbusLogined, &Logined::UserListChanged, this, &UserWidget::onLoginUserListChanged);
}

QStringList UserWidget::getUsernameList() {
    if (countNum==0||!m_whiteList.isEmpty()) {
        return m_whiteList;
    } else {
        countNum--;
    }

    for (DBusUser *inter : m_userDbus.values()) {
        if (!inter->locked() && !m_whiteList.contains(inter->userName()))
            m_whiteList.append(inter->userName());
    }

//    whiteList = QStringList(whiteList.toSet().toList());
    qDebug() << "getUsernameList:" << m_whiteList;
    return m_whiteList;
}

void UserWidget::onUserListChanged()
{
    for (const QString &name : m_dbusAccounts->userList())
        onUserAdded(name);
}

void UserWidget::onUserAdded(const QString &name)
{
    if (m_userDbus.contains(name))
        return;

    DBusUser *user = new DBusUser(ACCOUNT_DBUS_SERVICE, name, QDBusConnection::systemBus(), this);

    m_userDbus.insert(name, user);

    updateAvatar(user->userName());
}

void UserWidget::onUserRemoved(const QString &name)
{
    DBusUser *user;
    user = m_userDbus.find(name).value();

    if (user) {
        m_userDbus.remove(name);
        user->deleteLater();

        UserButton *button;
        for (int index(0); index != m_userBtns.count(); ++index) {
            button = m_userBtns.at(index);

            if (button) {
                m_userBtns.removeOne(button);
                button->deleteLater();
                return;
            }
        }
    }
}

void UserWidget::onLoginUserListChanged(const QString &value)
{
    QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8());

    QJsonArray jsonArray = doc.array();

    if (jsonArray.isEmpty())
        return;

    for (int i(0); i != jsonArray.count(); ++i) {
        const QJsonObject &obj = jsonArray.at(i).toObject();
        if (obj["Display"].toString().isEmpty())
            continue;
        m_loggedInUsers << obj["Name"].toString();
    }
}

void UserWidget::setCurrentUser(const QString &username)
{
    qDebug() << username << sender();

    const bool isChooseUser = isChooseUserMode;

    m_currentUser = username;
    if (isChooseUser)
        m_lockInter.SwitchToUser(username);

    isChooseUserMode = false;

    for (UserButton *user : m_userBtns) {
        if (user->objectName() == username) {
            user->showButton();
            user->setImageSize(user->AvatarLargerSize);
            user->setButtonChecked(false);
            user->setSelected(false);
            user->show();
        } else
            user->hide(180);

        user->move(rect().center() - user->rect().center(), 200);
    }

    emit userChanged(m_currentUser);
    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
}

void UserWidget::updateAvatar(QString name)
{
    qDebug() << "add user: " << name;

    UserButton *user = new UserButton(name);
    user->hide();
    user->setObjectName(name);
    user->setParent(this);
    user->move(rect().center() - user->rect().center());

    connect(user, &UserButton::imageClicked, this, &UserWidget::setCurrentUser);

    m_userBtns.append(user);
}

void UserWidget::removeUser(QString name)
{
    qDebug() << "remove user: " << name;

    for (int i(0); i < m_userBtns.count(); ++i) {
        if (m_userBtns[i]->name() == name) {
            UserButton *btn = m_userBtns[i];
            m_userBtns.removeAt(i);
            btn->deleteLater();
            break;
        }
    }
}

void UserWidget::expandWidget()
{
    isChooseUserMode = true;

    const int count = m_userBtns.count();
    const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.
    const int offset = (width() - USER_ICON_WIDTH * qMin(count, maxLineCap)) / 2;

    // Adjust size according to user count.
    if (maxLineCap < count) {
        setFixedSize(width(), USER_ICON_HEIGHT * qCeil(count * 1.0 / maxLineCap));
    }

    for (int i = 0; i != count; ++i)
    {
        UserButton *user = m_userBtns.at(i);
        const QString username = user->objectName();

        if (m_loggedInUsers.contains(username)) {
            user->setButtonChecked(true);
        } else {
            user->setButtonChecked(false);
        }

        user->stopAnimation();
        user->show();
        user->showButton();
        user->setImageSize(UserButton::AvatarSmallSize);
        if (i + 1 <= maxLineCap) {
            // the first line.
            user->move(QPoint(offset + i * USER_ICON_WIDTH, 0), 200);
        } else {
            // the second line.
            user->move(QPoint(offset + (i - maxLineCap) * USER_ICON_WIDTH, USER_ICON_HEIGHT), 200);
        }
    }

    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
}

void UserWidget::saveLastUser()
{
    m_lockInter.SwitchToUser(currentUser()).waitForFinished();
}

void UserWidget::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);

    if (isChooseUserMode) {
        // rearrange the user icons.
        expandWidget();
    } else {
        for (UserButton *user : m_userBtns)
            user->move(rect().center() - user->rect().center(), 1);
    }

    m_loadingAni->move(rect().center().x() - UserButton::AvatarLargerSize / 2,
                       rect().center().y() - UserButton::AvatarLargerSize / 2 - 14);
}

void UserWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    for (UserButton* user: m_userBtns) {
        user->updateAvatar();
    }
}

void UserWidget::switchUserByKey(int i, int j) {
    m_userBtns.at(i)->hide(10);
    m_currentUser = m_userBtns.at(j)->objectName();
    m_userBtns.at(j)->setSelected(false);
    m_userBtns.at(j)->show();
    m_userBtns.at(j)->showButton();
    m_userBtns.at(j)->setImageSize(UserButton::AvatarLargerSize);
}

void UserWidget::chooseButtonChecked() {
    qDebug() << "Grab Key Release Event";

    this->grabKeyboard();
    qDebug() << "Get the Key Return or Enter";
    bool checkedBtsExist = false;
    for (UserButton* user: m_userBtns) {
        if (user->selected()) {
            setCurrentUser(user->objectName());
            checkedBtsExist = true;
        }
    }
    if (!checkedBtsExist) {
        setCurrentUser(m_currentUser);
    }
}

void UserWidget::leftKeySwitchUser() {

    if (!isChooseUserMode) {
        for (int i = 0; i < m_userBtns.length(); i++) {
            qDebug() << "zz:" << i << m_userBtns.at(i)->objectName();
            if (m_userBtns.at(i)->objectName() == m_currentUser) {

                if (i == 0) {
                    switchUserByKey(0, m_userBtns.length() - 1);
                    break;
                } else {
                    qDebug() << "$$$" << i;
                    switchUserByKey(i, i - 1);
                    break;
                }
            } else {
                continue;
            }

        }
    } else {
        if (m_currentUserIndex == 0) {
            m_currentUserIndex = m_userBtns.length() - 1;
        } else {
            m_currentUserIndex = m_currentUserIndex - 1;
        }

        for (int j = 0; j < m_userBtns.length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns.at(j)->setSelected(true);
            } else {
                m_userBtns.at(j)->setSelected(false);
            }
        }

    }
}

void UserWidget::rightKeySwitchUser() {
    qDebug() << "RightKeyPressed";
    if (!isChooseUserMode) {
        for (int i = 0; i < m_userBtns.length(); i++) {
            if (m_userBtns.at(i)->objectName() == m_currentUser) {

                if (i == m_userBtns.length() - 1) {
                    switchUserByKey(m_userBtns.length() - 1, 0);
                    break;
                } else {
                    qDebug() << "$$$" << i;
                    switchUserByKey(i, i + 1);
                    break;
                }
            } else {
                continue;
            }
        }
    } else {
        if (m_currentUserIndex ==  m_userBtns.length() - 1) {
            m_currentUserIndex = 0;
        } else {
            m_currentUserIndex = m_currentUserIndex + 1;
        }

        for (int j = 0; j < m_userBtns.length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns.at(j)->setSelected(true);
            } else {
                m_userBtns.at(j)->setSelected(false);
            }
        }
    }
}

const QString UserWidget::loginUser()
{
    struct passwd *pws;
    pws = getpwuid(getuid());
    return QString(pws->pw_name);
}

const QString UserWidget::currentUser()
{
    qDebug() << Q_FUNC_INFO << m_currentUser;

    if (!m_currentUser.isEmpty() && m_whiteList.contains(m_currentUser)) {
        return m_currentUser;
    }

    struct passwd *pws;
    pws = getpwuid(getuid());
    const QString currentLogin(pws->pw_name);

    //except the current-user named lightdm
    if (!currentLogin.isEmpty() && currentLogin!="lightdm")
        return currentLogin;

    if (!m_whiteList.isEmpty())
        return m_whiteList.first();

    // return first user
    if (m_userDbus.count() > 0) {
        const QString tmpUsername = m_userDbus.first()->userName();
        updateAvatar(tmpUsername);
        return tmpUsername;
    }


    //if get the user failed again, start to read dbus for 3 times
    countNum = 3;
    QTimer* mTimer = new QTimer(this);
    mTimer->setInterval(100);
    mTimer->start();
    connect(mTimer,  &QTimer::timeout, this, &UserWidget::getUsernameList);

//    whiteList = QStringList(whiteList.toSet().toList());
//    if (whiteList.length()!=0) {
//        updateAvatar(whiteList[0]);
//        return whiteList[0];
//    }

    qWarning() << "no users !!!";
    return QString();
}

const QString &UserWidget::getUserAvatar(const QString &username)
{
    for (UserButton *button : m_userBtns)
        if (button->name() == username)
            return button->avatar();

    return QString();
}

const QStringList UserWidget::getLoggedInUsers() const
{
    return m_loggedInUsers;
}
