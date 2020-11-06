/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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

#include "itemdelegate.h"
#include "bubbletitlewidget.h"
#include "bubbleitem.h"
#include "../notification/constants.h"
#include "overlapwidet.h"
#include "notifylistview.h"

#include <QDebug>

ItemDelegate::ItemDelegate(NotifyListView *view, NotifyModel *model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_model(model)
    , m_view(view)
{
}

QWidget *ItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    EntityPtr notify = index.data().value<EntityPtr>();
    if (!notify) {
        return nullptr;
    }
    if(notify->isTitle())
    {
        BubbleTitleWidget *titleWidget = new BubbleTitleWidget(m_model, notify, parent);
        titleWidget->setParentView(m_view);
        return titleWidget;
    }
    else if(notify->hideCount() != 0)
    {
        OverLapWidet *widget = new OverLapWidet(m_model, notify, parent);
        widget->setParentView(m_view);
        return widget;
    }
    else {
        BubbleItem *bubble = new BubbleItem(parent, notify);
        bubble->setParentModel(m_model);
        bubble->setParentView(m_view);
        return bubble;
    }
}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    EntityPtr notify = index.data().value<EntityPtr>();

    QSize bubbleSize = OSD::BubbleSize(OSD::ShowStyle::BUBBLEWIDGET);
    bubbleSize.setHeight(bubbleSize.height() + BubbleSpacing);

    if(notify && notify->isTitle())
    {
        return QSize(380,50);
    }
    else if(notify->hideCount() != 0)
    {
        return bubbleSize + QSize(0,notify->hideCount()*10);
    }
    else {
        return bubbleSize;
    }
}

void ItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    QRect rect = option.rect;
    QSize size = sizeHint(option, index);
    editor->setGeometry(rect.x(), rect.y(), size.width(), size.height() - BubbleSpacing);
}
