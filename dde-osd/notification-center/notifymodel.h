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

#ifndef NOTIFYMODEL_H
#define NOTIFYMODEL_H

#include <QAbstractListModel>
#include <QListView>
#include <QDateTime>
#include <QPointer>
#include <memory>

class NotificationEntity;
class Persistence;

class NotifyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotifyDataRole {
        NotifyIdRole = Qt::UserRole + 1,
        NotifyNameRole = Qt::UserRole + 2,
        NotifySummaryRole = Qt::UserRole + 3,
        NotifyBodyRole = Qt::UserRole + 4,
        NotifyIconRole = Qt::UserRole + 5,
        NotifyTimeRole = Qt::UserRole + 6,
        NotifyEntityRole = Qt::UserRole + 7
    };

    NotifyModel(QObject *parent = nullptr, std::shared_ptr<NotificationEntity> notify = nullptr);
    void addNotify(std::shared_ptr<NotificationEntity> entity);
    void setView(QListView *view) { m_view = view; }
    int rowCount() { return m_displays.size(); }
    void removeNotify(std::shared_ptr<NotificationEntity> entity);
    void setPersistence(Persistence *db) { m_database = db; }

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QListView *m_view = nullptr;
    Persistence *m_database = nullptr;
    QList<std::shared_ptr<NotificationEntity>> m_notfications;
    QList<std::shared_ptr<NotificationEntity>> m_displays;
};

Q_DECLARE_METATYPE(std::shared_ptr<NotifyModel>);

#endif // NOTIFYMODEL_H
