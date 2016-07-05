/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchnormal.h"

extern QString getThemeIconPath(QString iconName);
extern void showThemeImage(QString iconName, QSvgWidget* svgLoader, QLabel* notSvgLoader);

SwitchNormal::SwitchNormal(QWidget *parent) : QWidget(parent)
{
    initGlobalVars(parent);

    initBasicOperation();
}

void SwitchNormal::initGlobalVars(QWidget *parent){


    m_ParentItem = parent;

    m_NormalImageSvg = new QSvgWidget(parent);
    m_NormalImageLabel = new QLabel(parent);

    m_SwitchWMLabel = new QLabel(parent);
}

void SwitchNormal::initBasicOperation(){
    m_ParentItem->resize(BASE_SIZE,BASE_SIZE);
    // set fixed size for image icon, and move it to app's center
    m_NormalImageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_NormalImageLabel->move((m_ParentItem->width() - IMAGE_SIZE) / 2, (m_ParentItem->height() - IMAGE_SIZE) / 2);

    // set fixed size for image icon, and move it to app's center
    m_NormalImageSvg->setFixedSize(IMAGE_SIZE,IMAGE_SIZE);
    m_NormalImageSvg->move((m_ParentItem->width() - IMAGE_SIZE) / 2, (m_ParentItem->height() - IMAGE_SIZE) / 2);

    // set font-size and color and position and wordwrap and alignment for m_SwitchWMLabel
    m_SwitchWMLabel->setGeometry(SWITCHWM_TEXT_GEOMETRY);
    m_SwitchWMLabel->setStyleSheet(SWITCHWM_TEXT_STYLE);
    m_SwitchWMLabel->setWordWrap(true);
    m_SwitchWMLabel->setAlignment(Qt::AlignCenter);
}

void SwitchNormal::hideNormal(){
    m_NormalImageSvg->setVisible(false);
    m_NormalImageLabel->setVisible(false);
    m_SwitchWMLabel->setVisible(false);
}

void SwitchNormal::showNormal(){
    m_NormalImageSvg->setVisible(true);
    m_NormalImageLabel->setVisible(true);
    m_SwitchWMLabel->setVisible(true);
}

void SwitchNormal::cancelText(){
    m_NormalImageSvg->move((m_ParentItem->width() - IMAGE_SIZE) / 2, (m_ParentItem->height() - IMAGE_SIZE) / 2);
    m_NormalImageLabel->move((m_ParentItem->width() - IMAGE_SIZE) / 2, (m_ParentItem->height() - IMAGE_SIZE) / 2);
    m_SwitchWMLabel->setText("");
}

void SwitchNormal::showText(QString text){
    m_NormalImageSvg->move(SWITCHWM_IMAGE_POINT);
    m_NormalImageLabel->move(SWITCHWM_IMAGE_POINT);
    m_SwitchWMLabel->setText(text);
}

// this function is used to display normal osd'show, which is different from SwitchLayout and SwitchMonitors
void SwitchNormal::loadBasicImage(QString whichImage)
{
    showNormal();
    // DbusInterface to get defalut sink, which is usabel when computer has more than 1 sound card
    QDBusInterface audioInterface("com.deepin.daemon.Audio",
                                          "/com/deepin/daemon/Audio",
                                          "",QDBusConnection::sessionBus(),this);
    // default sink path
    QString defautSinkPath = qvariant_cast<QDBusObjectPath>(audioInterface.call("GetDefaultSink").arguments()[0]).path();
    VolumeDbus volumeInterface("com.deepin.daemon.Audio",
                               defautSinkPath,
                               QDBusConnection::sessionBus(), this);
    // TODO: can not read the status first time.
    m_CanAudioMuteRun = volumeInterface.mute();
    m_CanAudioMuteRun = volumeInterface.mute();
    m_CanAudioMuteRun = volumeInterface.mute();
    if (whichImage == "Brightness") {
        showThemeImage(getThemeIconPath("display-brightness-symbolic"), m_NormalImageSvg, m_NormalImageLabel);
    } else if (whichImage == "AudioMute") {
        if (m_CanAudioMuteRun) {
            showThemeImage(getThemeIconPath("audio-volume-muted-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        } else {
            loadBasicImage("Audio");
        }
    } else if (whichImage == "Audio") {
        double volume = volumeInterface.volume();
        // 0.7~1.0 means high volume range, 0.3~0.7 means medium volume range, and 0.0~0.3 means low volume range.
        if (volume > 0.7 && volume <= 1.0) {
            showThemeImage(getThemeIconPath("audio-volume-high-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume > 0.3 && volume <= 0.7) {
            showThemeImage(getThemeIconPath("audio-volume-medium-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume > 0.0) {
            showThemeImage(getThemeIconPath("audio-volume-low-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume == 0.0) {
            showThemeImage(getThemeIconPath("audio-volume-muted-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        }
    }
}

double SwitchNormal::getVolume(){
    QDBusInterface audioInterface("com.deepin.daemon.Audio",
                                          "/com/deepin/daemon/Audio",
                                          "",QDBusConnection::sessionBus(),this);
    // DbusInterface to get defalut sink, which is usabel when computer has more than 1 sound card
    QString defautSinkPath = qvariant_cast<QDBusObjectPath>(audioInterface.call("GetDefaultSink").arguments()[0]).path();
    VolumeDbus volumeInterface("com.deepin.daemon.Audio",
                               defautSinkPath,
                               QDBusConnection::sessionBus(), this);
    return volumeInterface.volume();
}

void SwitchNormal::searchAddedImage(QString iconName){
    showThemeImage(getThemeIconPath(iconName),m_NormalImageSvg,m_NormalImageLabel);
}
