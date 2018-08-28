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

#include "mediawidget.h"
#include "util_updateui.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWheelEvent>

MediaWidget::MediaWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
}

void MediaWidget::initUI()
{
    m_dmprisWidget = new DMPRISControl;
    m_dmprisWidget->setFixedWidth(200);
    m_dmprisWidget->setPictureVisible(false);

    m_volumeBtn = new DImageButton;
    m_volumeBtn->setNormalPic(":/img/mpris/volume_normal.svg");
    m_volumeBtn->setHoverPic(":/img/mpris/volume_hover.svg");
    m_volumeBtn->setPressPic(":/img/mpris/volume_press.svg");
    m_volumeBtn->installEventFilter(this);

    m_volumeNums = new QLabel;
    m_volumeNums->hide();

    QVBoxLayout *volumeLayout = new QVBoxLayout;
    volumeLayout->setMargin(0);
    volumeLayout->setSpacing(0);
    volumeLayout->addStretch();
    volumeLayout->addWidget(m_volumeNums, 0 , Qt::AlignHCenter);
    volumeLayout->addSpacing(15);
    volumeLayout->addWidget(m_volumeBtn);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);

    mainlayout->addStretch();
    mainlayout->addWidget(m_dmprisWidget, 0, Qt::AlignBottom);
    mainlayout->addSpacing(26);
    mainlayout->addLayout(volumeLayout);

    setLayout(mainlayout);

    updateStyle(":/skin/mediawidget.qss", this);
}

void MediaWidget::initConnect()
{
    connect(m_volumeBtn, &DImageButton::clicked, this, &MediaWidget::onMuteSwitchChanged);
    connect(m_dmprisWidget, &DMPRISControl::mprisAcquired, this, &MediaWidget::changeVisible);
    connect(m_dmprisWidget, &DMPRISControl::mprisLosted, this, &MediaWidget::changeVisible);

    changeVisible();
}

void MediaWidget::initMediaPlayer()
{
    QDBusInterface dbusInter("org.freedesktop.DBus", "/", "org.freedesktop.DBus", QDBusConnection::sessionBus(), this);

    QDBusReply<QStringList> response = dbusInter.call("ListNames");
    const QStringList &serviceList = response.value();
    QString service = QString();
    for (const QString &serv : serviceList) {
        if (!serv.startsWith("org.mpris.MediaPlayer2.")) {
            continue;
        }
        service = serv;
        break;
    }
    if (service.isEmpty()) {
        qDebug() << "media player dbus has not started, waiting for signal...";
        QDBusConnectionInterface *dbusDaemonInterface = QDBusConnection::sessionBus().interface();
        connect(dbusDaemonInterface, &QDBusConnectionInterface::serviceOwnerChanged, this,
                [=](const QString &name, const QString &oldOwner, const QString &newOwner) {
                    Q_UNUSED(oldOwner);
                    if (name.startsWith("org.mpris.MediaPlayer2.") && !newOwner.isEmpty()) {
                        initMediaPlayer();
                        disconnect(dbusDaemonInterface);
                    }
                }
        );
        return;
    }

    qDebug() << "got media player dbus service: " << service;

    m_dbusInter = new DBusMediaPlayer2(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);

    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, this, &MediaWidget::onVolumeChanged);
    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, [this] {
        m_volumeNums->setText(QString::number(m_dbusInter->volume() * 100));
    });

    m_lastVolumeNums = m_dbusInter->volume();

    m_dbusInter->VolumeChanged();
    m_dbusInter->MetadataChanged();
    m_dbusInter->PlaybackStatusChanged();
    m_dbusInter->VolumeChanged();
}

void MediaWidget::onVolumeChanged()
{
    if (m_dbusInter->volume()) {
        const double volume = m_dbusInter->volume();
        if (volume < 0.3) {
            m_volumeBtn->setNormalPic(":/img/mpris/volume_low_normal.svg");
            m_volumeBtn->setHoverPic(":/img/mpris/volume_low_hover.svg");
            m_volumeBtn->setPressPic(":/img/mpris/volume_low_press.svg");
        } else if (volume < 0.6) {
            m_volumeBtn->setNormalPic(":/img/mpris/volume_medium_normal.svg");
            m_volumeBtn->setHoverPic(":/img/mpris/volume_medium_hover.svg");
            m_volumeBtn->setPressPic(":/img/mpris/volume_medium_press.svg");
        } else {
            m_volumeBtn->setNormalPic(":/img/mpris/volume_normal.svg");
            m_volumeBtn->setHoverPic(":/img/mpris/volume_hover.svg");
            m_volumeBtn->setPressPic(":/img/mpris/volume_press.svg");
        }
    } else {
        m_volumeBtn->setNormalPic(":/img/mpris/mute_normal.svg");
        m_volumeBtn->setHoverPic(":/img/mpris/mute_hover.svg");
        m_volumeBtn->setPressPic(":/img/mpris/mute_press.svg");
    }
}

void MediaWidget::onMuteSwitchChanged()
{
    if (!m_dbusInter) {
        return;
    }

    const double currentVolume = m_dbusInter->volume();

    if (currentVolume) {
        m_lastVolumeNums = currentVolume;
        m_dbusInter->setVolume(0);
    } else {
        m_dbusInter->setVolume(m_lastVolumeNums);
    }
}

bool MediaWidget::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)

    switch (event->type()) {
    case QEvent::Wheel:
    {
        QWheelEvent *e  = static_cast<QWheelEvent *>(event);

        if (!e)
            break;

        const int delta = e->delta() / 60;

        m_lastVolumeNums += double(delta) / 100;

        if (m_lastVolumeNums < 0)
            m_lastVolumeNums = 0;
        if (m_lastVolumeNums > 1.0)
            m_lastVolumeNums = 1.0;

        if (!m_dbusInter) {
            break;
        }

        m_dbusInter->setVolume(m_lastVolumeNums);
        break;
    }
    case QEvent::Enter:     m_volumeNums->show();       break;
    case QEvent::Leave:     m_volumeNums->hide();       break;
    default:;
    }

    return false;
}

void MediaWidget::changeVisible()
{
    const bool isWorking = m_dmprisWidget->isWorking();
    m_dmprisWidget->setVisible(isWorking);
    m_volumeBtn->setVisible(isWorking);
}
