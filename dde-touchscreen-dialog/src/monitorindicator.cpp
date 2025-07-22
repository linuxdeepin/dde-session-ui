// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "monitorindicator.h"

#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

#include <X11/extensions/shape.h>

MonitorIndicator::MonitorIndicator(QWidget *parent)
    : QFrame(parent)
{
    setAccessibleName("MonitorIndicator");
    setWindowFlags(Qt::SplashScreen | Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
}

void MonitorIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 设置蓝色边框
    QPen pen(QColor("#2ca7f8"));
    pen.setWidth(10); // 设置边框宽度
    painter.setPen(pen);
    
    // 绘制矩形边框，留出一些边距
    QRect borderRect = rect().adjusted(5, 5, -5, -5);
    painter.drawRect(borderRect);
}

void MonitorIndicator::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);
    update(); // 触发重绘
}
