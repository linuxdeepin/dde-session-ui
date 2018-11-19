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

LoginWindow::LoginWindow(SessionBaseModel * const model, QWidget *parent)
    : FullscreenBackground(parent)
    , m_loginFrame( new LockContent(model, this))
{
    m_loginFrame->setLeftBottomWidget(new LogoWidget);
    setContent(m_loginFrame);


    connect(m_loginFrame, &LockContent::requestBackground, this, [=] (const QString &wallpaper) {
        updateBackground(wallpaper);
    });

    connect(model, &SessionBaseModel::authFinished, this, [=] (bool successd) {
        m_loginFrame->setVisible(!successd);
    });

    connect(m_loginFrame, &LockContent::requestAuthUser, this, &LoginWindow::requestAuthUser);
    connect(m_loginFrame, &LockContent::requestSwitchToUser, this, &LoginWindow::requestSwitchToUser);
}
