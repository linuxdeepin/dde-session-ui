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
    double m_volume;
    bool m_mute;

    com::deepin::daemon::Audio *m_audioInter;
    com::deepin::daemon::audio::Sink *m_sinkInter;

    QString pixmapPath() const;

private slots:
    void defaultSinkChanged(const QDBusObjectPath &path);
};

#endif // AUDIOPROVIDER_H
