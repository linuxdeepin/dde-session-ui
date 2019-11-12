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
#include <QPointer>


BubbleOverlapWidget::BubbleOverlapWidget(const QList<std::shared_ptr<NotificationEntity>> &entitys,
                                         QWidget *parent, NotifyModel *model)
    : QWidget(parent)
    , m_notifications(entitys)
    , m_notifyModel(model)
{
    initOverlap();
}

void BubbleOverlapWidget::initOverlap()
{
    qreal scal_ratio = 1;
    int height_init = 12;
    int index = 0;
    QSize standard_size = OSD::BubbleSize(OSD::BUBBLEWIDGET);
    QPoint up_point(0, standard_size.height());

    foreach (auto notify, m_notifications) {
        BubbleItem *bubble = new BubbleItem(this, notify);

        if (m_notifyModel != nullptr)
            bubble->setModel(m_notifyModel);

        if (index >= 1) {
            height_init -= 2;
            bubble->setFixedSize(standard_size.width() * scal_ratio, height_init);
            int lr_margin = (standard_size.width() - bubble->width()) / 2;
            QPoint move_point(lr_margin, up_point.y());
            up_point = QPoint(move_point.x(), move_point.y() + height_init);
            bubble->setShowContent(false);
            bubble->move(move_point);
            bubble->lower();
        }

        scal_ratio = (scal_ratio * 19) / 20;
        index ++;
    }
}
