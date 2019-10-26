/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <190771752ri@gmail.com>
 *
 * Maintainer: listenerri <190771752ri@gmail.com>
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

#include "notifyview.h"
#include "notification/persistence.h"
#include "notification/notificationentity.h"
#include "bubblegroup.h"

#include <QScroller>
#include <QFrame>

NotifyView::NotifyView(QWidget *parent, Persistence* database)
    : QWidget(parent),
      m_database(database),
      m_content(new QWidget),
      mainLayout(new QVBoxLayout)
{
    Q_ASSERT(database);
    initData();

    setContentsMargins(0, 0, 0, 0);
    setUpdatesEnabled(true);

    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    QScroller *scroller = QScroller::scroller(this);
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    scroller->setScrollerProperties(sp);

    connect(m_database, &Persistence::RecordAdded, this, &NotifyView::addNotify);

    setLayout(mainLayout);
}

void NotifyView::initData()
{
    if(m_database == nullptr)  return;
    QList<std::shared_ptr<NotificationEntity>> notifications = m_database->getAllNotify();

    foreach (auto notify, notifications) {
        pushGroup(notify);
    }
}

void NotifyView::addNotify(std::shared_ptr<NotificationEntity> entity)
{
    pushGroup(entity);
}

void NotifyView::pushGroup(std::shared_ptr<NotificationEntity> entity)
{
    if(m_applications.contains(entity->appName())) {
        m_applications[entity->appName()]->pushBubble(entity);
    } else {
        BubbleGroup* bubble_group = new BubbleGroup(this, entity);
        bubble_group->pushBubble(entity);
        m_applications.insert(entity->appName(), bubble_group);
        mainLayout->addWidget(bubble_group, Qt::AlignTop);
    }
}
