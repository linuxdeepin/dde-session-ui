/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchnormal.h"

#include <QtConcurrent>
#include <QFutureWatcher>

extern QString getThemeIconPath(QString iconName);
extern void showThemeImage(QString iconName, QSvgWidget* svgLoader, QLabel* notSvgLoader);

SwitchNormal::SwitchNormal(QWidget *parent)
    : QWidget(parent),
      m_mute(false),
      m_volume(0),
      m_dbusAudio(new DBusAudio(this)),
      m_dbusSink(nullptr)
{
    initGlobalVars(parent);
    initBasicOperation();

    defaultSinkChanged();
    connect(m_dbusAudio, &DBusAudio::DefaultSinkChanged, this, &SwitchNormal::defaultSinkChanged);
}

void SwitchNormal::initGlobalVars(QWidget *parent)
{
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

bool SwitchNormal::getMute()
{
    return m_mute;
}

double SwitchNormal::getVolume()
{
    return m_volume;
}

void SwitchNormal::defaultSinkChanged()
{
    const QDBusObjectPath path = m_dbusAudio->GetDefaultSink();
    const QString sinkPath = path.path();

    if (m_dbusSink) {
        m_dbusSink->deleteLater();
    }
    m_dbusSink = new VolumeDbus("com.deepin.daemon.Audio", sinkPath, QDBusConnection::sessionBus(), this);
    m_volume = m_dbusSink->volume();
    m_mute = m_dbusSink->mute();

    connect(m_dbusSink, &VolumeDbus::MuteChanged, [this] { m_mute = m_dbusSink->mute(); });
    connect(m_dbusSink, &VolumeDbus::VolumeChanged, [this] {
        if (m_lastImage == "AudioUp") {
            m_volume = qMax(m_dbusSink->volume(), m_volume);
        } else {
            m_volume = qMin(m_dbusSink->volume(), m_volume);
        }
    });
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

    if (whichImage.startsWith("Brightness")) {
        showThemeImage(getThemeIconPath("display-brightness-symbolic"), m_NormalImageSvg, m_NormalImageLabel);
    } else if (whichImage == "AudioMute") {
        if (!m_mute) {
            showThemeImage(getThemeIconPath("audio-volume-muted-symbolic-osd"), m_NormalImageSvg, m_NormalImageLabel);
        } else {
            loadBasicImage("AudioUp");
        }
    } else if (whichImage.startsWith("Audio")) {
        double volume = m_volume;
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

    m_lastImage = whichImage;
}

void SwitchNormal::searchAddedImage(QString iconName) {
    showThemeImage(getThemeIconPath(iconName),m_NormalImageSvg,m_NormalImageLabel);
}
