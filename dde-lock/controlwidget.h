#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QWheelEvent>

#include <libdui/dimagebutton.h>

#include "dbus/dbusmediaplayer2.h"

DUI_USE_NAMESPACE

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

    void bindDBusService(DBusMediaPlayer2 *dbusInter);
    void hideMusicControlWidget();
    void showMusicControlWidget();

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    void volumeWheelControl(const QWheelEvent *e);
    void changePauseBtnPic();
    void changeVolumeBtnPic();

private:
    QLabel *m_songName;
    QLabel *m_volumeNums;
    DImageButton *m_prevSong;
    DImageButton *m_pauseSong;
    DImageButton *m_nextSong;
    DImageButton *m_volume;
    DImageButton *m_shutdown;

    QWidget *m_songControlWidget;
    DBusMediaPlayer2 *m_dbusInter = nullptr;
    double m_lastVolumeNums = 1.0;
};

#endif // CONTROLWIDGET_H
