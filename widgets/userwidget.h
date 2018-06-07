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

#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>
#include <QFrame>
#include <QScrollArea>

#include <com_deepin_daemon_logined.h>
#include <com_deepin_daemon_accounts_user.h>

#include "dbus/dbusaccounts.h"
#include "userbutton.h"
#include "dbus/dbuslockservice.h"

#define ACCOUNT_DBUS_SERVICE "com.deepin.daemon.Accounts"
#define ACCOUNT_DBUS_PATH "/com/deepin/daemon/Accounts"

using Logined = com::deepin::daemon::Logined;
using UserInter = com::deepin::daemon::accounts::User;

class User : public QObject
{
    Q_OBJECT

public:
    enum UserType
    {
        Native,
        ADDomain,
    };

    User(QObject *parent);

signals:
    void displayNameChanged(const QString &displayname) const;
    void logindChanged(bool islogind) const;
    void avatarChanged(const QString &avatar) const;
    void greeterBackgroundPathChanged(const QString &path) const;
    void desktopBackgroundPathChanged(const QString &path) const;

public:
    bool operator==(const User &user) const;
    const QString name() const { return m_userName; }
    bool isLogin() const { return m_isLogind; }
    uint uid() const { return m_uid; }

    void setisLogind(bool isLogind);
    virtual void setCurrentLayout(const QString &layout) { Q_UNUSED(layout); }

    virtual UserType type() const = 0;
    virtual QString displayName() const { return m_userName; }
    virtual QString avatarPath() const = 0;
    virtual QString greeterBackgroundPath() const = 0;
    virtual QString desktopBackgroundPath() const = 0;
    virtual QStringList kbLayoutList() { return QStringList(); }
    virtual QString currentKBLayout() { return QString(); }

protected:
    bool m_isLogind;
    uint m_uid;
    QString m_userName;
};

class NativeUser : public User
{
    Q_OBJECT

public:
    NativeUser(const QString &path, QObject *parent = nullptr);

    void setCurrentLayout(const QString &currentKBLayout);

    UserType type() const { return Native; }
    QString displayName() const;
    QString avatarPath() const;
    QString greeterBackgroundPath() const;
    QString desktopBackgroundPath() const;
    QStringList kbLayoutList();
    QString currentKBLayout();

    const QString path() const { return m_userPath; }

private:
    UserInter *m_userInter;
    QString m_userPath;
};

class ADDomainUser : public User
{
    Q_OBJECT

public:
    ADDomainUser(int uid, QObject *parent = nullptr);

    void setUserDisplayName(const QString &name);

    UserType type() const { return ADDomain; }
    QString avatarPath() const;
    QString greeterBackgroundPath() const;
    QString desktopBackgroundPath() const;
};

class UserWidget : public QScrollArea
{
    Q_OBJECT
public:
    UserWidget(QWidget* parent = 0);
    ~UserWidget();

    static const QString currentContextUser();
    User *currentUser() const { return m_currentUser; }
    const QList<User *> loginedUsers() const;
    const QList<User *> allUsers() const;

//    inline int count() const {return m_userBtns.count();}
    bool isChooseUserMode = false;
//    const QString getUserAvatar(const QString &username);
    bool getUserIsAutoLogin(const QString &username);
//    const QString getUserGreeterBackground(const QString &username);
//    const QStringList getUserKBHistory(const QString &username);
//    const QString getUserKBLayout(const QString &username);
//    const QString getDisplayName(const QString &username);
//    const QStringList getUserDesktopBackground(const QString &username);
//    const QStringList users() const;
    int availableUserCount() const { return m_availableUsers.size(); }
//    void setUserKBlayout(const QString &username, const QString &layout);

signals:
    void currentUserChanged(User *user);
    void switchToLogindUser(User *user);
    void userChanged(const QString &username);
    void chooseUserModeChanged(bool isChoose, QString curUser);
    void otherUserLogin();
    void currentUserBackgroundChanged(const QString &background);
    void userCountChanged(int count);

public slots:
//    void setCurrentUser(const QString &username);
    void expandWidget();
    void saveLastUser();
    void saveADUser(const QString &username);
    void restoreUser(User *user);
    User *initADLogin();

//    void chooseButtonChecked();

    void appendUser(User *user);

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void initUI();
    void initConnections();
    void removeUser(QString name);
//    void onUserListChanged();
    void onNativeUserAdded(const QString &path);
    void onNativeUserRemoved(const QString &path);
    void onLoginUserListChanged(const QString &loginedUserInfo);
//    UserButton* getUserByName(const QString &username);
//    void initOtherUser(const QString &username = "");

    void addAddomainUser(int uid);
    void onUserChoosed();
    void switchPreviousUser();
    void switchNextUser();

    bool checkHaveDisplay(const QJsonArray &array);

    void updateAllADUserInfo();
    void updateIconPosition();

    void checkADState();
    void findFirstNotLogin();

private:
//    int m_currentUserIndex = 0;
//    QSettings m_settings;
//    QString m_currentUser = QString();
//    QStringList m_whiteList;
    DBusLockService m_lockInter;
//    UserButton* m_currentBtns = nullptr;
//    QList<UserButton *> m_userBtns;
    DBusAccounts *m_dbusAccounts;
//    QMap<QString, UserInter *> m_userDbus;
    Logined *m_dbusLogined;
//    QStringList m_loggedInUsers;
//    UserButton *m_adLoginBtn = nullptr;
    User *m_currentUser;

    // refactor
    QList<User *> m_availableUsers;
    QList<UserButton *> m_availableUserButtons;

    QTimer *m_adCheckStateTimer;
    User *m_adLogin;
    QFrame *m_bgWidget;
};

#endif // WIDGET_H
