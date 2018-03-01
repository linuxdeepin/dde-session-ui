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

#include "shutdownwidget.h"

ShutdownWidget::ShutdownWidget(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

void ShutdownWidget::initConnect() {

    connect(m_requireShutdownButton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireShutdown);});
    connect(m_requireRestartButton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireRestart);});
    connect(m_requireSuspendBUtton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireSuspend);});
    connect(this, &ShutdownWidget::directLeft, &ShutdownWidget::leftKeySwitch);
    connect(this, &ShutdownWidget::directRight, &ShutdownWidget::rightKeySwitch);
}

void ShutdownWidget::initUI() {
    m_requireShutdownButton = new RoundItemButton(tr("Shut down"), this);
    m_requireShutdownButton->setObjectName("RequireShutdownButton");
    m_requireShutdownButton->setAutoExclusive(true);

    m_requireRestartButton = new RoundItemButton(tr("Reboot"), this);
    m_requireRestartButton->setObjectName("RequireRestartButton");
    m_requireRestartButton->setAutoExclusive(true);

    m_requireSuspendBUtton = new RoundItemButton(tr("Suspend"), this);
    m_requireSuspendBUtton->setObjectName("RequireSuspendButton");
    m_requireSuspendBUtton->setAutoExclusive(true);

    m_currentSelectedBtn = m_requireShutdownButton;
    m_currentSelectedBtn->updateState(RoundItemButton::Default);

    m_btnList = new QList<RoundItemButton*>;
    m_btnList->append(m_requireShutdownButton);
    m_btnList->append(m_requireRestartButton);
    m_btnList->append(m_requireSuspendBUtton);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(10);
    m_Layout->addStretch(0);
    m_Layout->addWidget(m_requireShutdownButton);
    m_Layout->addWidget(m_requireRestartButton);
    m_Layout->addWidget(m_requireSuspendBUtton);
    m_Layout->addStretch(0);
    setLayout(m_Layout);

    updateStyle(":/skin/requireshutdown.qss", this);
}

void ShutdownWidget::leftKeySwitch() {
    m_btnList->at(m_index)->updateState(RoundItemButton::Normal);
    if (m_index == 0) {
        m_index = m_btnList->length() - 1;
    } else {
        m_index  -= 1;
    }
    m_currentSelectedBtn = m_btnList->at(m_index);
    m_currentSelectedBtn->updateState(RoundItemButton::Checked);
}

void ShutdownWidget::rightKeySwitch() {
    m_btnList->at(m_index)->updateState(RoundItemButton::Normal);
    if (m_index == m_btnList->length() - 1) {
        m_index =  0;
    } else {
        m_index  += 1;
    }
    m_currentSelectedBtn = m_btnList->at(m_index);
    m_currentSelectedBtn->updateState(RoundItemButton::Checked);
}

void ShutdownWidget::shutdownAction() {
    qDebug() << "emit m_currentSelectedBtn clicked";
    emit m_currentSelectedBtn->clicked();
}

void ShutdownWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    QTimer::singleShot(300, this, &ShutdownWidget::grabKeyboard);
}

void ShutdownWidget::hideEvent(QHideEvent *event)
{
    QFrame::hideEvent(event);

    QTimer::singleShot(1, this, &ShutdownWidget::releaseKeyboard);
}

void ShutdownWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        hide();
        emit abortOperation();
        break;
    case Qt::Key_Left:
        leftKeySwitch();
        break;
    case Qt::Key_Right:
        rightKeySwitch();
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        shutdownAction();
        break;
    default:
        break;
    }


    QFrame::keyReleaseEvent(event);
}

ShutdownWidget::~ShutdownWidget() {
}
