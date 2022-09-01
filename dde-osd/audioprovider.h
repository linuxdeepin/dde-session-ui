// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef AUDIOPROVIDER_H
#define AUDIOPROVIDER_H

#include "abstractosdprovider.h"

#include <com_deepin_daemon_audio.h>
#include <com_deepin_daemon_audio_sink.h>
/*!
 * \~chinese \class AudioProvider
 * \~chinese \brief 调整音频时的通知类
 */
class AudioProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit AudioProvider(QObject *parent = nullptr);

    // model
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QString pixmapPath() const;

private slots:
    void defaultSinkChanged(const QDBusObjectPath &path);

private:
    com::deepin::daemon::Audio *m_audioInter;
    com::deepin::daemon::audio::Sink *m_sinkInter;
    mutable bool m_isMute;
};

#endif // AUDIOPROVIDER_H
