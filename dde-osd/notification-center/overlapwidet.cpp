/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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

#include "overlapwidet.h"
#include "bubbleitem.h"
#include "../notification/constants.h"
#include "notifylistview.h"
#include "notifymodel.h"

#include <QTimer>
#include <QKeyEvent>
#include <QPainterPath>

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
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);//消除锯齿

    QPalette pe = this->palette();//得到此类的调色板
    QColor brushColor(pe.color(QPalette::Base));
    brushColor.setAlpha(m_hasFocus ? m_hoverAlpha : m_unHoverAlpha);
    painter.setBrush(brushColor);

    int radius = 6;

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
}

OverLapWidet::OverLapWidet(NotifyModel *model, EntityPtr ptr, QWidget *parent)
    : DWidget(parent),
      m_entify(ptr),
      m_model(model)
{
    initOverlap();
}

void OverLapWidet::setParentView(NotifyListView *view)
{
    m_view = view;
    m_faceBubbleItem->setParentView(view);
}

QList<QPointer<QWidget> > OverLapWidet::bubbleElements()
{
    return m_faceBubbleItem->bubbleElements();
}

void OverLapWidet::expandAppGroup()
{
    hideOverlapBubble();
    ListItem appItem = m_model->getAppData(m_entify->appName());
    m_view->createExpandAnimation(m_entify->currentIndex(), appItem);
}

void OverLapWidet::initOverlap()
{
    m_faceBubbleItem = new BubbleItem(this, m_entify);
    m_faceBubbleItem->setAccessibleName("FaceBubbleItem");
    connect(m_faceBubbleItem, &BubbleItem::bubbleRemove, this, &OverLapWidet::hideOverlapBubble);
    m_faceBubbleItem->setOverlapWidget(true);
    m_faceBubbleItem->setParentModel(m_model);
    setFocusProxy(m_faceBubbleItem);
    const int bubbleItemHeight = m_faceBubbleItem->bubbleItemHeight();

    qreal scal_ratio = 1;
    int height_init = BubbleOverLapHeight;
    QSize standard_size = OSD::BubbleSize(OSD::BUBBLEWIDGET);
    standard_size.setHeight(bubbleItemHeight);
    QPoint up_point(0, standard_size.height());

    for(int i = 0; i < MIN(3,m_entify->hideCount()); ++i)
    {
        HalfRoundedRectWidget *bubble = new HalfRoundedRectWidget(this);
        bubble->setAccessibleName("HalfRoundedRectBubble");
        scal_ratio = (scal_ratio * 19) / 20;

        height_init -= 2;
        bubble->setFixedSize(int(standard_size.width() * scal_ratio), height_init);
        int lr_margin = (standard_size.width() - bubble->width()) / 2;
        QPoint move_point(lr_margin, up_point.y());
        up_point = QPoint(move_point.x(), move_point.y() + height_init);
        bubble->move(move_point);
        bubble->lower();
        bubble->setFocusPolicy(Qt::NoFocus);
    }
}

void OverLapWidet::hideOverlapBubble()
{
    resize(m_faceBubbleItem->size());
}

void OverLapWidet::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_view->aniState())
        expandAppGroup();
    return QWidget::mouseReleaseEvent(event);
}

void OverLapWidet::focusInEvent(QFocusEvent *event)
{
    m_faceBubbleItem->setHasFocus(true);
    return DWidget::focusOutEvent(event);
}

void OverLapWidet::focusOutEvent(QFocusEvent *event)
{
    m_faceBubbleItem->setHasFocus(false);
    return DWidget::focusOutEvent(event);
}
