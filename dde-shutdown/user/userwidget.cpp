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

#include "userwidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>

UserWidget::UserWidget(QWidget *parent)
    : QWidget(parent),
      m_showAnimation(new QPropertyAnimation),
      m_hideAnimation(new QPropertyAnimation)
{
    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->addSpacing(0);

    m_showAnimation->setPropertyName("geometry");
    m_showAnimation->setTargetObject(this);
    m_hideAnimation->setPropertyName("geometry");
    m_hideAnimation->setTargetObject(this);
    this->setFixedSize(USER_ICON_SIZE,USER_ICON_SIZE);


    QRect desktopRect = qApp->desktop()->availableGeometry();
    m_finalX = (desktopRect.width()- USER_ICON_SIZE)/2;
    m_finalY = (desktopRect.height()- USER_ICON_SIZE)/2 - 150;
    this->move(m_finalX, m_finalY);

    addUser(QString(":img/user/kakaxi.png"), "administrator");
//    addUser(QString(":img/mingren.png"), "demo");
//    addUser(QString(":img/mingren.png"), "test");
//    addUser(QString(":img/mingren.png"), "tom");
//    addUser(QString(":img/mingren.png"), "linda");
    setLayout(m_Layout);
    setStyleSheet("background-color: #66CDAA");

    QList<ImageButton*> m_children =
            this->findChildren<ImageButton *>();
    for(int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->objectName() != m_currentUser) {
            m_children[i]->hide();
        } else {
            m_children[i]->setIconSize(ImageButton::AvatarSmallSize);
        }
    }

    showWidget();
    initConnect();
}
UserWidget::~UserWidget()
{
}
void UserWidget::initConnect() {
    connect(this, SIGNAL(selectedUser(QString)),
            SLOT(loginInUser(QString)));

    connect(m_showAnimation, &QPropertyAnimation::finished,
            [=]{
        this->update();
    });
    connect(m_hideAnimation, &QPropertyAnimation::finished,
            [=]{
        this->update();
    });
}
void UserWidget::addUser(QString url, QString name) {
    ImageButton* add_user = new ImageButton(url, name);
    QLabel* add_username = new QLabel;
    add_username->setText(name);
    userList << name;
    m_Layout->addWidget(add_user);
    m_Layout->addWidget(add_username);
    connect(add_user, SIGNAL(clicked(QString)),
            this, SIGNAL(selectedUser(QString)));
}
void UserWidget::loginInUser(QString nam) {

     hideWidget();

     QList<ImageButton*> m_children =
             this->findChildren<ImageButton *>();
     for(int i = 0; i < m_children.length(); i++) {
         if (m_children[i]->objectName()!=nam) {
             m_children[i]->hide();
         } else {
             m_children[i]->show();
             m_children[i]->setIconSize(ImageButton::AvatarBigSize);
         }
     }

}
void UserWidget::setGeometry(const QRect &rect) {
    this->move(rect.x(), rect.y());
    this->setFixedSize(rect.width(), rect.height());
}
void UserWidget::showWidget() {
    QRect rectBegin = QRect(m_finalX, m_finalY,
                            USER_ICON_SIZE, USER_ICON_SIZE);
    m_showAnimation->setStartValue(rectBegin);
    QRect desktopRect = qApp->desktop()->availableGeometry();
    int m_endWidth = userList.length()*USER_ICON_SIZE;
    QRect rectEnd = QRect((desktopRect.width()-m_endWidth)/2,
               m_finalY,  m_endWidth, USER_ICON_SIZE);
    m_showAnimation->setEndValue(rectEnd);
    m_showAnimation->start();

    QList<ImageButton*> m_children =
            this->findChildren<ImageButton *>();
    for(int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->objectName()!="administrator") {
            m_children[i]->show();
        } else {
            m_children[i]->setIconSize(ImageButton::AvatarSmallSize);
        }
    }

}
void UserWidget::hideWidget() {
    QRect rectBegin = QRect(this->x(), m_finalY,
                            this->width(), USER_ICON_SIZE);
    m_hideAnimation->setStartValue(rectBegin);
    QRect rectEnd = QRect(m_finalX, m_finalY, USER_ICON_SIZE, USER_ICON_SIZE);
    m_hideAnimation->setEndValue(rectEnd);
    m_hideAnimation->start();

    QList<ImageButton*> m_children =
            this->findChildren<ImageButton *>();
    for(int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->objectName()!= "administrator") {
            m_children[i]->hide();
        } else {
            m_children[i]->setIconSize(ImageButton::AvatarBigSize);
        }
    }
}
//void UserWidget::keyPressEvent(QKeyEvent *e) {
//    Q_UNUSED(e);
//    if (e->key()==Qt::Key_Escape) {
//        qApp->quit();
//    }
//}
