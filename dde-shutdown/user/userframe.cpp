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

#include "userframe.h"

UserFrame::UserFrame(QWidget *parent)
    : QFrame(parent)
{
    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addSpacing(0);

    addUser(QString(":img/user/kakaxi.png"), "administrator");
    setLayout(m_Layout);
}
UserFrame::~UserFrame()
{}
void UserFrame::addUser(QString url, QString name) {
    ImageButton* add_user = new ImageButton(url, name);
    QLabel* add_userName = new QLabel;
    add_user->setText(name);
    userList << name;
    m_Layout->addWidget(add_user);
    m_Layout->addWidget(add_userName);
    connect(add_user, SIGNAL(clicked(QString)),
            this, SIGNAL(selectedUser(QString)));
}
