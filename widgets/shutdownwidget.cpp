/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

    m_requireRestartButton = new RoundItemButton(tr("Restart"), this);
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

ShutdownWidget::~ShutdownWidget() {
}
