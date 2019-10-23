/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     fanpengcheng <fanpengcheng_cm@deepin.com>
 *
 * Maintainer: fanpengcheng <fanpengcheng_cm@deepin.com>
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
#include "button.h"

#include <QAction>
#include <QDebug>
#include <QFont>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPushButton>
#include <QPainter>
#include <QPaintEvent>
#include <QSvgRenderer>
#include <QVBoxLayout>

Button::Button(QWidget *parent)
    : QWidget(parent)
    , m_text("Button")
    , m_radius(0)
    , m_in(false)
    , m_hasMenu(false)
    , m_align(Qt::AlignCenter)
    , m_menu(new QMenu(this))
{

}

void Button::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;

    update();
}

void Button::setText(const QString &text)
{
    m_text = text;

    update();
}

void Button::setId(const QString &id)
{
    m_id = id;
}

void Button::setTextAlignment(Qt::Alignment align)
{
    m_align = align;

    update();
}

void Button::setRadius(int radius)
{
    m_radius = radius;

    update();
}

void Button::addAction(QAction *action)
{
    m_menu->addAction(action);

    if (!m_hasMenu) {
        m_hasMenu = true;
    }
}

void Button::clear()
{
    m_menu->clear();

    m_hasMenu = false;
}

void Button::hideMenu()
{
    m_menu->hide();
}

void Button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        if (m_contentRect.contains(mapFromGlobal(QCursor::pos()))) {
            Q_EMIT clicked();
            Q_EMIT toggled(m_id);
        } else if (m_hasMenu && m_menuRect.contains(mapFromGlobal(QCursor::pos()))) {
            QWidget *w = qobject_cast<QWidget *>(m_menu->parent());
            if (w) {
                //unable to determine QMenu's size before it is displayed.
                m_menu->show();
                QPoint p;
                p.setX(w->geometry().x() + w->geometry().width() - m_menu->width());
                p.setY(w->geometry().y() + w->geometry().height());
                m_menu->exec(mapToGlobal(p));
            }
        }
    }

//    return QWidget::mousePressEvent(event);
}

void Button::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRectF rect(0, 0, width(), height());
    if (m_hasMenu) {
        m_contentRect.setX(rect.x());
        m_contentRect.setY(rect.y());
        m_contentRect.setWidth(MIN(rect.width() * 2 / 3, rect.width() - 10));
        m_contentRect.setHeight(rect.height());

        m_menuRect.setX(rect.x() + m_contentRect.width());
        m_menuRect.setY(rect.y());
        m_menuRect.setWidth(rect.width() - m_contentRect.width());
        m_menuRect.setHeight(rect.height());
    } else {
        m_contentRect = rect;
    }

    //先画矩形
    painter.save();
    QColor brushColor(Qt::white);
    brushColor.setAlpha(m_in ? 120 : 60);
    painter.setBrush(brushColor);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter.setPen(borderPen);
    painter.drawRoundRect(rect, m_radius, m_radius);
    painter.restore();

    //绘制图标
    painter.save();

    painter.drawPixmap(m_contentRect, m_pixmap, QRectF());

    painter.restore();

    //绘制文本
    painter.save();

    QTextOption option;
    option.setAlignment(m_align);
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(m_contentRect, m_text, option);
    painter.restore();

    //绘制菜单图标
    painter.save();
    if (m_hasMenu) {
        QSvgRenderer svgRender;
        svgRender.load(QString(":/icons/OSD_menu_down.svg"));

        int xPadding = 5;
        int yPadding = 25;

        QRectF viewRect(m_menuRect.x() + xPadding
                        , m_menuRect.y() + yPadding
                        , m_menuRect.width() - 2 * xPadding,
                        m_menuRect.height() - 2 * yPadding);

        svgRender.render(&painter, viewRect);
    }
    painter.restore();

    return QWidget::paintEvent(event);
}

void Button::enterEvent(QEvent *event)
{
    m_in = true;

    update();

    return QWidget::enterEvent(event);
}

void Button::leaveEvent(QEvent *event)
{
    m_in = false;

    update();

    return QWidget::leaveEvent(event);
}

