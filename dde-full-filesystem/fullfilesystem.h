/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
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

#ifndef FULLFILESYSTEM_H
#define FULLFILESYSTEM_H

#include <QLabel>
#include <QPushButton>
#include <com_deepin_daemon_accounts.h>
#include <com_deepin_daemon_accounts_user.h>
#include <com_deepin_daemon_imageblur.h>
#include <com_deepin_daemon_accounts_user.h>

using Accounts = com::deepin::daemon::Accounts;
using AccountsUser = com::deepin::daemon::accounts::User;
using ImageBlur = com::deepin::daemon::ImageBlur;
using User = com::deepin::daemon::accounts::User;

class FullFilesystem : public QWidget
{
    Q_OBJECT

public:
    FullFilesystem(QWidget *parent = 0);

public Q_SLOTS:
    void setCurrentUser(User *currentUser);

Q_SIGNALS:
    void requestSetBackground(const QString &background);

private Q_SLOTS:
    void onRecoveryButtonClicked();
    void onPoweroffButtonClicked();

private:
    void initUI();
    void initDbus();
    void setBackground(const QString &path);
    void onBlurWallpaperFinished(const QString &source, const QString &blur, bool status);

private:
    User *m_currentUser;
    Accounts *m_accountsInter;
    AccountsUser *m_accountsUserInter;
    ImageBlur *m_blurImageInter;

    QLabel *m_iconLabel;
    QLabel *m_title;
    QLabel *m_message;

    QPushButton *m_recovery;
    QPushButton *m_poweroff;
};

#endif // FULLFILESYSTEM_H
