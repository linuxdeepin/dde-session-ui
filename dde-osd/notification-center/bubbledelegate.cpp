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
#include "bubbleoverlapwidget.h"
#include "notification/notificationentity.h"
#include "notification/constants.h"

#include <QDebug>

BubbleDelegate::BubbleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *BubbleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    std::shared_ptr<NotificationEntity> notify = index.data(NotifyModel::NotifyEntityRole).value<std::shared_ptr<NotificationEntity>>();
    NotifyModel *model = const_cast<NotifyModel *>(dynamic_cast<const NotifyModel *>(index.model()));
    Q_ASSERT(model);

    if (notify == nullptr) {
        return nullptr;
    }

    if (model->isShowOverlap() && index.row() == BubbleEntities - 1) {
        BubbleOverlapWidget *overlap_widget = new BubbleOverlapWidget(parent, notify, model);
        return overlap_widget;
    } else {
        BubbleItem *bubble = new BubbleItem(parent, notify);
        bubble->setModel(model);
        return bubble;
    }
}

QSize BubbleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    NotifyModel *model = const_cast<NotifyModel *>(dynamic_cast<const NotifyModel *>(index.model()));
    QSize size = OSD::BubbleSize(OSD::ShowStyle::BUBBLEWIDGET);
    if (model->isShowOverlap() && index.row() == BubbleEntities - 1) size.setHeight(size.height() + 20);
    return size;
}

void BubbleDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    QRect rect = option.rect;
    QSize size = sizeHint(option, index);
    if (index.row() > 0) rect.setY(rect.y() + Notify::CenterMargin * index.row());
    editor->setGeometry(rect.x(), rect.y(), size.width(), size.height());
}
