/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QWheelEvent>

#include <dimagebutton.h>

#include "dbus/dbusmediaplayer2.h"
#include "util_file.h"

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

    void bindDBusService(DBusMediaPlayer2 *dbusInter);
    void hideMusicControlWidget();
    void showMusicControlWidget();
    inline void setUserSwitchEnable(bool enable) {m_userswitch->setVisible(enable);}

signals:
    void shutdownClicked();

private slots:
    void volumeWheelControl(const QWheelEvent *e);
    void changePauseBtnPic();
    void changeVolumeBtnPic();
    void switchToGreeter();

protected:
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    QLabel *m_songName;
    QLabel *m_volumeNums;
    DTK_WIDGET_NAMESPACE::DImageButton *m_prevSong;
    DTK_WIDGET_NAMESPACE::DImageButton *m_pauseSong;
    DTK_WIDGET_NAMESPACE::DImageButton *m_nextSong;
    DTK_WIDGET_NAMESPACE::DImageButton *m_volume;
    DTK_WIDGET_NAMESPACE::DImageButton* m_userswitch;
    DTK_WIDGET_NAMESPACE::DImageButton *m_shutdown;

    QWidget *m_songControlWidget;
    DBusMediaPlayer2 *m_dbusInter = nullptr;
    double m_lastVolumeNums = 1.0;
    UtilFile* m_utilFile;
};

#endif // CONTROLWIDGET_H
