// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef COMMON_H
#define COMMON_H

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QDebug>

#define Pair(x, y) QPair<QString, QString>(x, y)

static const int ImageTextItemWidth = 140;
static const int ImageTextItemHeight = 140;
static const int TextItemWidth = 120;
static const int TextItemHeight = 40;

static const QColor ItemHighlightColor = "#FFFFFF";//"#01bdff";

/*!
 * \~chinese \class DrawHelper
 * \~chinese \brief osd辅助绘图类
 */
class DrawHelper {
public:
    //绘制图片
    static void DrawImage(QPainter *painter, const QStyleOptionViewItem &option, const QString &pix, bool withText = false, bool withProgress = false);
    //绘制文字
    static void DrawText(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, QColor color = Qt::black, bool withImage = true);
    //绘制进度条
    static void DrawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, double progress, const QColor color);
    //绘制中心的数字
    static void DrawCenterNum(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, const bool isCurrent);
    //绘制背景
    static void DrawBackground(QPainter *painter, const QStyleOptionViewItem &option);
    //绘制进度条刻度
    static void DrawVolumeGraduation(QPainter *painter, const QStyleOptionViewItem &option, const QColor color);
};

#endif // COMMON_H
