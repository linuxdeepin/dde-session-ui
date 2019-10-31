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

#include "notification/bubbleabstract.h"

class NotificationEntity;
class QPropertyAnimation;

class BubbleItem : public BubbleAbStract
{
    Q_OBJECT
public:
    BubbleItem(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);

Q_SIGNALS:
    void closeBubble();
    void clicked();
    void havorStateChanged(bool);

public Q_SLOTS:
    void onHavorStateChanged(bool hover);
    void moveAnimation(const QPoint &point);

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void onRefreshTime();
    void initUI();

private:
    QTimer *m_refreshTimer = nullptr;
    bool m_canClose = false;
    QPropertyAnimation *m_moveAnimation = nullptr;
};

#endif // BUBBLEITEM_H
