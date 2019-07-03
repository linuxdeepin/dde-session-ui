/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#include "loginwindow.h"
#include "lockcontent.h"
#include "view/logowidget.h"
#include "userinfo.h"

LoginWindow::LoginWindow(SessionBaseModel * const model, QWidget *parent)
    : FullscreenBackground(parent)
    , m_loginFrame( new LockContent(model, this))
{
    LogoWidget *logoWidget = new LogoWidget;
    m_loginFrame->setLeftBottomWidget(logoWidget);
    setContent(m_loginFrame);
    m_loginFrame->hide();

    connect(m_loginFrame, &LockContent::requestBackground, this, [=] (const QString &wallpaper) {
        updateBackground(wallpaper);
#ifdef DISABLE_LOGIN_ANI
        // 在认证成功以后会通过更改背景来实现登录动画，但是禁用登录动画的情况下，会立即调用startSession，
        // 导致当前进程被lightdm退掉，X上会残留上一帧的画面，可以看到输入框等画面。使用repaint()强制刷新背景来避免这个问题。
        repaint();
#endif
    });

    connect(model, &SessionBaseModel::authFinished, this, [=] (bool successd) {
        m_loginFrame->setVisible(!successd);
#ifdef DISABLE_LOGIN_ANI
        // 在认证成功以后会通过更改背景来实现登录动画，但是禁用登录动画的情况下，会立即调用startSession，
        // 导致当前进程被lightdm退掉，X上会残留上一帧的画面，可以看到输入框等画面。使用repaint()强制刷新背景来避免这个问题。
        repaint();
#endif
    });

    connect(m_loginFrame, &LockContent::requestAuthUser, this, &LoginWindow::requestAuthUser);
    connect(m_loginFrame, &LockContent::requestSwitchToUser, this, &LoginWindow::requestSwitchToUser);
    connect(m_loginFrame, &LockContent::requestSetLayout, this, &LoginWindow::requestSetLayout);

    connect(model, &SessionBaseModel::currentUserChanged, this, [=] (std::shared_ptr<User> user) {
        if (user.get()) {
            logoWidget->updateLocale(user->locale().split(".").first());
        }
    });
}
