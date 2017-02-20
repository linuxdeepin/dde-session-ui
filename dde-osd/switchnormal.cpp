/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchnormal.h"

#include <com_deepin_daemon_audio.h>
#include <com_deepin_daemon_audio_sink.h>

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

    int Y = (m_ParentItem->height() - IMAGE_SIZE) / 2;
    if (m_switchAudioOrBright)
        Y = (m_ParentItem->height() - IMAGE_SIZE) / 2.5;

    m_NormalImageLabel->move((m_ParentItem->width() - IMAGE_SIZE) / 2, Y);
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

    com::deepin::daemon::Audio audioInterface("com.deepin.daemon.Audio",
                                              "/com/deepin/daemon/Audio",
                                              QDBusConnection::sessionBus(),this);
    com::deepin::daemon::audio::Sink volumeInterface("com.deepin.daemon.Audio",
                                                     audioInterface.defaultSink().path(),
                                                     QDBusConnection::sessionBus(), this);
    // TODO: can not read the status first time.
    m_CanAudioMuteRun = volumeInterface.mute();

    m_switchAudioOrBright = true;

    if (whichImage == "Brightness") {
        showThemeImage(":/icons/OSD_light.png", m_NormalImageSvg, m_NormalImageLabel);
    } else if (whichImage == "AudioMute") {
        if (m_CanAudioMuteRun) {
            m_switchAudioOrBright = false;
            showThemeImage(":/icons/OSD_mute.png", m_NormalImageSvg, m_NormalImageLabel);
        } else {
            loadBasicImage("Audio");
        }
    } else if (whichImage == "Audio") {
        double volume = volumeInterface.volume();
        // 0.7~1.0 means high volume range, 0.3~0.7 means medium volume range, and 0.0~0.3 means low volume range.
        if (volume > 0.7 && volume <= 1.0) {
            showThemeImage(":/icons/OSD_volume_3.png", m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume > 0.3 && volume <= 0.7) {
            showThemeImage(":/icons/OSD_volume_2.png", m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume > 0.0) {
            showThemeImage(":/icons/OSD_volume_1.png", m_NormalImageSvg, m_NormalImageLabel);
        } else if (volume == 0.0) {
            showThemeImage(":/icons/OSD_mute.png", m_NormalImageSvg, m_NormalImageLabel);
        }
        emit muteChanged();
    }
}

double SwitchNormal::getVolume(){
    com::deepin::daemon::Audio audioInterface("com.deepin.daemon.Audio",
                                              "/com/deepin/daemon/Audio",
                                              QDBusConnection::sessionBus(),this);
    com::deepin::daemon::audio::Sink volumeInterface("com.deepin.daemon.Audio",
                                                     audioInterface.defaultSink().path(),
                                                     QDBusConnection::sessionBus(), this);
    return volumeInterface.volume();
}

void SwitchNormal::searchAddedImage(QString iconName){
    if (iconName =="capslock-enabled-symbolic") {
        showThemeImage(":/icons/OSD_caps_lock_off.png",m_NormalImageSvg,m_NormalImageLabel);
    } else if (iconName == "numlock-disabled-symbolic") {
        showThemeImage(":/icons/OSD_num_lock_off.png",m_NormalImageSvg,m_NormalImageLabel);
    } else if (iconName == "numlock-enabled-symbolic") {
        showThemeImage(":/icons/OSD_num_lock_on.png",m_NormalImageSvg,m_NormalImageLabel);
    } else if (iconName == "capslock-disabled-symbolic") {
        showThemeImage(":/icons/OSD_caps_lock_on.png",m_NormalImageSvg,m_NormalImageLabel);
    } else if (iconName == "touchpad-toggled-symbolic") {
        showThemeImage(":/icons/OSD_trackpad_on.png",m_NormalImageSvg,m_NormalImageLabel);
    } else if (iconName == "touchpad-disabled-symbolic") {
        showThemeImage(":/icons/OSD_trackpad_off.png",m_NormalImageSvg,m_NormalImageLabel);
    } else {
        showThemeImage(getThemeIconPath(iconName),m_NormalImageSvg,m_NormalImageLabel);
    }
    m_switchAudioOrBright = false;
}
