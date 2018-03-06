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
#include "app/loginmanager.h"

LoginWindow::LoginWindow(QWidget *parent)
    : FullscreenBackground(parent)
    , m_loginFrame( new LoginManager(this))
{
    setContent(m_loginFrame);

    connect(m_loginFrame, static_cast<void (LoginManager::*)(const QString &) const>(&LoginManager::requestBackground),
            this, &LoginWindow::switchUserBackground);
    connect(m_loginFrame, static_cast<void (LoginManager::*)(const QPixmap &) const>(&LoginManager::requestBackground),
            this, &LoginWindow::drawUserBackground);
}

void LoginWindow::switchUserBackground(const QString &path)
{
    setBackground(path, FakeBackground::FadeIn);
}

// NOTE(kirigaya): Setting the animation type
void LoginWindow::drawUserBackground(const QPixmap &pixmap)
{
    setBackground(pixmap, FakeBackground::FadeOut);
}
