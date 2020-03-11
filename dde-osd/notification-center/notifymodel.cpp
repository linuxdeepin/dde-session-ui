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
#include "notification/persistence.h"
#include "notification/constants.h"
#include "notification-center/shortcutmanage.h"

#include <QDebug>

NotifyModel::NotifyModel(QObject *parent, EntityPtr notify)
    : QAbstractListModel(parent)
{
    addNotify(notify);
    connect(this, &NotifyModel::currentIndexChanged, ShortcutManage::instance(), &ShortcutManage::onViewIndexChanged);
    connect(this, &NotifyModel::expandOver, this, [ = ](int index) {
        Q_EMIT currentIndexChanged(index);
    });
    connect(ShortcutManage::instance(), &ShortcutManage::refreshTimer, this, &NotifyModel::removeOverdueNotify);
}

int NotifyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_displays.size();
}

QVariant NotifyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || m_displays.isEmpty() || index.row() >= m_displays.size()) {
        return QVariant();
    }

    EntityPtr notify = m_displays.at(index.row());

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

    case NotifyViewRole:
        return QVariant::fromValue(m_view);

    default:
        break;
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
    if (m_displays.size() < BubbleEntities || isExpand()) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_displays.push_front(entity);
        endInsertRows();
    } else {
        beginResetModel();
        m_displays.push_front(entity);
        m_notfications.push_front(m_displays.last());
        m_displays.removeLast();
        endResetModel();
    }

    appendNotify();
    layoutGroup();
}

void NotifyModel::removeNotify(EntityPtr entity)
{
    int index = m_displays.indexOf(entity);
    deleteNotify(index);

    bool update = false;

    if (m_displays.contains(entity)) {
        beginRemoveRows(QModelIndex(), index, index);
        m_displays.removeOne(entity);
        endRemoveRows();
        if (index <= m_displays.size() - 1) {
            Q_EMIT currentIndexChanged(index);
            update = true;
        } else {
            if (m_notfications.isEmpty()) { //意味着这条消息删除后就没有新的消息再插入进来了，索引需要-1
                Q_EMIT currentIndexChanged(index - 1);
                update = true;
            }
        }
    }
    m_notfications.removeOne(entity);//？什么作用
    if (!m_notfications.isEmpty()) {
        int index = m_displays.size();
        beginInsertRows(QModelIndex(), index, index);
        m_displays.push_back(m_notfications.takeFirst());
        endInsertRows();
        if (!update) {
            Q_EMIT currentIndexChanged(index);
        }
    }

    // need remove database
    if (m_database != nullptr) {
        m_database->removeOne(QString::number(entity->id()));
    }

    layoutGroup();
}

void NotifyModel::expandData(EntityPtr entity)
{
    if (m_notfications.isEmpty()) return;

    int index = m_displays.indexOf(entity);
    if (index == BubbleEntities - 1) {
        if (!m_notfications.isEmpty()) {
            m_displays.append(m_notfications);
            m_notfications.clear();
        }
        layoutGroup();
        expandNotify(index);
    }
}

void NotifyModel::collapseData()
{
    if (m_notfications.isEmpty()) {
        if (m_displays.size() > BubbleEntities) {
            auto displays = m_displays.mid(0, BubbleEntities);
            m_notfications.append(m_displays.mid(BubbleEntities));
            m_displays.clear();

            m_displays.append(displays);
        }

        refreshContent();
        layoutGroup();
    }
}

void NotifyModel::refreshContent()
{
    beginResetModel();
    endResetModel();
}

bool NotifyModel::isShowOverlap() const
{
    int notify_count = m_displays.size() + m_notfications.size();
    if (notify_count > BubbleEntities && !m_notfications.isEmpty()) {
        return true;
    }

    return false;
}

bool NotifyModel::isExpand() const
{
    int notify_count = m_displays.size() + m_notfications.size();
    if (notify_count > BubbleEntities && m_notfications.isEmpty()) {
        return true;
    }

    return false;
}

bool NotifyModel::canExpand(EntityPtr entity) const
{
    if (m_displays.indexOf(entity) == BubbleEntities - 1 && m_notfications.size() != 0) {
        return true;
    }
    return false;
}

QList<EntityPtr> NotifyModel::overlapNotifys()
{
    QList<EntityPtr> notifys;
    if (isShowOverlap()) {
        notifys.push_front(m_displays.last());

        int index = 0;
        while (index < BubbleEntities - 1) {
            if (index < m_notfications.size())
                notifys.push_back(m_notfications.at(index));

            index ++;
        }
    }
    return notifys;
}

void NotifyModel::removeOverdueNotify()
{
    int overdueNotify = 0;
    for (int index = 0; index < m_displays.size(); index ++) {
        auto notify = m_displays.at(index);
        QDateTime bubbleDateTime = QDateTime::fromMSecsSinceEpoch(notify->ctime().toLongLong());
        QDateTime currentDateTime = QDateTime::currentDateTime();
        int elapsedDay = int(bubbleDateTime.daysTo(currentDateTime));
        if (elapsedDay > 7) {
            overdueNotify++;
            m_displays.removeOne(notify);
        }
    }
    if (overdueNotify) {
        layoutGroup();
    }
}
