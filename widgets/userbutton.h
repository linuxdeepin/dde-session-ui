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

#ifndef USERBUTTON_H
#define USERBUTTON_H

#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPushButton>
#include <QtGui/QPaintEvent>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>
#include <com_deepin_daemon_accounts_user.h>
#include <memory>

#include "useravatar.h"

using UserInter = com::deepin::daemon::accounts::User;

static const int USER_ICON_WIDTH = 180;
static const int USER_ICON_HEIGHT = 180;

class User;
class UserButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:
    UserButton(std::shared_ptr<User> user, QWidget* parent=0);
    ~UserButton();

    enum AvatarSize {
        AvatarSmallSize = 90,
        AvatarLargerSize = 110,
    };

    bool selected() const;
    void setSelected(bool selected);

    std::shared_ptr<User> userInfo() const { return m_user; }

signals:
    void opacityChanged();

public slots:
    void setImageSize(const AvatarSize &avatarsize);

    void show();
    void hide();
    void move(const QPoint &position, bool immediately = false);
    void stopAnimation();

    double opacity();
    void setOpacity(double opa);
    void setCustomEffect();
    void addTextShadowAfter();

protected:
    void paintEvent(QPaintEvent* event);

private:
    void initUI();
    void initConnect();
    void addTextShadow(bool isEffective);

private:
    std::shared_ptr<User> m_user;

    bool m_selected;
    UserAvatar* m_userAvatar;
    QLabel* m_userNameLabel;
    QLabel *m_checkedMark;
    QHBoxLayout* m_buttonLayout;
    QHBoxLayout *m_nameLayout;
    QVBoxLayout* m_centralLayout;

    AvatarSize m_avatarsize = AvatarLargerSize;
    int m_borderWidth = 0;

    double m_opacity;

#ifndef DISABLE_ANIMATIONS
    QPropertyAnimation *m_moveAni;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
#endif

    QGraphicsOpacityEffect* m_opacityEffect;
};
#endif // UserButton

