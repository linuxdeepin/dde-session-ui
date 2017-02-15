/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SWITCHNORMAL_H
#define SWITCHNORMAL_H

#include "dbus/volumedbus.h"
#include <QWidget>
#include <QSvgWidget>
#include <QLabel>

#include "dbus/dbusaudio.h"
#include "dbus/volumedbus.h"

class SwitchNormal : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchNormal(QWidget *parent = 0);

    void loadBasicImage(QString whichImage);
    void searchAddedImage(QString iconName);
    void hideNormal();
    void showNormal();
    void cancelText();
    void showText(QString text);

    double getVolume();
    bool getMute();

private slots:
    void defaultSinkChanged();

private:
    void initGlobalVars(QWidget *parent);
    void initBasicOperation();

    QString m_lastImage;
    bool m_mute;
    double m_volume;

    DBusAudio *m_dbusAudio;
    VolumeDbus *m_dbusSink;

    QWidget* m_ParentItem;
    QSvgWidget* m_NormalImageSvg;
    QLabel* m_NormalImageLabel;
    QLabel* m_SwitchWMLabel;

    bool m_CanAudioMuteRun;

    const int BASE_SIZE = 140;
    const int IMAGE_SIZE = 56;
    const QRect SWITCHWM_TEXT_GEOMETRY = QRect(16,96,108,34);
    const QPoint SWITCHWM_IMAGE_POINT = QPoint(42,30);
    const QString SWITCHWM_TEXT_STYLE = "QLabel{font-size:14px;color:#ffffff}";
};

#endif // SWITCHNORMAL_H
