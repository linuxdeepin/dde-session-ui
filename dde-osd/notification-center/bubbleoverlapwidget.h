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

#ifndef BUBBLEOVERLAPWIDGET_H
#define BUBBLEOVERLAPWIDGET_H

#include "bubbleitem.h"
#include <QPointer>
#include <QFrame>
#include <memory>

class NotificationEntity;
class BubbleItem;
class NotifyModel;

class BubbleOverlapWidget : public QFrame
{
    Q_OBJECT
public:
    BubbleOverlapWidget(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr,
                        NotifyModel *model = nullptr);
    void setModel(NotifyModel *model);

private:
    void initOverlap();

private:
    std::shared_ptr<NotificationEntity> m_notify;
    NotifyModel *m_notifyModel = nullptr;
    QPointer<BubbleItem> m_topBubble = nullptr;
    QPointer<BubbleItem> m_firstOverlap = nullptr;
    QPointer<BubbleItem> m_secondOverlap = nullptr;
};

#endif // NOTIFYWIDGET_H
