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
#include <QStyleOption>
#include <QVBoxLayout>

ButtonContent::ButtonContent(QWidget *parent)
    : DWidget(parent)
    , m_text("")
    , m_align(Qt::AlignCenter)
{

}

void ButtonContent::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;

    update();
}

void ButtonContent::setText(const QString &text)
{
    m_text = text;

    update();
}

void ButtonContent::setId(const QString &id)
{
    m_id = id;
}

void ButtonContent::setTextAlignment(Qt::Alignment align)
{
    m_align = align;

    update();
}

QSize ButtonContent::sizeHint() const
{
    return QSize(60, 50);
}

QSize ButtonContent::minimumSizeHint() const
{
    return QSize(30, 25);
}

void ButtonContent::drawBackground(QPainter *painter)
{
    painter->save();

    QColor brushColor(Qt::transparent);
    painter->setBrush(brushColor);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter->setPen(borderPen);
    painter->drawRect(QRectF(0, 0, width(), height()));

    painter->restore();
}

void ButtonContent::drawPixmap(QPainter *painter)
{
    painter->save();

    painter->drawPixmap(geometry(), m_pixmap, QRectF());

    painter->restore();
}

void ButtonContent::drawContent(QPainter *painter)
{
    painter->save();

    QTextOption option;
    option.setAlignment(textAlignment());
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawText(geometry(), text(), option);

    painter->restore();
}

void ButtonContent::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

        if (geometry().contains(mapFromGlobal(QCursor::pos()))) {
            Q_EMIT clicked();
            Q_EMIT toggled(m_id);
        }
    }

    return QWidget::mousePressEvent(event);
}

void ButtonContent::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    drawBackground(&painter);
    drawPixmap(&painter);
    drawContent(&painter);

    return QWidget::paintEvent(event);
}

ButtonMenu::ButtonMenu(QWidget *parent)
    : DWidget(parent)
{

}

void ButtonMenu::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    Q_EMIT menuToggled();
}

void ButtonMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_IndicatorArrowDown, &opt, &painter, this);
}

Button::Button(QWidget *parent)
    : DWidget(parent)
    , m_button(new ButtonContent(this))
    , m_menuArea(new ButtonMenu(this))
    , m_menu(new DMenu(this))
    , m_radius(0)
    , m_hover(false)
{
    m_menuArea->setFixedWidth(MIN(15, width() / 3));
    m_menuArea->hide();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(m_button);
    layout->addWidget(m_menuArea);
    setLayout(layout);

    connect(m_button, &ButtonContent::clicked, this, &Button::clicked);
    connect(m_button, &ButtonContent::toggled, this, &Button::toggled);
    connect(m_menuArea, &ButtonMenu::menuToggled, this, &Button::onMenuToggled);
}

void Button::setPixmap(const QPixmap &pixmap)
{
    m_button->setPixmap(pixmap);
}

void Button::setText(const QString &text)
{
    m_button->setText(text);
}

void Button::setId(const QString &id)
{
    m_button->setId(id);
}

void Button::setTextAlignment(Qt::Alignment align)
{
    m_button->setTextAlignment(align);
}

void Button::setRadius(int radius)
{
    m_radius = radius;

    update();
}

void Button::addAction(QAction *action)
{
    m_menu->addAction(action);

    m_menuArea->show();
}

void Button::clear()
{
    m_menu->clear();

    m_menuArea->hide();
}

void Button::setHoverState(bool state)
{
    m_hover = state;

    update();
}

QSize Button::sizeHint() const
{
    return QSize(60, 50);
}

QSize Button::minimumSizeHint() const
{
    return QSize(30, 25);
}

void Button::hideMenu()
{
    m_menu->hide();
}

void Button::onMenuToggled()
{
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

void Button::drawBackground(QPainter *painter)
{
    painter->save();

    QColor brushColor(Qt::white);
    brushColor.setAlpha(m_hover ? 120 : 60);
    painter->setBrush(brushColor);

    QPen borderPen;
    borderPen.setColor(Qt::transparent);
    painter->setPen(borderPen);
    painter->drawRoundRect(QRectF(0, 0, width(), height()), m_radius, m_radius);

    painter->restore();
}

void Button::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    drawBackground(&painter);

    return QWidget::paintEvent(event);
}

void Button::enterEvent(QEvent *event)
{
    setHoverState(true);

    return DWidget::enterEvent(event);
}

void Button::leaveEvent(QEvent *event)
{
    setHoverState(false);

    return DWidget::leaveEvent(event);
}
