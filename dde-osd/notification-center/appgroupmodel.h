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
/*!
 * \~chinese \class ApplicationGroup
 * \~chinese \brief App组的信息类
 */
class ApplicationGroup : public QObject
{
    Q_OBJECT
public:
    ApplicationGroup(EntityPtr entity, Persistence *database);

    QVariant notifyModel() { return QVariant::fromValue(m_notifyModel); }
    QString appName() { return m_appName; }
    QString timeStamp() { return m_timeStamp; }

signals:
    void removeGroup();
    void layoutGroup();

private:
    std::shared_ptr<NotifyModel> m_notifyModel;
    QString m_appName;
    QString m_timeStamp;
};

/*!
 * \~chinese \class AppGroupModel
 * \~chinese \brief MVC模型中的Model类,app组的Model,继承自QAbstractListModel,用于从数据库中获取数据.
 */
class AppGroupModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NotifyDataRole {
        ApplicationNameRole = Qt::UserRole + 1,
        TimeStampRole = Qt::UserRole + 2,
        NotifyModelRole = Qt::UserRole + 3,
        GroupViewRole = Qt::UserRole + 4
    };

    AppGroupModel(QObject *parent = nullptr, Persistence *database = nullptr);
    void setView(QListView *view) { m_view = view; }
    QListView *view() { return m_view; }
    void removeGroup(std::shared_ptr<NotifyModel> model); //移除一个app组的通知
    QList<QPointer<ApplicationGroup>> appGroups() { return m_applications; }

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void dataChanged();                                 //数据库有添加数据时发送该信号
    void currentIndexChanged(const QModelIndex &index);
    void currentIndexChanged_(const QModelIndex &index1, const QModelIndex &index2);
    void setScrollBarValue(int value);

private:
    void initData();                                    //初始化数据
    int indexOf(std::shared_ptr<NotifyModel> model);    //返回有多少个App组
    void addNotify(EntityPtr entity);                   //添加通知
    QPointer<ApplicationGroup> appGroup(const QString &app_name); //根据App名称查找App组

private:
    QListView *m_view = nullptr;
    Persistence *m_database = nullptr;
    QList<QPointer<ApplicationGroup>> m_applications;
};

#endif // APPGROUPMODEL_H
