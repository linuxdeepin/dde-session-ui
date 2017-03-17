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

    QList<QPair<QString, QString>> data() const Q_DECL_OVERRIDE;
    Style style() const Q_DECL_OVERRIDE;

    void setVolume(double volume);
    void setMute(bool mute);

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
