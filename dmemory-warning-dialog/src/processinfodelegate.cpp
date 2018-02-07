/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#include "processinfodelegate.h"
#include "processinfomodel.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>

ProcessInfoDelegate::ProcessInfoDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ProcessInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.row() % 2)
        painter->fillRect(option.rect, QColor(0, 0, 0, 255 * 0.05));

    if (index.column() != COLUMN_ICON)
        return QStyledItemDelegate::paint(painter, option, index);

    const QPixmap pix = index.data(ProcessInfoModel::IconRole).value<QPixmap>();
    painter->drawPixmap(option.rect.center() - pix.rect().center() / pix.devicePixelRatio(), pix);
}
