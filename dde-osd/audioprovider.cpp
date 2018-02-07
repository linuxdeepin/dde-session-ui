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

#include "audioprovider.h"

#include "common.h"

AudioProvider::AudioProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_volume(0),
      m_mute(false),
      m_audioInter(new com::deepin::daemon::Audio("com.deepin.daemon.Audio",
                                                  "/com/deepin/daemon/Audio",
                                                  QDBusConnection::sessionBus(), this)),
      m_sinkInter(nullptr)
{
    m_suitableParams << "AudioUp" << "AudioDown" << "AudioMute";

    m_audioInter->setSync(false);
    connect(m_audioInter, &__Audio::DefaultSinkChanged,
            this, &AudioProvider::defaultSinkChanged);
    defaultSinkChanged(m_audioInter->defaultSink());
}

void AudioProvider::setVolume(double volume)
{
    if (volume == m_volume) return;

    m_volume = volume;
    emit dataChanged();
}

void AudioProvider::setMute(bool mute)
{
    if (mute == m_mute) return;

    m_mute = mute;
    emit dataChanged();
}

int AudioProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant AudioProvider::data(const QModelIndex &, int role) const
{
    if (role == Qt::DecorationRole) {
        return pixmapPath();
    }

    return m_mute ? 0 : m_volume;
}

void AudioProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant progressData = index.data(Qt::DisplayRole);

    DrawHelper::DrawImage(painter, option, imageData.toString(), false, true);
    DrawHelper::DrawProgressBar(painter, option, progressData.toDouble());
}

QSize AudioProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QString AudioProvider::pixmapPath() const
{
    if (m_mute) {
        return ":/icons/OSD_mute.svg";
    }

    const int level = m_volume > 0.6 ? 3 : m_volume > 0.3 ? 2 : 1;
    return QString(":/icons/OSD_volume_%1.svg").arg(level);
}

void AudioProvider::defaultSinkChanged(const QDBusObjectPath &path)
{
    const QString pathStr = path.path();
    if (!pathStr.isEmpty()) {
        if (m_sinkInter) {
            m_sinkInter->deleteLater();
            m_sinkInter = nullptr;
        }

        m_sinkInter = new com::deepin::daemon::audio::Sink("com.deepin.daemon.Audio",
                                                           pathStr,
                                                           QDBusConnection::sessionBus(), this);
        m_sinkInter->setSync(false);

        connect(m_sinkInter, &__Sink::VolumeChanged, this, &AudioProvider::setVolume);
        connect(m_sinkInter, &__Sink::MuteChanged, this, &AudioProvider::setMute);
        setVolume(m_sinkInter->volume());
        setMute(m_sinkInter->mute());
    }
}


