// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "audioprovider.h"
#include "common.h"

#include <DGuiApplicationHelper>

DGUI_USE_NAMESPACE

AudioProvider::AudioProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_audioInter(new com::deepin::daemon::Audio("com.deepin.daemon.Audio",
                                                  "/com/deepin/daemon/Audio",
                                                  QDBusConnection::sessionBus(), this)),
      m_sinkInter(nullptr),
      m_isMute(false)
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
        return pixmapPath();
    } else if (role == Qt::EditRole) {
        return m_audioInter->increaseVolume();
    }

    return m_sinkInter->volume();
}

void AudioProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant progressData = index.data(Qt::DisplayRole);
    QVariant increaseVolumnData = index.data(Qt::EditRole);
    QString iconPath;
    QColor color;
    if (DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType) {
        iconPath = imageData.toString();
        color = QColor(Qt::black);
    } else {
        iconPath = QString(imageData.toString()).replace(".svg", "_dark.svg");
        color = QColor(Qt::white);
    }

    // NOTE: Max volume is 1.5
    DrawHelper::DrawImage(painter, option, iconPath, false, true);
    if (!m_isVisible) {
        color.setAlpha(0.4 * 255);
    }
    DrawHelper::DrawProgressBar(painter, option, progressData.toDouble() / (increaseVolumnData.toBool() ? 1.5 : 1.0), color);
    if (increaseVolumnData.toBool())
        DrawHelper::DrawVolumeGraduation(painter, option, color);
}

QSize AudioProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QString AudioProvider::pixmapPath() const
{
    m_isMute = m_sinkInter->mute();
    if (m_isMute) {
        return ":/icons/OSD_mute.svg";
    }
    const double volume = m_sinkInter->volume();

    const int level = volume > 0.6 ? 3 : volume > 0.3 ? 2 : 1;
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
        connect(m_sinkInter, &com::deepin::daemon::audio::Sink::MuteChanged, this, [this] {
            if (m_sinkInter->mute() != m_isMute)
                emit dataChanged();
        });

        m_sinkInter->setSync(true);
    }
}
