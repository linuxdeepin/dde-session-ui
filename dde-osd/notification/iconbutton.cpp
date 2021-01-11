/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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


#include "iconbutton.h"

#include <QPainter>
#include <QIcon>
#include <QStyle>
#include <QDebug>
#include <QMouseEvent>

IconButton::IconButton(QWidget *parent)
    : DWidget(parent)
    , m_text("")
    , m_hasFocus(false)
    , m_hover(false)
    , m_opacity(0)
    , m_radius(8)
    , m_hasBackColor(false)
{

}

IconButton::IconButton(const QString &text, QWidget *parent)
    : IconButton(parent)
{
    setText(text);
}

void IconButton::setText(const QString &text)
{
    m_text = text;

    update();
}

void IconButton::setIcon(const QString &iconPath)
{
    m_pixmap = QPixmap(iconPath);

    update();
}

void IconButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QColor color;
    if (m_hasBackColor) {
        color = palette().color(QPalette::Base);
        color.setAlpha(m_hover ? 255 * 0.2 : m_opacity);
    } else {
        color = palette().color(QPalette::WindowText);
        color.setAlpha(m_hasFocus ? 80 : (m_hover ? 50 : 20));
    }

    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRoundedRect(rect(), m_radius, m_radius);

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter.setPen(palette().color(QPalette::WindowText));
    painter.drawText(rect(), m_text, option);

    if (m_text.isEmpty()) {
        if (m_pixmap.isNull()) {
            m_pixmap = style()->standardIcon(QStyle::SP_TitleBarCloseButton).pixmap(width());
        }
        painter.drawPixmap(QRect((rect().width() - m_pixmap.width())/ 2,
                                 (rect().height() - m_pixmap.height())/ 2,
                                 m_pixmap.width(), m_pixmap.height()), m_pixmap);
    }
}

void IconButton::setFocusState(bool has)
{
    m_hasFocus = has;

    update();
}

void IconButton::setBackOpacity(int opacity)
{
    if (opacity < 0 || m_opacity > 255)
        return;

    m_opacity = opacity;

    m_hasBackColor = true;

    update();
}

void IconButton::setRadius(int radius)
{
    m_radius = radius;

    update();
}

void IconButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_pressPos = event->pos();
}

void IconButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (rect().contains(m_pressPos) && rect().contains(event->pos()))
        Q_EMIT clicked();
}

void IconButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hover = true;

    update();

    return DWidget::enterEvent(event);
}

void IconButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hover = false;

    update();

    return DWidget::leaveEvent(event);
}
