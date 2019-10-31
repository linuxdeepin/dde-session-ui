/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
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

#ifndef BUBBLEITEM_H
#define BUBBLEITEM_H

#include "notification/bubble.h"

class NotificationEntity;
class BubbleItem : public Bubble
{
    Q_OBJECT
public:
    BubbleItem(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);

signals:
    void closeBubble();

private:
    void onRefreshTime();

private:
    QTimer *m_refreshTimer = nullptr;
};

#endif // BUBBLEITEM_H
