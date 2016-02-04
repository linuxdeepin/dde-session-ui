/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
