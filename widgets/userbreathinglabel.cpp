/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "userbreathinglabel.h"

#include <QtDebug>

UserBreathingLabel::UserBreathingLabel(QWidget *parent)
    : QLabel(parent), m_opacity(1.0)
{
    m_showOpacityAnimation = new QPropertyAnimation(this, "opacity");
    m_hideOpacityAnimation = new QPropertyAnimation(this, "opacity");
    label_effect = new QGraphicsOpacityEffect(this);

    connect(this, SIGNAL(opacityChanged()), this, SLOT(setEffect()));
    connect(this, SIGNAL(hideFinished()), this, SLOT(hide()));
}
UserBreathingLabel::~UserBreathingLabel()
{
    delete m_showOpacityAnimation;
    delete m_hideOpacityAnimation;
}

//////////////////////////////////Test New Function
///
double UserBreathingLabel::opacity() const {
    return m_opacity;
}
void UserBreathingLabel::setOpacity(double opa) {
    if (opa != m_opacity) {
        m_opacity = opa;
        emit opacityChanged();
    }

}

void UserBreathingLabel::showLabel()
{
    if (m_showOpacityAnimation->state() == QPropertyAnimation::Running ||
        m_hideOpacityAnimation->state() == QPropertyAnimation::Running ||
        isVisible())
        return update();

    m_showOpacityAnimation->setStartValue(0);
    m_showOpacityAnimation->setEndValue(1.0);
    m_showOpacityAnimation->start();
    m_showOpacityAnimation->setDuration(500);
}

void UserBreathingLabel::hideLabel()
{
    if (m_showOpacityAnimation->state() == QPropertyAnimation::Running ||
        m_hideOpacityAnimation->state() == QPropertyAnimation::Running ||
        !isVisible())
        return update();

    m_hideOpacityAnimation->setStartValue(1.0);
    m_hideOpacityAnimation->setEndValue(0);
    m_hideOpacityAnimation->start();
    m_hideOpacityAnimation->setDuration(300);
}

void UserBreathingLabel::setEffect() {
    label_effect->setOpacity(m_opacity);
    this->setGraphicsEffect(label_effect);

}
