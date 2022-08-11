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

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

AudioProvider::AudioProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_audioInter(new com::deepin::daemon::Audio("com.deepin.daemon.Audio",
                                                  "/com/deepin/daemon/Audio",
                                                  QDBusConnection::sessionBus(), this)),
      m_sinkInter(nullptr)
{
    m_suitableParams << "AudioUp" << "AudioDown" << "AudioMute" << "AudioUpAsh" << "AudioDownAsh" << "AudioMuteAsh";

    m_audioInter->setSync(true, false);
    connect(m_audioInter, &__Audio::DefaultSinkChanged,
            this, &AudioProvider::defaultSinkChanged);

    auto onAudioIsValid = [ = ](bool isvalid) {
        if (isvalid) {
            defaultSinkChanged(m_audioInter->defaultSink());
        }
    };

    connect(m_audioInter, &__Audio::serviceValidChanged, this, onAudioIsValid);

    onAudioIsValid(m_audioInter->isValid());
}

int AudioProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant AudioProvider::data(const QModelIndex &, int role) const
{
    if (!m_sinkInter->isValid()) {
        AudioProvider *provider = const_cast<AudioProvider *>(this);
        provider->defaultSinkChanged(provider->m_audioInter->defaultSink());
    }

    if (role == Qt::DecorationRole) {
        return pixmapName();
    } else if (role == Qt::EditRole) {
        return m_audioInter->increaseVolume();
    }

    return m_sinkInter->volume();
}

void AudioProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QVariant increaseVolumeData = index.data(Qt::EditRole);
    QVariant volumeValue = index.data(Qt::DisplayRole);
    QString pixmapName = index.data(Qt::DecorationRole).toString();

    QColor color;
    bool isLightTheme = false;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        color = QColor(Qt::black);
        isLightTheme = true;
    } else {
        color = QColor(Qt::white);
    }

    DrawHelper::DrawImage(painter, option, pixmapName, isLightTheme);

    DrawHelper::DrawProgressBar(painter, option, volumeValue.toDouble() / (increaseVolumeData.toBool() ? 1.5 : 1.0), color);

    // 绘制刻度
    if (increaseVolumeData.toBool())
        drawScale(painter, option);

    DrawHelper::DrawPercentValue(painter, option, volumeValue.toDouble() * 100);
}

QSize AudioProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QString AudioProvider::pixmapName() const
{
    if (m_sinkInter->mute()) {
        return "osd_volume_mute";
    }

    const double volume = m_sinkInter->volume();
    QString level = "0";
    if (volume > 0 && volume <= 0.33)
        level = "33";
    else if (volume > 0.33 && volume <= 0.66)
        level = "66";
    else if (volume > 0.66 && volume <= 1)
        level = "100";
    else if (volume > 1)
        level = "more";

    return QString("osd_volume_%1").arg(level);
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
        m_sinkInter->setSync(true);
    }
}

void AudioProvider::drawScale(QPainter *painter, const QStyleOptionViewItem &option) const
{
    const int leftMargin = 200;
    const int topMargin = 20;
    const int lineHeight = 6;
    const int spacing = 8;

    QPen pen;
    pen.setWidth(2);

    QColor color(Qt::black);
    color.setAlpha(255 * 0.3);
    pen.setColor(color);
    painter->setPen(pen);

    int x = option.rect.left() + leftMargin;
    int y = option.rect.top() + topMargin;
    painter->drawLine(x, y, x, y + lineHeight);
    painter->drawLine(x, y + lineHeight + spacing, x, y + lineHeight + spacing + lineHeight);
}
