/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
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

#include "bubbledelegate.h"
#include "notifymodel.h"
#include "bubbleitem.h"
#include "notifycommon.h"
#include "notification/notificationentity.h"

#include <QDebug>

BubbleDelegate::BubbleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *BubbleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    std::shared_ptr<NotificationEntity> notify = index.data(NotifyModel::NotifyEntityRole).value<std::shared_ptr<NotificationEntity>>();

    if (notify == nullptr) {
        return nullptr;
    }

    BubbleItem *bubble = new BubbleItem(parent, notify);
    return bubble;
}

QSize BubbleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    return QSize(Notify::BubbleItemWidth,  Notify::BubbleItemHeight);
}
