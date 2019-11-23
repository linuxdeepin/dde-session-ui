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
#include "notification/constants.h"
#include "notification/bubbletool.h"

#include <QDebug>

AppGroupDelegate::AppGroupDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *AppGroupDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    std::shared_ptr<NotifyModel> model = index.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    QString title = BubbleTool::getDeepinAppName(index.data(AppGroupModel::ApplicationNameRole).toString());
    AppGroupModel *grpup_model = const_cast<AppGroupModel *>(dynamic_cast<const AppGroupModel *>(index.model()));
    Q_ASSERT(grpup_model);

    if (model == nullptr) {
        return nullptr;
    }

    BubbleGroup *group = new BubbleGroup(parent, model);
    group->setGroupTitle(title);
    connect(group, &BubbleGroup::closeGroup, grpup_model, [grpup_model, index]() {
        grpup_model->removeGroup(index);
    });
    return group;
}

QSize AppGroupDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    std::shared_ptr<NotifyModel> model = index.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();

    int row = model->rowCount();
    int group_height = row * OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + Notify::GroupTitleHeight + Notify::CenterMargin * row;
    return QSize(OSD::BubbleWidth(OSD::ShowStyle::BUBBLEWIDGET), group_height + Notify::GroupMargin);
}

void AppGroupDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    QRect rect = option.rect;
    QSize size = sizeHint(option, index);
    editor->setGeometry(rect.x(), rect.y(), size.width(), size.height());
}
