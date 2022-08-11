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

#include "common.h"

#include <DDciIcon>
#include <DFontSizeManager>

#include <QIcon>
#include <QSvgRenderer>
#include <QPainterPath>
#include <QApplication>

DWIDGET_USE_NAMESPACE

void DrawHelper::DrawImage(QPainter *painter, const QStyleOptionViewItem &option, const QString &iconName, bool isLightTheme)
{
    const int margin = 14;
    const QSize iconSize = QSize(32, 32);

    DDciIconPalette palette{option.palette.color(QPalette::Normal, QPalette::ToolTipText),
                            option.palette.color(QPalette::Normal, QPalette::HighlightedText)};
    DDciIcon icon = DDciIcon::fromTheme(iconName);
    qreal ratio = qApp->devicePixelRatio();
    QPixmap pixmap = icon.pixmap(ratio, iconSize.width(), isLightTheme ? DDciIcon::Light : DDciIcon::Dark, DDciIcon::Normal, palette);

    QRect pixMapRect(option.rect.left() + margin, option.rect.top() + margin, iconSize.width(), iconSize.height());
    painter->drawPixmap(pixMapRect, pixmap);
}

void DrawHelper::DrawText(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, QColor color, bool withImage)
{
    QRect rect( option.rect );
    if (withImage) {
        rect.setY(rect.y() + 60);
    }

    QTextOption opt;
    opt.setAlignment(Qt::AlignCenter);

    painter->setPen(color);
    painter->drawText(rect, text, opt);
}

void DrawHelper::DrawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, double progress, const QColor color)
{
    const QRect rect(option.rect);
    const int progressBarWidth = 220;
    const int progressBarHeight = 4;
    const float radius = progressBarHeight / 2.f;
    const int leftMargin = 55;
    const int topMargin = 28;

    painter->setPen(Qt::NoPen);

    QRect dest(rect.x() + leftMargin, rect.y() + topMargin, progressBarWidth, progressBarHeight);
    painter->setOpacity(0.1);
    painter->setBrush(color);
    painter->drawRoundedRect(dest, radius, radius);

    dest.setWidth(dest.width() * progress);
    painter->setOpacity(1);
    painter->setBrush(color);
    painter->drawRoundedRect(dest, radius, radius);
}

void DrawHelper::DrawBackground(QPainter *painter, const QStyleOptionViewItem &option)
{
    QPainterPath path;
    path.addRoundedRect(option.rect.marginsRemoved(QMargins(10, 10, 10, 10)), 5, 5);

    painter->fillPath(path, QColor("#2ca7f8"));
}

void DrawHelper::DrawPercentValue(QPainter *painter, const QStyleOptionViewItem &option, int value)
{
    const int rightMargin = 16;
    const int percentFlagWidth = 9; // %号宽度
    const int percentFlagHeight = 20;
    const int topMargin = 14;
    const int ValueAreaHeight = 26;

    QFont font = DFontSizeManager::instance()->t4();
    font.setPixelSize(font.pixelSize());

    QFontMetrics fm(font);
    int valueAreaWidth = fm.width(QString::number(value));
    QRect valueRect(option.rect.right() - rightMargin - valueAreaWidth - percentFlagWidth, option.rect.top() + topMargin,
                    valueAreaWidth, ValueAreaHeight);
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setFont(font);
    painter->drawText(valueRect, QString::number(value));

    font = DFontSizeManager::instance()->t10();
    painter->setFont(font);

    QRect percentRect(valueRect.right() + 1, valueRect.top() + valueRect.height() / 2, percentFlagWidth, percentFlagHeight);
    painter->drawText(percentRect, "%");
}
