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
#include "dbus/dbusaccounts.h"
#include "dbus/dbususer.h"
#include "dbus/dbuslockservice.h"
#include "accountsutils.h"

#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QSettings>

#include <unistd.h>
#include <pwd.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/lock"
#define LOCKSERVICE_NAME "com.deepin.dde.lock"

DWIDGET_USE_NAMESPACE

UserWidget::UserWidget(QWidget* parent)
    : UserWidget("", parent) {

}

UserWidget::UserWidget(const QString &username, QWidget* parent)
    : QFrame(parent),
    m_currentUser(),
    m_userBtns(new QList<UserButton *>),
    m_userModel(new QLightDM::UsersModel(this))
{
    DBusLockService m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);
    m_currentUser = username.isEmpty() ? m_lockInter.CurrentUser() : username;

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setStyleSheet("background-color:red;");
//    setFixedSize(qApp->desktop()->width(), USER_ICON_HEIGHT);
    setFixedWidth(qApp->desktop()->width());
//    move(0, (qApp->desktop()->height() - rect().height()) / 2 - 95);

    initUI();
}

UserWidget::~UserWidget()
{

}

void UserWidget::initUI()
{
    //Get the username list for the first time!
    countNum = 3;
    getUsernameList();


    qDebug() << "whiteList:             " << m_whiteList;

    const int userCount = m_userModel->rowCount(QModelIndex());
    for (int i(0); i != userCount; ++i)
    {
        const QString &username = m_userModel->data(m_userModel->index(i), QLightDM::UsersModel::NameRole).toString();

        // pass blocked account
        if (!m_whiteList.contains(username))
            continue;

        updateAvatar(username);
    }

    qDebug() << "whiteList: " << m_whiteList;
    QPixmap loading(":/img/facelogin_animation.png");
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

    const int count = m_userBtns->count();
    const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.

    // Adjust size according to user count.
    if (maxLineCap < count) {
        setFixedSize(width(), USER_ICON_HEIGHT * qCeil(count * 1.0 / maxLineCap));
    } else {
        setFixedHeight(USER_ICON_HEIGHT);
    }
}

void UserWidget::updateAvatar(QString username) {
    const QString path = AccountsUtils::GetUserAvatar(username);
    if (!path.isEmpty())
        addUser(path, username);
    else
        addUser("/var/lib/AccountsService/icons/default.png", username);
}

QStringList UserWidget::getUsernameList() {
    if (countNum==0||!m_whiteList.isEmpty()) {
        return m_whiteList;
    } else {
        countNum--;
    }

    DBusAccounts *accounts = new DBusAccounts(ACCOUNT_DBUS_SERVICE,  ACCOUNT_DBUS_PATH, QDBusConnection::systemBus(), this);
    const QStringList userList = accounts->userList();


    for (const QString &user : userList)
    {
        DBusUser *inter = new DBusUser(ACCOUNT_DBUS_SERVICE, user, QDBusConnection::systemBus(), this);

        if (!inter->locked() && !m_whiteList.contains(inter->userName()))
             m_whiteList.append(inter->userName());
        inter->deleteLater();
    }
    accounts->disconnect();
    accounts->deleteLater();
//    whiteList = QStringList(whiteList.toSet().toList());
    qDebug() << "getUsernameList:" << m_whiteList;
    return m_whiteList;
}

void UserWidget::setCurrentUser(const QString &username)
{
    isChooseUserMode = false;

    for (UserButton *user : *m_userBtns) {
        if (user->objectName() == username) {
            user->showButton();
            user->setImageSize(user->AvatarLargerSize);
            if (user->isChecked())
                user->setButtonChecked(false);
            user->show();
        } else
            user->hide(180);

        user->move(rect().center() - user->rect().center(), 200);
    }

    m_currentUser = username;

    emit userChanged(username);
    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
}

void UserWidget::addUser(QString avatar, QString name) {
    UserButton *user = new UserButton(avatar, name);
    user->hide();
    user->setObjectName(name);
    user->setParent(this);
    user->move(rect().center() - user->rect().center());

    connect(user, &UserButton::imageClicked, this, &UserWidget::setCurrentUser);

    m_userBtns->append(user);
}

void UserWidget::expandWidget()
{
    isChooseUserMode = true;

    const int count = m_userBtns->count();
    const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.
    const int offset = (width() - USER_ICON_WIDTH * qMin(count, maxLineCap)) / 2;
    const QString username = currentUser();

    // Adjust size according to user count.
    if (maxLineCap < count) {
        setFixedSize(width(), USER_ICON_HEIGHT * qCeil(count * 1.0 / maxLineCap));
    }

    for (int i = 0; i != count; ++i)
    {
        if (m_userBtns->at(i)->objectName() != username) {
             m_userBtns->at(i)->showButton();
             m_userBtns->at(i)->setButtonChecked(false);
        }
        if (m_userBtns->at(i)->objectName() == username) {
             m_userBtns->at(i)->setButtonChecked(true);
        }
        m_userBtns->at(i)->stopAnimation();

        m_userBtns->at(i)->show();
        m_userBtns->at(i)->setImageSize(UserButton::AvatarSmallSize);
        if (i + 1 <= maxLineCap) {
            // the first line.
            m_userBtns->at(i)->move(QPoint(offset + i * USER_ICON_WIDTH, 0), 200);
        } else {
            // the second line.
            m_userBtns->at(i)->move(QPoint(offset + (i - maxLineCap) * USER_ICON_WIDTH, USER_ICON_HEIGHT), 200);
        }
    }

    emit chooseUserModeChanged(isChooseUserMode, m_currentUser);
}

void UserWidget::saveLastUser()
{
    DBusLockService m_lockInter(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);
    m_currentUser = m_lockInter.SwitchToUser(currentUser());
}

void UserWidget::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);

    if (isChooseUserMode) {
        // rearrange the user icons.
        expandWidget();
    } else {
        for (UserButton *user : *m_userBtns)
            user->move(rect().center() - user->rect().center(), 1);
    }

    m_loadingAni->move(rect().center().x() - UserButton::AvatarLargerSize / 2,
                       rect().center().y() - UserButton::AvatarLargerSize / 2 - 14);
}

void UserWidget::switchUserByKey(int i, int j) {
    m_userBtns->at(i)->hide(10);
    m_currentUser = m_userBtns->at(j)->objectName();
    m_userBtns->at(j)->show();
    m_userBtns->at(j)->showButton();
    m_userBtns->at(j)->setImageSize(UserButton::AvatarLargerSize);
}

void UserWidget::chooseButtonChecked() {
    qDebug() << "Grab Key Release Event";

    this->grabKeyboard();
    qDebug() << "Get the Key Return or Enter";
    bool checkedBtsExist = false;
    for (UserButton* user: *m_userBtns) {
        if (user->isChecked()) {
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
        for (int i = 0; i < m_userBtns->length(); i++) {
            qDebug() << "zz:" << i << m_userBtns->at(i)->objectName();
            if (m_userBtns->at(i)->objectName() == m_currentUser) {

                if (i == 0) {
                    switchUserByKey(0, m_userBtns->length() - 1);
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
            m_currentUserIndex = m_userBtns->length() - 1;
        } else {
            m_currentUserIndex = m_currentUserIndex - 1;
        }

        for (int j = 0; j < m_userBtns->length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns->at(j)->setButtonChecked(true);
            } else {
                m_userBtns->at(j)->setButtonChecked(false);
            }
        }

    }
}

void UserWidget::rightKeySwitchUser() {
    qDebug() << "RightKeyPressed";
    if (!isChooseUserMode) {
        for (int i = 0; i < m_userBtns->length(); i++) {
            if (m_userBtns->at(i)->objectName() == m_currentUser) {

                if (i == m_userBtns->length() - 1) {
                    switchUserByKey(m_userBtns->length() - 1, 0);
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
        if (m_currentUserIndex ==  m_userBtns->length() - 1) {
            m_currentUserIndex = 0;
        } else {
            m_currentUserIndex = m_currentUserIndex + 1;
        }

        for (int j = 0; j < m_userBtns->length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns->at(j)->setButtonChecked(true);
            } else {
                m_userBtns->at(j)->setButtonChecked(false);
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
    qDebug() << "currentUser:" << m_currentUser;

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
    if (m_userModel->rowCount(QModelIndex()) > 0) {
        QString tmpUsername = m_userModel->data(m_userModel->index(0), QLightDM::UsersModel::NameRole).toString();
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
