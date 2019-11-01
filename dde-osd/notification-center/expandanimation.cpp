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
#include  "notifycommon.h"

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

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

void ExpandAnimation::addData(std::shared_ptr<NotificationEntity> entity)
{
    BubbleItem *bubble = new BubbleItem(this, entity);
    bubble->show();
    m_bubbleList.push_front(bubble);

    int index = 0;
    while (index < m_bubbleList.size()) {
        QPoint end(0, index * Notify::BubbleItemHeight + Notify::CenterMargin * index);
        QPoint start(0, end.y() - Notify::BubbleItemHeight - Notify::CenterMargin * index);
        QPointer<BubbleItem> bubble = m_bubbleList.at(index);
        if (bubble != nullptr) {
            QPropertyAnimation *move_ani = new QPropertyAnimation(bubble, "pos", this);
            move_ani->setEasingCurve(QEasingCurve::OutCubic);
            move_ani->setDuration(200);
            move_ani->setStartValue(start);
            move_ani->setEndValue(end);
            m_animationGroup->addAnimation(move_ani);
        }
        index ++;
    }

    m_animationGroup->start();
}
