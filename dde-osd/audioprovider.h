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

#ifndef AUDIOPROVIDER_H
#define AUDIOPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_audio.h>
#include <com_deepin_daemon_audio_sink.h>

class AudioProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit AudioProvider(QObject *parent = 0);

    void setVolume(double volume);
    void setMute(bool mute);

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QTimer *m_initSinkTimer;

    double m_volume;
    bool m_mute;
    int m_tryLimit;

    com::deepin::daemon::Audio *m_audioInter;
    com::deepin::daemon::audio::Sink *m_sinkInter;

    QString pixmapPath() const;

private slots:
    void defaultSinkChanged(const QDBusObjectPath &path);
    void tryGetDbusData();
};

#endif // AUDIOPROVIDER_H
