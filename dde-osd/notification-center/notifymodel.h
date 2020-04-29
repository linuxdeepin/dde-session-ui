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

#ifndef NotifyModel_H
#define NotifyModel_H

#include "../notification/constants.h"

#include <memory>

#include <QAbstractListModel>
#include <QPointer>
#include <QListView>

#define OVERLAPTIMEOUT (4 * 60 * 60)
#define NOTIFYTIMEOUT (7 * 24 * 60 * 60)
#define TIME_OUT_CHECK_TIME 1000

class Persistence;
class NotifyListView;
class QTimer;

typedef struct{
    QString lastTimeStamp;// 此组应用最新的时间组
    QString appName;
    QList<EntityPtr> appList;
    bool isExpand = false;
    int hideCount = 0;
    int showCount = 0;
} ListItem;

class NotifyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    NotifyModel(QObject *parent = nullptr, Persistence *database = nullptr);
    void setView(NotifyListView *view);
    NotifyListView *view() { return m_view; }
    void expandData(QString appName);                   //展开通知
    void collapseData();                                //折叠通知
    void removeAppGroup(QString appName);
    void removeNotify(EntityPtr entity);
    void pushNotify(EntityPtr entity);
    bool isExpand(QString appName);
    ListItem getAppData(QString appName) const;
    void removeAllData();
    void removeTimeOutNotify();
    void resetModel();

public:
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void dataChanged();                                 //数据库有添加数据时发送该信号

private:
    void initData();                                    //初始化数据
    void addNotify(EntityPtr entity);                   //添加通知
    void addNotifyWithoutPaint(EntityPtr entity);
    EntityPtr getEntityByRow(int row) const;
    bool canShow(EntityPtr ptr);                        // 判断消息是否应该层叠[即超过4小时]
    void checkShow(QList<ListItem>& list);              //进一步整理层叠的消息
    bool checkTimeOut(EntityPtr ptr);

private:
    NotifyListView *m_view = nullptr;
    Persistence *m_database = nullptr;
    QList<ListItem> m_notifications;                    //外层为app,内层为此app的消息
    QList<EntityPtr> m_newNotifyList;
    QTimer *m_addTimer;
    QTimer *m_checkTimeOutTimer;
    bool m_timerState = false;
};

#endif // NotifyModel_H
