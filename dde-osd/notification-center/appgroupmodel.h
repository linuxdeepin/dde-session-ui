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

#ifndef APPGROUPMODEL_H
#define APPGROUPMODEL_H

#include "notifymodel.h"
#include "notification/notificationentity.h"

#include <QAbstractListModel>
#include <QPointer>
#include <QListView>
#include <memory>

class Persistence;
class QListView;

class ApplicationGroup : public QObject
{
    Q_OBJECT
public:
    ApplicationGroup(std::shared_ptr<NotificationEntity> entity, Persistence *database);

    QVariant notifyModel() { return QVariant::fromValue(m_notifyModel); }
    QString appName() { return m_appName; }
    QString timeStamp() { return m_timeStamp; }

private:
    std::shared_ptr<NotifyModel> m_notifyModel;
    QString m_appName;
    QString m_timeStamp;
};

class AppGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotifyDataRole {
        ApplicationNameRole = Qt::UserRole + 1,
        TimeStampRole = Qt::UserRole + 2,
        NotifyModelRole = Qt::UserRole + 3
    };

    AppGroupModel(QObject *parent = nullptr, Persistence *database = nullptr);
    void setView(QListView *view) { m_view = view; }
    void removeGroup(const QModelIndex &index);

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    void initData();
    void addNotify(std::shared_ptr<NotificationEntity> entity);
    QPointer<ApplicationGroup> appGroup(const QString &app_name);

private:
    QListView *m_view = nullptr;
    Persistence *m_database = nullptr;
    QList<QPointer<ApplicationGroup>> m_applications;
};

#endif // APPGROUPMODEL_H
