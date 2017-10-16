/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "systemmonitor.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QPaintEvent>
#include <QApplication>
#include <QScreen>
#include <QDebug>

SystemMonitor::SystemMonitor(QWidget *parent) : QWidget(parent)
{
    m_state = Leave;

    const qreal ratio = devicePixelRatioF();

    m_icon = new QWidget;
    m_icon->installEventFilter(this);
    m_icon->setFixedSize(24, 24);

    m_text = new QLabel(tr("Start system monitor"));
    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_text->adjustSize();
    setFixedHeight(40);
    setFixedWidth(24 + 10 + 60 + m_text->width());

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setContentsMargins(30, 0, 30, 0);

    layout->addWidget(m_icon, 0, Qt::AlignVCenter);
    layout->addSpacing(10);
    layout->addWidget(m_text, 0, Qt::AlignVCenter);

    setLayout(layout);
}

void SystemMonitor::setState(SystemMonitor::State state)
{
    m_state = state;
    update();
}

void SystemMonitor::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Enter;
    update();
}

void SystemMonitor::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Leave;
    update();
}

void SystemMonitor::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    m_text->setStyleSheet("color: white;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Release;
    update();

    emit clicked();
}

void SystemMonitor::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);

    m_text->setStyleSheet("color: #2ca7f8;"
                          "font-size: 14px;"
                          "font-weight: 400;");

    m_state = Press;
    update();

    event->accept();
}

bool SystemMonitor::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_icon) {
        if (event->type() == QEvent::Paint) {
            QPainter painter(m_icon);
            QSvgRenderer render(QString(":/img/deepin-system-monitor.svg"), m_icon);
            render.render(&painter, m_icon->rect());
        }
    }

    return false;
}

void SystemMonitor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPainterPath path;
    path.addRoundedRect(rect(), 5, 5);
    painter.setClipPath(path);

    switch(m_state) {
    case Enter:
    case Release:{
        painter.fillRect(rect(), QColor(255, 255, 255, 0.1 * 255));
        break;
    }
    case Press: {
        painter.fillRect(rect(), QColor(255, 255, 255, 0.05 * 255));
        break;
    }
    case Leave:
    default:
        break;
    }

}

void SystemMonitor::keyPressEvent(QKeyEvent *event)
{
    if (m_state == Enter && (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)) {
        event->accept();
        emit clicked();
    }
}
