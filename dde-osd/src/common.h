/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef COMMON_H
#define COMMON_H

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QDebug>

static const int ImageTextItemWidth = 338;
static const int ImageTextItemHeight = 60;
static const int TextItemWidth = 120;
static const int TextItemHeight = 40;

/*!
 * \~chinese \class DrawHelper
 * \~chinese \brief osd辅助绘图类
 */
class DrawHelper {
public:
    // 绘制图片
    static void DrawImage(QPainter *painter, const QStyleOptionViewItem &option, const QString &iconName, bool isLightTheme);
    //绘制文字
    static void DrawText(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, QColor color = Qt::black, bool withImage = true);
    //绘制进度条
    static void DrawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, double progress, const QColor color);
    // 绘制百分比数据
    static void DrawPercentValue(QPainter *painter, const QStyleOptionViewItem &option, double value, const QColor &color);
    // 绘制背景
    static void DrawBackground(QPainter *painter, const QStyleOptionViewItem &option);
};

#endif // COMMON_H
