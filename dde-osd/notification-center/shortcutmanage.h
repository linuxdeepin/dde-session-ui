/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
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

#ifndef SHORTCUTMANAGE_H
#define SHORTCUTMANAGE_H

#include <QObject>
#include <QModelIndex>
#include <memory>
#include <QWidget>
#include <QPointer>

#include "notification/dbusdisplay.h"
#include "notification/dbusdock.h"
#include "notification/constants.h"

class QKeyEvent;
class QMouseEvent;
class QFocusEvent;
class QEnterEvent;
class AppGroupModel;
class ApplicationGroup;

/*!
 * \~chinese \class ShortcutManage
 * \~chinese \brief 全局按键事件管理类，通知中心里的每一条通知的关闭按钮，菜单，通知本身可以实现按tab键切换选中焦点
 */
class ShortcutManage : public QObject
{
    Q_OBJECT
public:
    static ShortcutManage *instance(QObject *parent = nullptr);
    void setAppModel(AppGroupModel *model);
    void initIndex();

public Q_SLOTS:
    void onGroupIndexChanged(const QModelIndex &groupIndex); //当前App组中Index发生改变时执行该函数
    void onGroupIndexChanged_(const QModelIndex &groupIndex, const QModelIndex &index);
    void onViewIndexChanged(const QModelIndex &index);//当前视图中Index发生改变时执行该函数

protected:
    bool eventFilter(QObject *object, QEvent *event) override; //过滤事件

private:
    explicit ShortcutManage(QObject *parent = nullptr);
    bool handKeyEvent(QObject *object, QKeyEvent *event);   //处理键盘TAB键按下的事件
    bool handPressEvent(QObject *object);                   //处理键盘Enter键按下的事件
    bool handBubbleTab(QWidget *item);                      //按下键盘TAB键时计算气泡中index的位置是否到最后一个
    bool calcNextBubbleIndex();                             //计算下一个气泡的位置
    bool calcNextGroupIndex();                              //计算下一个气泡组的位置
    int calcScrollValue();

private:
    static ShortcutManage *m_instance;
    AppGroupModel *m_appModel = nullptr;
    QModelIndex m_currentGroupIndex;
    QModelIndex m_currentIndex;
    QPointer<QWidget> m_currentElement = nullptr;

    DBusDisplay *m_displayInter;
    DBusDock *m_dockDeamonInter;

Q_SIGNALS:
    void refreshTimer();
    void setScrollBar(int value);
};

#endif // SHORTCUTMANAGE_H
