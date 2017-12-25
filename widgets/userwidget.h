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

#include <com_deepin_daemon_logined.h>
#include <com_deepin_daemon_accounts_user.h>

#include "dbus/dbusaccounts.h"
#include "userbutton.h"
#include "dbus/dbuslockservice.h"

#define ACCOUNT_DBUS_SERVICE "com.deepin.daemon.Accounts"
#define ACCOUNT_DBUS_PATH "/com/deepin/daemon/Accounts"

using Logined = com::deepin::daemon::Logined;
using DBusUser = com::deepin::daemon::accounts::User;

class UserWidget : public QFrame
{
    Q_OBJECT
public:
    UserWidget(QWidget* parent = 0);
    ~UserWidget();

    static const QString loginUser();
    const QString currentUser();
    inline int count() const {return m_userBtns.count();}
    bool isChooseUserMode = false;
    const QString getUserAvatar(const QString &username);
    const QStringList getLoggedInUsers() const;
    bool getUserIsAutoLogin(const QString &username);
    const QString getUserGreeterBackground(const QString &username);
    const QStringList getUserKBHistory(const QString &username);
    const QString getUserKBLayout(const QString &username);
    const QString getDisplayName(const QString &username);
    const QStringList getUserDesktopBackground(const QString &username);
    const QStringList users() const;
    void setUserKBlayout(const QString &username, const QString &layout);

signals:
    void userChanged(const QString &username);
    void chooseUserModeChanged(bool isChoose, QString curUser);
    void otherUserLogin();
    void currentUserBackgroundChanged(const QString &background);
    void currentUserNameChanged(const QString &background);
    void userCountChanged(int count);

public slots:
    void updateCurrentUser(const QString &username);
    void setCurrentUser(const QString &username);
    void expandWidget();
    void saveLastUser();

    void leftKeySwitchUser();
    void rightKeySwitchUser();
    void chooseButtonChecked();

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void initUI();
    void initConnections();
    void removeUser(QString name);
    void onUserListChanged();
    void onUserAdded(const QString &path);
    void onUserRemoved(const QString &name);
    void onLoginUserListChanged(const QString &value);
    UserButton* getUserByName(const QString &username);
    void updateCurrentUserPos(const int duration = 0) const;
    void initOtherUser(const QString &username = "");
    void initADLogin();

private:
    int m_currentUserIndex = 0;
    QSettings m_settings;
    QString m_currentUser = QString();
    QStringList m_whiteList;
    DBusLockService m_lockInter;
    UserButton* m_currentBtns = nullptr;
    QList<UserButton *> m_userBtns;
    DBusAccounts *m_dbusAccounts;
    QMap<QString, DBusUser *> m_userDbus;
    Logined *m_dbusLogined;
    QStringList m_loggedInUsers;
    UserButton *m_adLoginBtn = nullptr;
};

#endif // WIDGET_H
