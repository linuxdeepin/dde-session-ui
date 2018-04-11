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

#include <QtGui/QPainter>
#include <QDebug>
#include <QTimer>
#include <QSettings>

#include "userbutton.h"
#include "userwidget.h"

UserButton::UserButton(User *user, QWidget *parent)
    : QPushButton(parent)

    , m_user(user)
    , m_opacity(0)
#ifndef DISABLE_ANIMATIONS
    , m_moveAni(new QPropertyAnimation(this, "pos"))
    , m_showAnimation(new QPropertyAnimation(this, "opacity"))
    , m_hideAnimation(new QPropertyAnimation(this, "opacity"))
#endif
    , m_selected(false)
    , m_loginIconVisible(false)
{
    setVisible(false);

    initUI();
    initConnect();
}

void UserButton::initConnect()
{
#ifndef DISABLE_ANIMATIONS
    connect(m_hideAnimation, &QPropertyAnimation::finished, this, &QPushButton::hide);
#endif
    connect(m_userAvatar, &UserAvatar::clicked, this, &UserButton::click);
    connect(m_user, &User::displayNameChanged, m_userNameLabel, &QLabel::setText);
    connect(m_user, &User::avatarChanged, this, [=] (const QString avatar) {
        m_userAvatar->setIcon(avatar);
    });
}

void UserButton::initUI()
{
    setFixedSize(USER_ICON_WIDTH, USER_ICON_HEIGHT);
    setFocusPolicy(Qt::NoFocus);

    m_userAvatar = new UserAvatar;
    m_userAvatar->setAvatarSize(UserAvatar::AvatarLargeSize);
    m_userAvatar->setFixedSize(120, 120);

    m_userNameLabel = new QLabel;

    QFont font(m_userNameLabel->font());
    font.setPixelSize(16);
    m_userNameLabel->setFont(font);
    m_userNameLabel->setText(m_user->displayName());
    m_userNameLabel->setStyleSheet("QLabel {"
                                   "text-align:center; "
                                   "color: white;"
                                   "}");

    m_userAvatar->setIcon(m_user->avatarPath());

    m_checkedMark = new QLabel;
    m_checkedMark->setFixedSize(16 + 10, 16);
    m_checkedMark->setPixmap(QPixmap(""));
    m_checkedMark->setStyleSheet("background: url(\":/img/avatar_checked.png\"); margin-right: 10");
    m_checkedMark->setVisible(false);

    m_nameLayout = new QHBoxLayout;
    m_nameLayout->setSpacing(0);
    m_nameLayout->setMargin(0);
    m_nameLayout->addStretch();
    m_nameLayout->addWidget(m_checkedMark);
    m_nameLayout->addWidget(m_userNameLabel);
    m_nameLayout->addStretch();

    m_centralLayout = new QVBoxLayout;
    m_centralLayout->setMargin(0);
    m_centralLayout->setSpacing(0);
    m_centralLayout->addWidget(m_userAvatar);
    m_centralLayout->setAlignment(m_userAvatar, Qt::AlignHCenter);
    m_centralLayout->addLayout(m_nameLayout);
    m_centralLayout->addStretch();

    setStyleSheet("QPushButton {"
                  "background-color: transparent;"
                  "border: none;"
                  "}");

    setLayout(m_centralLayout);

    m_opacityEffect = new QGraphicsOpacityEffect;

    connect(this, &UserButton::opacityChanged, &UserButton::setCustomEffect);
}

void UserButton::setImageSize(const AvatarSize &avatarsize) {
    if (avatarsize==AvatarLargerSize) {
        m_userAvatar->setAvatarSize(m_userAvatar->AvatarLargeSize);
    } else {
        m_userAvatar->setAvatarSize(m_userAvatar->AvatarSmallSize);
    }

    m_avatarsize = avatarsize;
    update();
}

void UserButton::show()
{
#ifndef DISABLE_ANIMATIONS
    m_showAnimation->stop();
    m_showAnimation->setStartValue(0.0);
    m_showAnimation->setEndValue(1.0);
    m_showAnimation->setDuration(800);
    m_showAnimation->start();

    QPushButton::show();

//    connect(m_showAnimation, &QPropertyAnimation::finished, [&]{
//        QTimer::singleShot(500, this, SLOT(addTextShadowAfter()));
//    });
#else
    QPushButton::show();
#endif
}

//void UserButton::addTextShadowAfter()
//{
//    m_opacityEffect->setEnabled(false);
//    addTextShadow(true);
//}

//void UserButton::updateUserName(const QString &username)
//{
//     m_username = username;
//     updateUserDisplayName(m_displayName.isEmpty() ? m_username : m_displayName);
//}

//void UserButton::updateAvatar(const QString &avatar)
//{
//    m_avatar = avatar;
//    m_userAvatar->setIcon(avatar);
//}

//void UserButton::updateBackgrounds(const QStringList &list)
//{
//    m_backgrounds = list;
//}

//void UserButton::updateGreeterWallpaper(const QString &greeter)
//{
//    m_greeterWallpaper = greeter;
//}

//void UserButton::updateDisplayName(const QString &displayname)
//{
//    m_displayName = displayname;
//    updateUserDisplayName(m_displayName.isEmpty() ? m_username : m_displayName);
//}

//void UserButton::updateAutoLogin(bool autologin)
//{
//    m_isAutoLogin = autologin;
//}

//void UserButton::updateKbLayout(const QString &layout)
//{
//    m_kbLayout = layout;
//}

//void UserButton::updateKbHistory(const QStringList &history)
//{
//    m_kbHistory = history;
//}

//void UserButton::setDBus(UserInter *dbus)
//{
//    m_dbus = dbus;
//}

void UserButton::hide()
{
#ifndef DISABLE_ANIMATIONS
    m_hideAnimation->setStartValue(1);
    m_hideAnimation->setEndValue(0);
    m_hideAnimation->start();
#else
    QPushButton::hide();
#endif

#ifndef DISABLE_TEXT_SHADOW
    addTextShadow(false);
#endif

    m_opacityEffect->setEnabled(true);
}

void UserButton::move(const QPoint &position, bool immediately)
{
#ifndef DISABLE_ANIMATIONS
    const bool play_ani = !immediately;

    if (play_ani)
    {
        m_moveAni->stop();
        m_moveAni->setDuration(200);
        m_moveAni->setStartValue(pos());
        m_moveAni->setEndValue(position);

        m_moveAni->start();
    } else {
        QPushButton::move(position);
    }
#else
   Q_UNUSED(immediately);
   QPushButton::move(position);
#endif
}

void UserButton::addTextShadow(bool isEffective)
{
#ifndef DISABLE_TEXT_SHADOW
    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    nameShadow->setEnabled(isEffective);
    m_userNameLabel->setGraphicsEffect(nameShadow);
#endif
}

bool UserButton::selected() const
{
    return m_selected;
}

void UserButton::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

//const QString UserButton::name() const
//{
//    return m_username;
//}

//const QString UserButton::avatar() const
//{
//    return m_avatar;
//}

//const QString UserButton::greeter() const
//{
//    return m_greeterWallpaper;
//}

//bool UserButton::automaticLogin() const
//{
//    return m_isAutoLogin;
//}

//const QStringList UserButton::kbHistory()
//{
//    return m_kbHistory;
//}

//const QString UserButton::kblayout()
//{
//    return m_kbLayout;
//}

//const QString UserButton::displayName() const
//{
//    return m_displayName;
//}

//const QStringList UserButton::backgrounds() const
//{
//    return m_backgrounds;
//}

//UserInter *UserButton::dbus() const
//{
//    return m_dbus;
//}

void UserButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);

    if (!m_selected)
        return;

    QPainter painter(this);
    painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
    painter.setBrush(QColor(0, 0 , 0, 76));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRoundedRect(QRect(2, 2, width() - 4, height() - 4), 10, 10, Qt::RelativeSize);
}

void UserButton::stopAnimation()
{
#ifndef DISABLE_ANIMATIONS
    m_moveAni->stop();
#endif
}

double UserButton::opacity() {
    return m_opacity;
}

void UserButton::setOpacity(double opa) {
    if (m_opacity != opa) {
        m_opacity = opa;
        emit opacityChanged();
    }
}

void UserButton::setCustomEffect() {
    m_opacityEffect->setOpacity(m_opacity);
    setGraphicsEffect(m_opacityEffect);
}

void UserButton::setLoginIconVisible(bool checked) {
    m_checkedMark->setVisible(checked && m_user->isLogin());
}

UserButton::~UserButton()
{}
