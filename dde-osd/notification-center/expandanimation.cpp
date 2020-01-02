/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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

#include "expandanimation.h"
#include "bubbleitem.h"
#include "notification/notificationentity.h"
#include "notification/constants.h"

#include <QPropertyAnimation>

ExpandAnimation::ExpandAnimation(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setMaskColor(DBlurEffectWidget::AutoColor);

    m_animationGroup = new QParallelAnimationGroup(this);
    connect(m_animationGroup, &QParallelAnimationGroup::finished, this, &ExpandAnimation::finished);
}

ExpandAnimation::~ExpandAnimation()
{
    if (m_animationGroup != nullptr) {
        m_animationGroup->deleteLater();
    }

    if (!m_bubbleList.isEmpty()) {
        qDeleteAll(m_bubbleList);
    }
}

void ExpandAnimation::expandData(const QList<EntityPtr> &notifications)
{
    int index = 0;
    qreal scal_ratio = 19 / 20;
    int height_init = 18;

    QPoint end;
    QPoint start;
    QPoint up_point(0, OSD::BubbleSize(OSD::BUBBLEWIDGET).height());

    foreach (auto entity, notifications) {
        if (index == 0) {
            BubbleItem *bubble = new BubbleItem(this, entity);
            bubble->show();
            m_bubbleList.push_front(bubble);
        } else if (index >= 1 && index < notifications.size() - BubbleOverLap) {
            BubbleItem *bubble = new BubbleItem(this, entity);
            bubble->lower();
            bubble->show();
            m_bubbleList.push_front(bubble);

            end = QPoint(0, index * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * index);
            start = QPoint(0, 0);
            QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
            move_ani->setEasingCurve(QEasingCurve::OutCubic);
            move_ani->setDuration(500);
            move_ani->setStartValue(start);
            move_ani->setEndValue(end);

            QPropertyAnimation *opacity_ani = new QPropertyAnimation(bubble, "windowOpacity", this);
            opacity_ani->setEasingCurve(QEasingCurve::OutCubic);
            opacity_ani->setDuration(500);
            opacity_ani->setStartValue(0);
            opacity_ani->setEndValue(1);

            m_animationGroup->addAnimation(move_ani);
            m_animationGroup->addAnimation(opacity_ani);
        } else {
            height_init -= 6;
            BubbleItem *bubble = new BubbleItem(this, entity);
            bubble->setFixedSize(OSD::BubbleSize(OSD::BUBBLEWIDGET).width() * scal_ratio, height_init);
            int lr_margin = OSD::BubbleSize(OSD::BUBBLEWIDGET).width() - bubble->width() / 2;
            start = QPoint(lr_margin, up_point.y());
            end = QPoint(0, index * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * index);
            up_point = QPoint(start.x(), start.y() + height_init);

            QRect start_rect(start, bubble->size());
            QRect end_rect(end, OSD::BubbleSize(OSD::BUBBLEWIDGET));

            QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "geometry", this);
            move_ani->setEasingCurve(QEasingCurve::OutCubic);
            move_ani->setDuration(500);
            move_ani->setStartValue(start_rect);
            move_ani->setEndValue(end_rect);

            m_animationGroup->addAnimation(move_ani);
        }

        scal_ratio = (scal_ratio * 19) / 20;
        index ++;
    }
}

void ExpandAnimation::appendData(const QList<EntityPtr> &notifications)
{
    int index = 0;

    foreach (auto entity, notifications) {
        BubbleItem *bubble = new BubbleItem(this, entity);
        bubble->show();
        m_bubbleList.push_front(bubble);

        QPoint start(0, (index - 1) * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * (index - 1));
        QPoint end(0, index * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin);

        QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
        move_ani->setEasingCurve(QEasingCurve::OutCubic);
        move_ani->setDuration(500);
        move_ani->setStartValue(start);
        move_ani->setEndValue(end);

        m_animationGroup->addAnimation(move_ani);

        index ++;
    }
}

void ExpandAnimation::removeData(const QList<EntityPtr> &notifications)
{
    QPoint end(OSD::BubbleWidth(OSD::BUBBLEWIDGET), 0);
    QPoint start(0, 0);

    int index = 0;
    foreach (auto entity, notifications) {
        if (index == 0) {
            BubbleItem *bubble = new BubbleItem(this, entity);
            bubble->show();
            m_bubbleList.push_front(bubble);

            QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
            move_ani->setEasingCurve(QEasingCurve::OutCubic);
            move_ani->setDuration(500);
            move_ani->setStartValue(start);
            move_ani->setEndValue(end);

            QPropertyAnimation *opacity_ani = new QPropertyAnimation(bubble, "windowOpacity", this);
            opacity_ani->setEasingCurve(QEasingCurve::OutCubic);
            opacity_ani->setDuration(500);
            opacity_ani->setStartValue(1);
            opacity_ani->setEndValue(0);

            m_animationGroup->addAnimation(move_ani);
            m_animationGroup->addAnimation(opacity_ani);
        } else {
            BubbleItem *bubble = new BubbleItem(this, entity);
            bubble->show();
            m_bubbleList.push_front(bubble);

            start = QPoint(0, index * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * index);
            end = QPoint(0, (index - 1) * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * (index - 1));

            QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
            move_ani->setEasingCurve(QEasingCurve::OutCubic);
            move_ani->setDuration(500);
            move_ani->setStartValue(start);
            move_ani->setEndValue(end);

            m_animationGroup->addAnimation(move_ani);
        }

        index ++;
    }
}
