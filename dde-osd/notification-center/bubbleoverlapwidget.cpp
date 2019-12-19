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
#include "shortcutmanage.h"
#include "notification/notificationentity.h"
#include "notification/constants.h"
#include <QPointer>

HalfRoundedRectWidget::HalfRoundedRectWidget(QWidget *parent)
    : AlphaWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setFixedSize(OSD::BubbleSize(OSD::BUBBLEWIDGET));

    setHoverAlpha(Notify::BubbleDefaultAlpha * 3);
    setUnHoverAlpha(Notify::BubbleDefaultAlpha * 2);
}

void HalfRoundedRectWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPalette pe = this->palette();
    QColor brushColor(pe.color(QPalette::Base));
    brushColor.setAlpha(m_hasFocus ? m_hoverAlpha : m_unHoverAlpha);
    painter.setBrush(brushColor);

    DStyleHelper dstyle(style());
    int radius = dstyle.pixelMetric(DStyle::PM_FrameRadius);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter.setPen(borderPen);

    QRect rect = this->rect();
    rect.setWidth(rect.width());
    rect.setHeight(rect.height());

    QPainterPath path;
    path.moveTo(rect.bottomRight() - QPointF(0, radius));
    path.lineTo(rect.topRight().x(), rect.topRight().y() / 2);
    path.lineTo(rect.topLeft().x(), rect.topLeft().y() / 2);
    path.lineTo(rect.bottomLeft() - QPointF(0, radius));
    path.arcTo(QRectF(QPointF(rect.bottomLeft() - QPointF(0, radius * 2)), QSize(radius * 2, radius * 2)), 180, 90);
    path.lineTo(rect.bottomRight() + QPointF(radius, 0));
    path.arcTo(QRectF(QPointF(rect.bottomRight() - QPointF(radius * 2, radius * 2)), QSize(radius * 2, radius * 2)), 270, 90);

    painter.drawPath(path);
    return AlphaWidget::paintEvent(event);
}

BubbleOverlapWidget::BubbleOverlapWidget(const QList<std::shared_ptr<NotificationEntity>> &entitys,
                                         QWidget *parent, NotifyModel *model)
    : QWidget(parent)
    , m_notifications(entitys)
    , m_notifyModel(model)
{
    setFocusPolicy(Qt::StrongFocus);
    initOverlap();
}

BubbleOverlapWidget::~BubbleOverlapWidget()
{
}

void BubbleOverlapWidget::initOverlap()
{
    qreal scal_ratio = 1;
    int height_init = BubbleOverLapHeight;
    int index = 0;
    QSize standard_size = OSD::BubbleSize(OSD::BUBBLEWIDGET);
    QPoint up_point(0, standard_size.height());

    foreach (auto notify, m_notifications) {
        if (index >= 1) {
            HalfRoundedRectWidget *bubble = new HalfRoundedRectWidget(this);

            height_init -= 2;
            bubble->setFixedSize(standard_size.width() * scal_ratio, height_init);
            int lr_margin = (standard_size.width() - bubble->width()) / 2;
            QPoint move_point(lr_margin, up_point.y());
            up_point = QPoint(move_point.x(), move_point.y() + height_init);
            bubble->move(move_point);
            bubble->lower();
            bubble->setFocusPolicy(Qt::NoFocus);
        } else {
            m_faceBubbleItem = new BubbleItem(this, notify);

            if (m_notifyModel != nullptr)
                m_faceBubbleItem->setModel(m_notifyModel);
            setFocusProxy(m_faceBubbleItem);
        }

        scal_ratio = (scal_ratio * 19) / 20;
        index ++;
    }
}
