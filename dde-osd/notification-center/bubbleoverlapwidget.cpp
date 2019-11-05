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

#include "bubbleoverlapwidget.h"
#include "notification/notificationentity.h"
#include "notification/constants.h"


BubbleOverlapWidget::BubbleOverlapWidget(QWidget *parent, std::shared_ptr<NotificationEntity> entity,
                                         NotifyModel *model)
    : QFrame(parent)
    , m_notify(entity)
    , m_notifyModel(model)
{
    setAttribute(Qt::WA_TranslucentBackground);
    initOverlap();
}

void BubbleOverlapWidget::initOverlap()
{
    if (m_notify == nullptr) return;

    m_topBubble = new BubbleItem(this, m_notify);
    m_topBubble->setModel(m_notifyModel);
    m_firstOverlap = new BubbleItem(this);
    m_secondOverlap = new BubbleItem(this);

    m_firstOverlap->setFixedSize(QSize(m_topBubble->width() / 20 * 18, m_topBubble->height() / 20 * 18));

    QPoint p = QPoint(x() + OSD::BubbleWidth(OSD::BUBBLEWIDGET) / 20,
                      y() + m_topBubble->height() - OSD::BubbleHeight(OSD::BUBBLEWIDGET) * 2 / 3);

    m_firstOverlap->move(p);
    m_firstOverlap->lower();

    m_secondOverlap->setFixedSize(QSize(m_firstOverlap->width() / 20 * 18, m_firstOverlap->height() / 20 * 18));
    m_secondOverlap->move(QPoint(m_firstOverlap->x() + m_firstOverlap->width() / 20,
                                 m_firstOverlap->y() + m_firstOverlap->height()  - m_secondOverlap->height() * 2 / 3));
    m_secondOverlap->lower();
}
