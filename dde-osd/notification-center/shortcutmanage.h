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

class QKeyEvent;
class QMouseEvent;

class ShortcutItem
{
public:
    ShortcutItem(QWidget *widget = nullptr, const QModelIndex &index = QModelIndex());
    QWidget *m_widget = nullptr;
    QWidget *m_parent = nullptr;
    QModelIndex m_index;
};

class ShortcutGroup
{
public:
    ShortcutGroup(QWidget *widget = nullptr, const QModelIndex &index = QModelIndex());
    QWidget *m_widget = nullptr;
    QModelIndex m_index;
    QList<std::shared_ptr<ShortcutItem>> m_groups;
};

class ShortcutManage : public QObject
{
    Q_OBJECT
public:
    static ShortcutManage *instance(QObject *parent = nullptr);
    void append(std::shared_ptr<ShortcutGroup> group);
    void append(std::shared_ptr<ShortcutItem> item);
    void removeItem(QWidget *widget);
    void removeGroup(QWidget *widget);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    explicit ShortcutManage(QObject *parent = nullptr);
    bool handKeyEvent(QObject *object, QKeyEvent *event);
    bool handMouseEvent(QObject *object, QMouseEvent *event);

private:
    static ShortcutManage *m_instance;
    QList<std::shared_ptr<ShortcutGroup>> m_Shortcuts;
    QMap<QWidget *, std::shared_ptr<ShortcutItem>> m_widgetMap;
    QMap<QWidget *, std::shared_ptr<ShortcutGroup>> m_groupMap;
    std::shared_ptr<ShortcutItem> m_currentItem = nullptr;
};

#endif // SHORTCUTMANAGE_H
