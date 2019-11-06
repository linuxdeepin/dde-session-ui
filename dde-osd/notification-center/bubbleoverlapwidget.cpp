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
    : QFrame(parent)
    , m_notifications(entitys)
    , m_notifyModel(model)
{
    setAttribute(Qt::WA_TranslucentBackground);
    initOverlap();
}

void BubbleOverlapWidget::initOverlap()
{
    qreal scal_ratio = 1;
    int  point_extra = 12;
    QSize standard_size = OSD::BubbleSize(OSD::BUBBLEWIDGET);
    int index = 0;
    foreach (auto notify, m_notifications) {
        BubbleItem *bubble = new BubbleItem(this, notify);

        //Set the middle layer alpha
        if (index % 2 == 1) bubble->setAlpha(25);

        if (m_notifyModel != nullptr)
            bubble->setModel(m_notifyModel);

        QSize size = QSize(bubble->width() * scal_ratio, bubble->height() * scal_ratio);
        bubble->setFixedSize(size);
        int tb_margin = (standard_size.height() - bubble->height()) / 2;
        int lr_margin = (standard_size.width() - bubble->width()) / 2;
        if (lr_margin != 0 && tb_margin != 0) {
            QPoint point(lr_margin, 2 * tb_margin  + point_extra);
            bubble->clearContent();
            bubble->move(point);
            bubble->lower();

            point_extra = point_extra + 10;
        }

        scal_ratio = (scal_ratio * 19) / 20;
        index ++;
    }
}
