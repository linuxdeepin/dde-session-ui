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

#include "fullfilesystem.h"
#include "util_updateui.h"

#include <QVBoxLayout>
#include <QPushButton>

const int ButtonWidth = 280;
const int ButtonHeight = 30;

FullFilesystem::FullFilesystem(QWidget *parent)
    : QWidget(parent)
    , m_currentUser(0)
    , m_iconLabel(new QLabel("icon", this))
    , m_title(new QLabel("sorry, couldn't get into the desktop", this))
    , m_message(new QLabel("your disk space is full, please enter recovery mode to clean up the disk!", this))
    , m_recovery(new QPushButton("reboot to recovery mode", this))
    , m_poweroff(new QPushButton("poweroff", this))
{
    initUI();

    QTimer::singleShot(1, this, &FullFilesystem::initDbus);
}

void FullFilesystem::setCurrentUser(User *currentUser)
{
    m_currentUser = currentUser;

    setBackground(m_currentUser->greeterBackgroundPath());
}

void FullFilesystem::onRecoveryButtonClicked()
{
    qApp->quit();
}

void FullFilesystem::onPoweroffButtonClicked()
{

}

void FullFilesystem::initUI()
{
    m_iconLabel->setObjectName("IconLabel");
    m_title->setObjectName("TitleLabel");
    m_message->setObjectName("MsgLabel");
    m_recovery->setObjectName("Recovery");
    m_poweroff->setObjectName("Poweroff");

    m_recovery->setFixedSize(ButtonWidth, ButtonHeight);
    m_poweroff->setFixedSize(ButtonWidth, ButtonHeight);

    QVBoxLayout *mainVBLayout = new QVBoxLayout(this);
    QVBoxLayout *labelsVBLayout = new QVBoxLayout;
    QVBoxLayout *buttonsVBLayout = new QVBoxLayout;

    labelsVBLayout->addWidget(m_iconLabel, 0, Qt::AlignCenter);
    labelsVBLayout->addWidget(m_title, 0, Qt::AlignCenter);
    labelsVBLayout->addWidget(m_message, 0, Qt::AlignCenter);
    labelsVBLayout->setSpacing(20);

    buttonsVBLayout->addWidget(m_recovery, 0, Qt::AlignCenter);
    buttonsVBLayout->addSpacing(20);
    buttonsVBLayout->addWidget(m_poweroff, 0, Qt::AlignCenter);

    mainVBLayout->addStretch(4);
    mainVBLayout->addLayout(labelsVBLayout);
    mainVBLayout->addSpacing(200);
    mainVBLayout->addLayout(buttonsVBLayout);
    mainVBLayout->addStretch(1);

    setLayout(mainVBLayout);

    connect(m_recovery, &QPushButton::clicked, this, &FullFilesystem::onRecoveryButtonClicked);
    connect(m_poweroff, &QPushButton::clicked, this, &FullFilesystem::onPoweroffButtonClicked);

    updateStyle("://theme/fullfilesystem.qss", this);
}

void FullFilesystem::initDbus()
{
    m_accountsInter = new Accounts("com.deepin.daemon.Accounts", "/com/deepin/daemon/Accounts",
                                     QDBusConnection::systemBus(), this);

    m_accountsUserInter = new AccountsUser("com.deepin.daemon.Accounts", m_accountsInter->userList().first(),
                                     QDBusConnection::systemBus(), this);

    m_blurImageInter = new ImageBlur("com.deepin.daemon.Accounts", "/com/deepin/daemon/ImageBlur",
                                     QDBusConnection::systemBus(), this);


    setBackground(m_accountsUserInter->desktopBackgrounds().at(0));

    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &FullFilesystem::onBlurWallpaperFinished);
}

void FullFilesystem::setBackground(const QString &path)
{
    QString mPath = path;
    QUrl url(mPath);

    if (url.isLocalFile()) {
        mPath = url.toLocalFile();
    }

    QString wallpaper = m_blurImageInter->Get(mPath);

#ifdef QT_DEBUG
    qDebug() << wallpaper;
#endif

    Q_EMIT requestSetBackground(wallpaper.isEmpty() ? mPath : wallpaper);
}

void FullFilesystem::onBlurWallpaperFinished(const QString &source, const QString &blur, bool status)
{
    Q_UNUSED(source);

    if (status)
        Q_EMIT requestSetBackground(blur);
}
