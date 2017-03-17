#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include <com_deepin_daemon_audio.h>
#include <com_deepin_daemon_audio_sink.h>
#include <com_deepin_daemon_display.h>

#include "common.h"

class DataWrapper : public QObject
{
    Q_OBJECT
public:
    explicit DataWrapper(QObject *parent = 0);

    double volume() const;
    void setVolume(double volume);

    bool mute() const;
    void setMute(bool mute);

    double brightness() const;
    void setBrightness(double brightness);

signals:
    void dataChanged();

private:
    double m_volume;
    bool m_mute;
    double m_brightness;
};

class Container;
class ListView;
class Delegate;
class Model;
class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);

    void setStyle(const Style &style);

public slots:
    void ShowOSD(const QString &osd);

private:
    Container *m_container;
    ListView *m_listview;
    Delegate *m_delegate;
    Model *m_model;

    Style m_style;
    QString m_param;

    DataWrapper *m_data;
    com::deepin::daemon::Audio *m_audioInter;
    com::deepin::daemon::audio::Sink *m_sinkInter;
    com::deepin::daemon::Display *m_displayInter;

    void initData();

    void updateUI();
    void setupSize();
    void setupData();

    QString getCurrentPixmapPath();

private slots:
    void defaultSinkChanged(const QDBusObjectPath &path);
    void brightnessChanged(const BrightnessMap &brightness);
};

#endif // MANAGER_H
