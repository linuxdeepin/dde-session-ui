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

#include "appgroupmodel.h"
#include "notification/persistence.h"

ApplicationGroup::ApplicationGroup(std::shared_ptr<NotificationEntity> entity, Persistence *database)
{
    m_appName = entity->appName();
    m_timeStamp = entity->ctime();
    m_notifyModel = std::make_shared<NotifyModel>(this, entity);
    m_notifyModel->setPersistence(database);
    connect(m_notifyModel.get(), &NotifyModel::layoutGroup, this, [ = ]() {
        layoutGroup();

        if (m_notifyModel->rowCount() == 0) removeGroup();
    });
}

AppGroupModel::AppGroupModel(QObject *parent, Persistence *database)
    : QAbstractListModel(parent),
      m_database(database)
{
    initData();
}

void AppGroupModel::initData()
{
    Q_ASSERT(m_database);
    if (m_database == nullptr)  return;
    QList<std::shared_ptr<NotificationEntity>> notifications = m_database->getAllNotify();
    foreach (auto notify, notifications) {
        addNotify(notify);
    }

    Q_EMIT dataChanged();
    connect(m_database, &Persistence::RecordAdded, this, &AppGroupModel::addNotify);
}

int AppGroupModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_applications.size();
}

QVariant AppGroupModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() && m_applications.size() <= 0 && index.row() > m_applications.size()) {
        return QVariant();
    }

    QPointer<ApplicationGroup> application = m_applications.at(index.row());

    switch (role) {
    case ApplicationNameRole:
        return application->appName();

    case TimeStampRole:
        return application->timeStamp();

    case NotifyModelRole:
        return application->notifyModel();

    case GroupViewRole:
        return QVariant::fromValue(m_view);

    default:
        break;
    }

    return QVariant();
}

Qt::ItemFlags AppGroupModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (m_view != nullptr) m_view->openPersistentEditor(index);
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index);
}

QPointer<ApplicationGroup> AppGroupModel::appGroup(const QString &app_name)
{
    foreach (auto app, m_applications) {
        if (app->appName() == app_name) {
            return app;
        }
    }
    return nullptr;
}

void AppGroupModel::addNotify(std::shared_ptr<NotificationEntity> entity)
{
    auto app_group = appGroup(entity->appName());

    if (app_group != nullptr) {
        auto notify_model = app_group->notifyModel().value<std::shared_ptr<NotifyModel>>();
        notify_model->addNotify(entity);

        if (m_applications.first() != app_group) {
            int index = m_applications.indexOf(app_group);
            beginRemoveRows(QModelIndex(), index, index);
            m_applications.removeOne(app_group);
            endRemoveRows();

            beginInsertRows(QModelIndex(), 0, 0);
            m_applications.push_front(app_group);
            endInsertRows();
        }
    } else {
        ApplicationGroup *bubble_group = new ApplicationGroup(entity, m_database);
        beginInsertRows(QModelIndex(), 0, 0);
        m_applications.push_front(bubble_group);
        endInsertRows();

        connect(bubble_group, &ApplicationGroup::layoutGroup, this, [ = ]() {
            this->layoutChanged();
        });

        connect(bubble_group, &ApplicationGroup::removeGroup, this, [ = ]() {
            if (m_applications.contains(bubble_group)) {
                int index = m_applications.indexOf(bubble_group);

                beginRemoveRows(QModelIndex(), index, index);
                m_applications.removeOne(bubble_group);
                endRemoveRows();
            }
        });
    }

    Q_EMIT dataChanged();
}

void AppGroupModel::removeGroup(const QModelIndex &index)
{
    int row = index.row();
    if (row < 0 || row >= m_applications.size()) return;

    beginRemoveRows(QModelIndex(), row, row);
    auto app = m_applications.takeAt(row);
    endRemoveRows();

    //clear database content
    if (m_database != nullptr) m_database->removeApp(app->appName());
}
