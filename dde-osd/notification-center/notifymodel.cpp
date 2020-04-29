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

#include "notifymodel.h"
#include "../notification/persistence.h"
#include "notifylistview.h"

#include <QDebug>
#include <QDateTime>
#include <QTimer>

NotifyModel::NotifyModel(QObject *parent, Persistence *database)
    : QAbstractListModel(parent),
      m_database(database),
      m_addTimer(new QTimer(this)),
      m_checkTimeOutTimer(new QTimer(this))
{
    m_addTimer->setInterval(AnimationTime + 100);
    m_checkTimeOutTimer->setInterval(TIME_OUT_CHECK_TIME);
    m_checkTimeOutTimer->start();

    initData();
    connect(m_addTimer, &QTimer::timeout, this, [ = ] { //通知顺序添加,防止同时插入多个通知导致动画出问题
        pushNotify(m_newNotifyList.first());
        m_newNotifyList.removeFirst();
        if (m_newNotifyList.isEmpty()) {
            m_addTimer->stop();
            m_timerState = false;
        }
    });

    connect(m_checkTimeOutTimer, &QTimer::timeout, this, &NotifyModel::removeTimeOutNotify);
}

void NotifyModel::setView(NotifyListView *view)
{
    if (view == nullptr)
        return;
    m_view = view;
    connect(m_view, &NotifyListView::addedAniFinished, this, &NotifyModel::addNotify);
    connect(m_view, &NotifyListView::expandAniFinished, this, &NotifyModel::expandData);
}

void NotifyModel::expandData(QString appName)
{
    beginResetModel();
    for (int i = 0; i < m_notifications.size(); i++) {
        if (m_notifications[i].appName == appName) {
            m_notifications[i].isExpand = true;
            foreach(auto ptr,m_notifications[i].appList)
            {
                ptr->setShow(true);
                ptr->setHideCount(0);
            }
            break;
        }
    }
    endResetModel();
}

void NotifyModel::collapseData()
{
    beginResetModel();
    for (int i = 0; i < m_notifications.size(); i++) {
        m_notifications[i].isExpand = false;
    }
    checkShow(m_notifications);
    endResetModel();
}

void NotifyModel::removeAppGroup(QString appName)
{
    beginResetModel();
    if (m_notifications.isEmpty())
        return;
    for (int i = 0; i < m_notifications.size(); i++) {
        if (m_notifications[i].appName == appName) {
            m_notifications.removeAt(i);
        }
    }
    endResetModel();
    m_database->removeApp(appName);
}

void NotifyModel::removeNotify(EntityPtr entity)
{
    beginResetModel();
    if (m_notifications.isEmpty())
        return;
    for (int i = 0; i < m_notifications.size(); i++) {
        if (m_notifications[i].appName == entity->appName() && m_notifications[i].appList.contains(entity)) {
            m_notifications[i].appList.removeOne(entity);
            int count = 0;
            checkShow(m_notifications);
            if (!m_notifications[i].appList.isEmpty()) {
                foreach (auto ptr, m_notifications[i].appList) {
                    if (ptr->isShow())
                        count ++;
                }
            }

            if (m_notifications[i].appList.isEmpty()) { //如果app通知为空，删除app组
                for (int i = 0; i < m_notifications.size(); i++) {
                    if (m_notifications[i].appName == entity->appName()) {
                        m_notifications.removeAt(i);
                    }
                }
                checkShow(m_notifications);
            } else {
                if (!(count < m_notifications[i].appList.size())) { //没有Item折叠设置app组为展开状态
                    m_notifications[i].isExpand = true;
                }
            }
            break;
        }
    }
    endResetModel();
    if (m_database != nullptr) {
        m_database->removeOne(QString::number(entity->id()));
    }
}

void NotifyModel::pushNotify(EntityPtr entity) //数据库中有添加数据时发出信号,该函数被执行
{
    // 通知中心没有通知或者app组不在第一个直接刷新model,不需要动画
    if (m_notifications.isEmpty() || m_notifications[0].appName != entity->appName()) {
        addNotify(entity);
        return;
    }
    ListItem appItem = getAppData(entity->appName());
    //执行动画
    if (m_view->isVisible()) {
        m_view->createAddedAnimation(entity, appItem);
    }
}

bool NotifyModel::isExpand(QString appName)
{
    foreach (auto item, m_notifications) {
        if (item.appName == appName)
            return item.isExpand;
    }
    return false;
}

ListItem NotifyModel::getAppData(QString appName) const
{
    foreach (auto item, m_notifications) {
        if (item.appName == appName)
            return item;
    }
    ListItem item;
    return item;
}

void NotifyModel::removeAllData()
{
    beginResetModel();
    m_notifications.clear();
    endResetModel();
    m_database->removeAll();
}

void NotifyModel::removeTimeOutNotify()
{
    int removeCount = 0;
    for (int i = 0; i < m_notifications.size(); i++) {
        for (int j = 0; j < m_notifications[i].appList.size(); j++) {
            if(checkTimeOut(m_notifications[i].appList[j])) {
                m_notifications[i].appList.removeAt(j);
                checkShow(m_notifications);
                removeCount ++;
            }
            if(m_notifications[i].appList.isEmpty()) {
                m_notifications.removeAt(i);
                checkShow(m_notifications);
                break;
            }
        }
    }
    if (removeCount > 0) {
        resetModel();
    }
}

void NotifyModel::resetModel()
{
    checkShow(m_notifications);
    beginResetModel();
    endResetModel();
}

void NotifyModel::initData()
{
    Q_ASSERT(m_database);
    if (m_database == nullptr)  return;
    QList<EntityPtr> notifications = m_database->getAllNotify();
    foreach (auto notify, notifications) {
        if(!checkTimeOut(notify))
            addNotifyWithoutPaint(notify);
    }
#if 1
    for(int i = 0; i < m_notifications.size(); ++i)
    {
        auto item = m_notifications[i];
        for(int j = 0; j < item.appList.size(); ++j)
        {
            auto ptr = item.appList[j];
        }
    }
#endif
    Q_EMIT dataChanged();
    connect(m_database, &Persistence::RecordAdded, this, [ = ] (EntityPtr entity) {
        if (m_view->isVisible()) {
            m_newNotifyList.push_front(entity); // 暂存通知
            if (!m_timerState) {
                m_addTimer->start();
                m_timerState = true;
            }
        } else {
            addNotify(entity);
        }
    });
}

int NotifyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int count = 0;
    foreach (auto item, m_notifications) {
        foreach (auto ptr, item.appList) {
            if (ptr->isShow())
                count ++;
        }
    }
    count += m_notifications.size();
    return count;
}

QVariant NotifyModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    if (!index.isValid() || index.row() > (rowCount(QModelIndex()) - 1)) {
        return QVariant();
    }

    EntityPtr entity = getEntityByRow(index.row());
    if (entity) {
#if 0
        if(entity->appName().contains("notify-send"))
        {
            qDebug() << QDateTime::fromMSecsSinceEpoch(entity->ctime().toLongLong()).toString("yyyy-MM-dd hh:mm:ss:zzz");
        }
#endif
        return QVariant::fromValue(entity);
    }
    return QVariant();
}

Qt::ItemFlags NotifyModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (m_view != nullptr) m_view->openPersistentEditor(index);
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index);
}

void NotifyModel::addNotify(EntityPtr entity)
{
    beginResetModel();
    bool bInsert = false;
    for (int i = 0 ; i < m_notifications.size(); ++i) {
        ListItem &item = m_notifications[i];
        if (item.appName == entity->appName()) {
            item.appList.push_front(entity);
            item.lastTimeStamp = qMax(item.lastTimeStamp, entity->ctime());
            bInsert = true;
            // 消息排序
            qSort(item.appList.begin(),item.appList.end(),[=](const EntityPtr& ptr1,const EntityPtr& ptr2){
                return ptr1->ctime().toLongLong() > ptr2->ctime().toLongLong();
            });
            break;
        }
    }

    // 新增应用
    if (!bInsert) {
        ListItem item;
        item.appName = entity->appName();
        item.lastTimeStamp = entity->ctime();
        item.appList.push_back(entity);
        item.isExpand = false;

        m_notifications.push_front(item);
    }

    // 对应用进行排序
    qSort(m_notifications.begin(), m_notifications.end(), [ = ](const ListItem & item1, const ListItem & item2) {
        return item1.lastTimeStamp.toLongLong() > item2.lastTimeStamp.toLongLong();
    });
    // 即将添加消息的应用永远在第一个
    for (int i = 0; i < m_notifications.size(); i++) {
        if (m_notifications[i].appName == entity->appName()) {
            m_notifications.move(i, 0);
        }
    }

    //进一步整理层叠的消息
    checkShow(m_notifications);

    endResetModel();
}

void NotifyModel::addNotifyWithoutPaint(EntityPtr entity)
{
    bool bInsert = false;
    for (int i = 0 ; i < m_notifications.size(); ++i) {
        ListItem &item = m_notifications[i];
        if (item.appName == entity->appName()) {
            item.appList.push_front(entity);
            item.lastTimeStamp = qMax(item.lastTimeStamp, entity->ctime());
            bInsert = true;
            // 消息排序
            qSort(item.appList.begin(),item.appList.end(),[=](const EntityPtr& ptr1,const EntityPtr& ptr2){
                return ptr1->ctime().toLongLong() > ptr2->ctime().toLongLong();
            });
            break;
        }
    }

    // 新增应用
    if (!bInsert) {
        ListItem item;
        item.appName = entity->appName();
        item.lastTimeStamp = entity->ctime();
        item.appList.push_back(entity);
        item.isExpand = false;

        m_notifications.push_front(item);
    }

    // 对应用进行排序
    qSort(m_notifications.begin(), m_notifications.end(), [ = ] (const ListItem & item1, const ListItem & item2) {
        return item1.lastTimeStamp.toLongLong() > item2.lastTimeStamp.toLongLong();
    });

    //进一步整理层叠的消息
    checkShow(m_notifications);
}

EntityPtr NotifyModel::getEntityByRow(int row) const
{
    Q_ASSERT(row <= rowCount(QModelIndex()) - 1);
    int index = 0;
    for (int i = 0; i < m_notifications.size(); ++i) {
        const auto &item = m_notifications[i];
        if (index == row) {
            // 标题
            EntityPtr ptr = std::make_shared<NotificationEntity>(item.appName);
            ptr->setTime(item.lastTimeStamp);
            ptr->setIsTitle(true);
            return ptr;
        }
        index++;
        for (int i = 0; i < item.appList.size(); ++i) {
            if (item.appList[i]->isShow()) {
                if (index == row)
                    return item.appList.at(i);
                index ++;
            }
        }
    }
    Q_UNREACHABLE();
}

bool NotifyModel::canShow(EntityPtr ptr)
{
    // 超过4小时的消息，应该折叠
    QDateTime t = QDateTime::fromMSecsSinceEpoch(ptr->ctime().toLongLong());
    return t.secsTo(QDateTime::currentDateTime()) < OVERLAPTIMEOUT;
}

void NotifyModel::checkShow(QList<ListItem> &list)
{
    //TODO 重置一下显示状态，这里后面优化下，不用每次重置，给EntityPtr加一条是否是第一条消息的属性就可以
    foreach (auto item, m_notifications) {
        if (item.isExpand)
            continue;
        foreach (auto ptr, item.appList) {
            ptr->setShow(canShow(ptr));
        }
    }

    for (int i = 0; i < list.size(); ++i) {
        if (list[i].isExpand)
            continue;
        auto &item = list[i];
        int hideCount = 0;
        int size = list[i].appList.size();
        int count = 0; // 展开的数量
        if (item.appList.isEmpty())
            return;
        for (int j = 0 ; j < item.appList.size(); ++j) {
            item.appList[j]->setHideCount(0);
            if (item.appList[j]->isShow())
            {
                count++;
            }
            if (count > BubbleEntities) { //最多展开3个,其余的即使没超过4个小时也需要折叠起来
                item.appList[j]->setShow(false);
                count = BubbleEntities;
            }
        }
        if (count == 0) { // 即都超过4小时的情况，那么相对最新的那一条需要显示，不能都折叠
            item.appList[0]->setShow(true);
            count = 1;
        }
        // 更新应用组信息
        hideCount = size - count;
        item.hideCount = hideCount;
        item.showCount = count;
        item.isExpand = false;
        item.appList[count - 1]->setHideCount(hideCount < BubbleEntities ? hideCount : BubbleEntities - 1);
    }
}

bool NotifyModel::checkTimeOut(EntityPtr ptr)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(ptr->ctime().toLongLong());
    return t.secsTo(QDateTime::currentDateTime()) > NOTIFYTIMEOUT;
}

