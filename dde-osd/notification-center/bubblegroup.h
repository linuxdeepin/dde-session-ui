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

#ifndef BUBBLEGROUP_H
#define BUBBLEGROUP_H

#include <QPointer>
#include <QWidget>
#include <memory>
#include <DLabel>
#include "expandanimation.h"

DWIDGET_BEGIN_NAMESPACE
class DIconButton;
DWIDGET_END_NAMESPACE

class QListView;
class BubbleItem;
class NotifyModel;
class BubbleDelegate;
class NotificationEntity;

DWIDGET_USE_NAMESPACE

class BubbleGroup : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleGroup(QWidget *parent = nullptr, std::shared_ptr<NotifyModel> model = nullptr);
    ~BubbleGroup() override;
    void setGroupTitle(const QString &title) { group_title->setText(title); }
    QListView *view() { return m_groupList; }

signals:
    void closeGroup();

protected:
    int animationNeedBubble(int index = 0);
    void expandAnimation();
    void removeAnimation(int index);
    void appendAnimation();

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void refreshTheme();

private:
    QList<QPointer<BubbleItem>> m_bubbleList;
    QWidget *m_titleWidget = nullptr;
    QListView *m_groupList = nullptr;
    BubbleDelegate *m_notifyDelegate = nullptr;
    DIconButton *title_close = nullptr;
    DLabel *group_title = nullptr;
    std::shared_ptr<NotifyModel> m_notifyModel = nullptr;
    QPointer<ExpandAnimation> m_expandAnimation = nullptr;
};

#endif // BUBBLEGROUP_H
