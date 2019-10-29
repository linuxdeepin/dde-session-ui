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

#include "notifymodel.h"
#include "notification/notificationentity.h"

NotifyModel::NotifyModel(QObject *parent, std::shared_ptr<NotificationEntity> notify)
    : QAbstractListModel(parent)
{
    addNotify(notify);
}

int NotifyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_notfications.size();
}

QVariant NotifyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() && m_notfications.size() <= 0 && index.row() > m_notfications.size()) {
        return QVariant();
    }

    std::shared_ptr<NotificationEntity> notify = m_notfications.at(index.row());

    switch (role) {
    case NotifyIdRole:
        return notify->id();

    case NotifyNameRole:
        return notify->appName();

    case NotifySummaryRole:
        return notify->summary();

    case NotifyBodyRole:
        return notify->body();

    case NotifyIconRole:
        return notify->appIcon();

    case NotifyTimeRole:
        return notify->ctime();

    case NotifyEntityRole:
        return QVariant::fromValue(notify);

    default:
        break;
    }

    return QVariant();
}

Qt::ItemFlags NotifyModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (m_view != nullptr) {
            m_view->openPersistentEditor(index);
        }
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index);
}

void NotifyModel::addNotify(std::shared_ptr<NotificationEntity> entity)
{
    beginResetModel();
    m_notfications.push_front(entity);
    endResetModel();
}
