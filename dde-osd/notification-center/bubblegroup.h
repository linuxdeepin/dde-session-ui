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
class AppGroupModel;
class BubbleDelegate;
class NotificationEntity;

DWIDGET_USE_NAMESPACE
/*!
 * \~chinese \class BubbleGroup
 * \~chinese \brief 继承自QWidget,通知中心窗口中的通知组窗口类
 */
class BubbleGroup : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleGroup(QWidget *parent = nullptr, std::shared_ptr<NotifyModel> model = nullptr);
    ~BubbleGroup() override;
    void setGroupTitle(const QString &title) { group_title->setText(title); } //设置通知组组名
    void setParentModel(AppGroupModel *model);
    QListView *view() { return m_groupList; }

signals:
    void closeGroup();

protected:
    int animationNeedBubble(int index = 0);     //产生动画需要的气泡个数
    void expandAnimation();
    void removeAnimation(int index);            //移除通知动画
    void appendAnimation();                     //添加通知的动画

protected:
    void enterEvent(QEvent *event) override;        //鼠标移动到窗口上,显示气泡组的关闭按钮
    void leaveEvent(QEvent *event) override;        //鼠标移出窗口,气泡组的关闭按钮不显示
    void focusInEvent(QFocusEvent *event) override; //当焦点移动到气泡组时显示气泡组关闭按钮
    void hideEvent(QHideEvent *event) override;     //窗口隐藏事件,窗口隐藏时判断气泡组是否展开,如果有展开折叠气泡组
    bool eventFilter(QObject *obj,QEvent *event) override;
    void refreshTheme();                            //根据系统主题改变前通知组的主题

private:
    QList<QPointer<BubbleItem>> m_bubbleList;
    QWidget *m_titleWidget = nullptr;
    QListView *m_groupList = nullptr;
    BubbleDelegate *m_notifyDelegate = nullptr;
    DIconButton *title_close = nullptr;
    DLabel *group_title = nullptr;
    std::shared_ptr<NotifyModel> m_notifyModel = nullptr;
    AppGroupModel *m_appModel = nullptr;

    QPointer<ExpandAnimation> m_expandAnimation = nullptr;
};

#endif // BUBBLEGROUP_H
