/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QHBoxLayout>

#include "controlwidget.h"

DWIDGET_USE_NAMESPACE

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mprisWidget = new DMPRISControl;
    m_mprisWidget->setFixedWidth(200);
    m_mprisWidget->setStyleSheet("color:white;"
                              "font-size:15px;");

    m_userswitch = new DImageButton;
    m_userswitch->setNormalPic(":/img/bottom_actions/userswitch_normal.png");
    m_userswitch->setHoverPic(":/img/bottom_actions/userswitch_hover.png");
    m_userswitch->setPressPic(":/img/bottom_actions/userswitch_press.png");

    m_shutdown = new DImageButton;
    m_shutdown->setNormalPic(":/img/bottom_actions/shutdown_normal.png");
    m_shutdown->setHoverPic(":/img/bottom_actions/shutdown_hover.png");
    m_shutdown->setPressPic(":/img/bottom_actions/shutdown_press.png");

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(m_mprisWidget);
    mainLayout->setAlignment(m_mprisWidget, Qt::AlignBottom);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_userswitch);
    mainLayout->setAlignment(m_userswitch, Qt::AlignBottom);
    mainLayout->addWidget(m_shutdown);
    mainLayout->setAlignment(m_shutdown, Qt::AlignBottom);

    setLayout(mainLayout);
    setFixedSize(500, 150);

    connect(m_shutdown, &DImageButton::clicked, this, &ControlWidget::shutdownClicked);
    connect(m_userswitch, &DImageButton::clicked, this, &ControlWidget::switchUser);

    connect(m_mprisWidget, &DMPRISControl::mprisAcquired, [=] { m_mprisWidget->setVisible(m_mprisWidget->isWorking()); });
    connect(m_mprisWidget, &DMPRISControl::mprisLosted, [=] { m_mprisWidget->setVisible(m_mprisWidget->isWorking()); });

    m_mprisWidget->setVisible(m_mprisWidget->isWorking());

}

//void ControlWidget::switchUser() {
//    m_utilFile = new UtilFile(this);
//    m_utilFile->setExpandState(1);

//    QProcess *process = new QProcess;
//    process->start("dde-switchtogreeter");
//    process->waitForFinished();
//    process->deleteLater();

//    // FIXME: 这儿要是不延时结束的话会闪一下桌面
//  //  QTimer::singleShot(2000, qApp, SLOT(quit()));
//}
