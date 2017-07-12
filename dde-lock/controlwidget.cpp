/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QHBoxLayout>
#include <QWheelEvent>

#include "controlwidget.h"

DWIDGET_USE_NAMESPACE

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mprisWidget = new DMPRISControl;
    m_mprisWidget->setFixedWidth(200);
    m_mprisWidget->setStyleSheet("color:white;"
                              "font-size:15px;");

    m_volumeNums = new QLabel;
    m_volumeNums->hide();
    m_volumeNums->setStyleSheet("color:white;"
                                "font-size:13px;");

    m_userswitch = new DImageButton;
    m_userswitch->setNormalPic(":/img/bottom_actions/userswitch_normal.png");
    m_userswitch->setHoverPic(":/img/bottom_actions/userswitch_hover.png");
    m_userswitch->setPressPic(":/img/bottom_actions/userswitch_press.png");

    m_shutdown = new DImageButton;
    m_shutdown->setNormalPic(":/img/bottom_actions/shutdown_normal.png");
    m_shutdown->setHoverPic(":/img/bottom_actions/shutdown_hover.png");
    m_shutdown->setPressPic(":/img/bottom_actions/shutdown_press.png");

    m_volume = new DImageButton;
    m_volume->setNormalPic(":/img/mpris/volume_normal.png");
    m_volume->setHoverPic(":/img/mpris/volume_hover.png");
    m_volume->setPressPic(":/img/mpris/volume_press.png");
    m_volume->installEventFilter(this);

    QVBoxLayout *volumeLayout = new QVBoxLayout;
    volumeLayout->setMargin(0);
    volumeLayout->setSpacing(0);
    volumeLayout->addStretch();
    volumeLayout->addWidget(m_volumeNums, 0 , Qt::AlignHCenter);
    volumeLayout->addSpacing(15);
    volumeLayout->addWidget(m_volume);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(26);
    mainLayout->addStretch();
    mainLayout->addWidget(m_mprisWidget);
    mainLayout->setAlignment(m_mprisWidget, Qt::AlignBottom);
    mainLayout->addLayout(volumeLayout);
    mainLayout->addWidget(m_userswitch);
    mainLayout->setAlignment(m_userswitch, Qt::AlignBottom);
    mainLayout->addWidget(m_shutdown);
    mainLayout->setAlignment(m_shutdown, Qt::AlignBottom);
    mainLayout->addSpacing(28);

    setLayout(mainLayout);
    setFixedSize(500, 150);

    connect(m_shutdown, &DImageButton::clicked, this, &ControlWidget::shutdownClicked);
    connect(m_userswitch, &DImageButton::clicked, this, &ControlWidget::switchUser);

    connect(m_mprisWidget, &DMPRISControl::mprisAcquired, this, &ControlWidget::changeVisible);
    connect(m_mprisWidget, &DMPRISControl::mprisLosted, this, &ControlWidget::changeVisible);

    changeVisible();
}

void ControlWidget::bindDBusService(DBusMediaPlayer2 *dbusInter)
{
    m_dbusInter = dbusInter;

    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, this, &ControlWidget::changeVolumeBtnPic);
    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, [this] {
        m_volumeNums->setText(QString::number(m_dbusInter->volume() * 100));
    });

    connect(m_volume, &DImageButton::clicked, [this] {
        const double currentVolume = m_dbusInter->volume();

        if (currentVolume) {
            m_lastVolumeNums = currentVolume;
            m_dbusInter->setVolume(0);
        } else {
            m_dbusInter->setVolume(m_lastVolumeNums);
        }
    });

    m_lastVolumeNums = m_dbusInter->volume();

    m_dbusInter->VolumeChanged();
    m_dbusInter->MetadataChanged();
    m_dbusInter->PlaybackStatusChanged();
    m_dbusInter->VolumeChanged();
}


bool ControlWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_volume) {
        switch (e->type()) {
        case QEvent::Wheel:     volumeWheelControl(reinterpret_cast<QWheelEvent *>(e));         break;
        case QEvent::Enter:     m_volumeNums->show();       break;
        case QEvent::Leave:     m_volumeNums->hide();       break;
        default:;
        }
    }

    return false;
}

void ControlWidget::volumeWheelControl(const QWheelEvent *e)
{
    const int delta = e->delta() / 60;

    m_lastVolumeNums += double(delta) / 100;
    if (m_lastVolumeNums < 0)
        m_lastVolumeNums = 0;
    if (m_lastVolumeNums > 1.0)
        m_lastVolumeNums = 1.0;

    m_dbusInter->setVolume(m_lastVolumeNums);
}

void ControlWidget::changeVolumeBtnPic()
{
    if (m_dbusInter->volume()) {

        const double volume = m_dbusInter->volume();
        if (volume < 0.3) {
            m_volume->setNormalPic(":/img/mpris/volume3_normal.png");
            m_volume->setHoverPic(":/img/mpris/volume3_hover.png");
            m_volume->setPressPic(":/img/mpris/volume3_press.png");
        } else if (volume < 0.6) {
            m_volume->setNormalPic(":/img/mpris/volume2_normal.png");
            m_volume->setHoverPic(":/img/mpris/volume2_hover.png");
            m_volume->setPressPic(":/img/mpris/volume2_press.png");
        } else {
            m_volume->setNormalPic(":/img/mpris/volume_normal.png");
            m_volume->setHoverPic(":/img/mpris/volume_hover.png");
            m_volume->setPressPic(":/img/mpris/volume_press.png");
        }
    } else {
        m_volume->setNormalPic(":/img/mpris/mute_normal.png");
        m_volume->setHoverPic(":/img/mpris/mute_hover.png");
        m_volume->setPressPic(":/img/mpris/mute_press.png");
    }
}

void ControlWidget::changeVisible()
{
    const bool isWorking = m_mprisWidget->isWorking();
    m_mprisWidget->setVisible(isWorking);
    m_volume->setVisible(isWorking);
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
