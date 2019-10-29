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

#include "appgroupdelegate.h"
#include "appgroupmodel.h"
#include "bubblegroup.h"
#include "notifycommon.h"

#include <QDebug>

AppGroupDelegate::AppGroupDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *AppGroupDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    std::shared_ptr<NotifyModel> model = index.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    QString title = index.data(AppGroupModel::ApplicationNameRole).toString();

    if (model == nullptr) {
        return nullptr;
    }

    BubbleGroup *group = new BubbleGroup(parent, model);
    group->setGroupTitle(title);
    return group;
}

QSize AppGroupDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    std::shared_ptr<NotifyModel> model = index.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();

    int row = model->rowCount();
    return QSize(Notify::BubbleItemWidth,  row * Notify::BubbleItemHeight + Notify::GroupTitleHeight + Notify::CenterMargin * row);
}
