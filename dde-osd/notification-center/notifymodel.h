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

#include "notification/constants.h"

class NotificationEntity;
class Persistence;
/*!
 * \~chinese \class NotifyModel
 * \~chinese \brief MVC模型中的Model类,这个类定义了用于views和delegates访问数据的接口。
 * \~chinese 从数据库中提取通知,提供所有通知气泡的数据
 */
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
        NotifyEntityRole = Qt::UserRole + 7,
        NotifyViewRole = Qt::UserRole + 8
    };

    NotifyModel(QObject *parent = nullptr, EntityPtr notify = nullptr);
    void addNotify(EntityPtr entity);                       //添加一条通知
    void setView(QListView *view) { m_view = view; }
    QListView *view() { return m_view; }
    int rowCount() { return m_displays.size(); }            //返回屏幕上能存放的通知数量
    void removeNotify(EntityPtr entity);
    void setPersistence(Persistence *db) { m_database = db; }   //设置从那个数据库读取数据
    void expandData(EntityPtr entity);
    void collapseData();
    void refreshContent();
    bool isShowOverlap() const;                             //判断通知气泡是否会重叠
    bool isExpand() const;                                  //判断通知气泡是否应该展开
    bool canExpand(EntityPtr entity) const;
    QList<EntityPtr> allNotifys() { return m_displays; }    //返回所有通知
    QList<EntityPtr> overlapNotifys();                      //返回被折叠的通知

signals:
    void appendNotify();
    void expandNotify();
    void deleteNotify(int index);
    void layoutGroup();
    void currentIndexChanged(const QModelIndex &index);     //通知ShortManager

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QListView *m_view = nullptr;
    Persistence *m_database = nullptr;
    QList<EntityPtr> m_notfications;
    QList<EntityPtr> m_displays;
};

Q_DECLARE_METATYPE(std::shared_ptr<NotifyModel>);

#endif // NOTIFYMODEL_H
