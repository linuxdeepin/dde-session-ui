/*
 * Copyright (C) 2020 ~ 2022 Uniontech Technology Co., Ltd.
 *
 * Author:     chenjun <chenjun@uniontech.com>
 *
 * Maintainer: chenjun <chenjun@uniontech.com>
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

#include "horizontalseperator.h"

#include <DApplicationHelper>

#include <QPainter>

/**
 * @brief HorizontalSeperator::HorizontalSeperator 分割线控件,高度值初始化为2个像素
 * @param parent
 */
HorizontalSeperator::HorizontalSeperator(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(2);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

QSize HorizontalSeperator::sizeHint() const
{
    return QSize(QWidget::sizeHint().width(), 2);
}

void HorizontalSeperator::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QPainter painter(this);
    QColor c = palette().color(QPalette::BrightText);
    c.setAlpha(int(0.1 * 255));

    painter.fillRect(rect(), c);
}
