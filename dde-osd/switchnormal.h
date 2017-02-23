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

#include <QWidget>
#include <QSvgWidget>
#include <QLabel>

class SwitchNormal : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchNormal(QWidget *parent = 0);

    double getVolume();
    void loadBasicImage(QString whichImage);
    void searchAddedImage(QString iconName);
    void hideNormal();
    void showNormal();
    void cancelText();
    void showText(QString text);

signals:
    void muteChanged();

private:
    void initGlobalVars(QWidget *parent);
    void initBasicOperation();

    QWidget* m_ParentItem;
    QSvgWidget* m_NormalImageSvg;
    QLabel* m_NormalImageLabel;
    QLabel* m_SwitchWMLabel;

    bool m_CanAudioMuteRun;
    bool m_switchAudioOrBright;

    const int BASE_SIZE = 140;
    const int IMAGE_SIZE = 64;
    const QRect SWITCHWM_TEXT_GEOMETRY = QRect(10,96,120,34);
    const QPoint SWITCHWM_IMAGE_POINT = QPoint(38,30);
    const QString SWITCHWM_TEXT_STYLE = "QLabel{font-size:14px;color:#000000}";
};

#endif // SWITCHNORMAL_H
