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
    : QFrame(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    m_animationGroup = new QSequentialAnimationGroup(this);
    connect(m_animationGroup, &QSequentialAnimationGroup::finished, this, &ExpandAnimation::finished);
}

ExpandAnimation::~ExpandAnimation()
{
    if (m_animationGroup != nullptr) {
        delete m_animationGroup;
    }

    if (!m_bubbleList.isEmpty()) {
        qDeleteAll(m_bubbleList);
    }
}

void ExpandAnimation::addData(const QList<std::shared_ptr<NotificationEntity>> &notifications)
{
    int index = 0;
    foreach (auto entity, notifications) {
        BubbleItem *bubble = new BubbleItem(this, entity);
        bubble->lower();
        bubble->show();
        m_bubbleList.push_front(bubble);

        QPoint end(0, index * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::CenterMargin * index);
        QPoint start(0, end.y() - OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) - Notify::CenterMargin * index);
        QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
        move_ani->setEasingCurve(QEasingCurve::OutCubic);
        move_ani->setDuration(500);
        move_ani->setStartValue(start);
        move_ani->setEndValue(end);
        m_animationGroup->addAnimation(move_ani);
        index ++;
    }
}
