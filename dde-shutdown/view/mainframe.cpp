/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "mainframe.h"
#include <QtCore/QObject>

MainFrame::MainFrame(QWidget* parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

void MainFrame::initConnect() {
    connect(this, SIGNAL(OutKeyLeft()), this ,SLOT(DirectLeft()));
    connect(this, SIGNAL(OutKeyRight()), this, SLOT(DirectRight()));
    connect(this, SIGNAL(pressEnterAction()), this, SLOT(EnterAction()));
}

void MainFrame::initUI() {
    m_shutdownFrame = new ShutDownFrame;

    m_contentLayout = new QVBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_contentLayout->addStretch();
    m_contentLayout->addWidget(m_shutdownFrame);
    m_contentLayout->addStretch();

    m_Layout = new QHBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_Layout->addLayout(m_contentLayout);

    setLayout(m_Layout);
}

void MainFrame::changeView(bool a) {
    qDebug() << "clicked" << a;
}
void MainFrame::DirectLeft() {
    emit m_shutdownFrame->keyLeft();
}
void MainFrame::DirectRight() {
    emit m_shutdownFrame->keyRight();
}
void MainFrame::EnterAction() {
    emit m_shutdownFrame->pressEnterAction();
}

void MainFrame::hideBtns(const QStringList &btnsName)
{
    m_shutdownFrame->hideBtns(btnsName);
}

void MainFrame::disableBtns(const QStringList &btnsName)
{
    m_shutdownFrame->disableBtns(btnsName);
}
MainFrame::~MainFrame()
{}

void MainFrame::setConfrim(const bool confirm)
{
    m_shutdownFrame->setConfirm(confirm);
}

void MainFrame::powerAction(const Actions action)
{
    m_shutdownFrame->powerAction(action);
}
