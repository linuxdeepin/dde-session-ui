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
class NotifyModel;

class BubbleItem : public BubbleAbStract
{
    Q_OBJECT
public:
    BubbleItem(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr);
    void setModel(NotifyModel *model);
    void clearContent();

Q_SIGNALS:
    void havorStateChanged(bool);

public Q_SLOTS:
    void onHavorStateChanged(bool hover);
    void onCloseBubble();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initContent();
    void onRefreshTime();
    void refreshTheme();

private:
    NotifyModel *m_notifyModel = nullptr;
    QTimer *m_refreshTimer = nullptr;
    QPropertyAnimation *m_closeAnimation = nullptr;
};

#endif // BUBBLEITEM_H
