/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
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

#ifndef ItemDelegate_H
#define ItemDelegate_H

#include "notifymodel.h"

#include <QStyledItemDelegate>

class NotifyListView;

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(NotifyListView *view = Q_NULLPTR, NotifyModel * model = Q_NULLPTR, QObject *parent = Q_NULLPTR);

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    NotifyModel *m_model;
    NotifyListView *m_view;
};

#endif // ItemDelegate_H
