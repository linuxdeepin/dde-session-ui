/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtDebug>
#include "dbreathinglabel.h"

DBreathingLabel::DBreathingLabel(QWidget *parent,Qt::WindowFlags f)
    : QLabel(parent, f), alpha_(255)
{
    m_showAnimation = new QPropertyAnimation(this, "alpha");
    m_hideAnimation = new QPropertyAnimation(this, "alpha");
    setDuration(400);
}
DBreathingLabel::DBreathingLabel (const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent,f), alpha_(255)
{
    m_showAnimation = new QPropertyAnimation(this, "alpha");
    m_hideAnimation = new QPropertyAnimation(this, "alpha");
}
DBreathingLabel::~DBreathingLabel()
{
    delete m_showAnimation;
    delete m_hideAnimation;
}
int DBreathingLabel::alpha() const{
    return alpha_;
}
void DBreathingLabel::setDuration(int duration) {
    m_showAnimation->setDuration(duration);
    m_hideAnimation->setDuration(duration);
}
void DBreathingLabel::setColor(QColor color) {
    m_palette.setColor(QPalette::WindowText, color);
    this->setPalette(m_palette);
}
void DBreathingLabel::setAlpha(int opa) {
    alpha_ = opa;
    QColor color =m_palette.color(QPalette::WindowText);
    color = QColor(color.red(), color.green(), color.blue(), opa);
    m_palette.setColor(QPalette::Foreground, color);
    setPalette(m_palette);
}


void DBreathingLabel::hideLabel() {
    m_showAnimation->setStartValue(255);
    m_showAnimation->setEndValue(0);
    m_showAnimation->start();
}

void DBreathingLabel::showLabel() {
    m_hideAnimation->setStartValue(0);
    m_hideAnimation->setEndValue(255);
    m_hideAnimation->start();
}
