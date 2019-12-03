#include "shortcutmanage.h"
#include "bubbleitem.h"
#include "bubblegroup.h"
#include "appgroupmodel.h"
#include "notifymodel.h"

#include <QWidget>
#include <QListView>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>

ShortcutManage *ShortcutManage::m_instance = nullptr;

ShortcutItem::ShortcutItem(QWidget *widget, const QModelIndex &index)
    : m_widget(widget)
    , m_index(index)
{

}

ShortcutGroup::ShortcutGroup(QWidget *widget, const QModelIndex &index)
    : m_widget(widget)
    , m_index(index)
{

}

ShortcutManage::ShortcutManage(QObject *parent)
    : QObject(parent)
{
    qApp->installEventFilter(this);
}

ShortcutManage *ShortcutManage::instance(QObject *parent)
{
    if (!m_instance)
        m_instance = new ShortcutManage(parent);

    return m_instance;
}

void ShortcutManage::append(std::shared_ptr<ShortcutGroup> group)
{
    m_Shortcuts.append(group);
}

void ShortcutManage::append(std::shared_ptr<ShortcutItem> item)
{

    QListView *view = item->m_index.data(NotifyModel::NotifyViewRole).value<QListView *>();
    if (view != nullptr) {
        int index = 0;
        foreach (auto it, m_Shortcuts) {
            auto it_group = dynamic_cast<BubbleGroup *>(it->m_widget);
            if (it != nullptr && it_group->view() == view) {
                if (index == 0 && it->m_groups.isEmpty()) {
                    QListView *group_view = it->m_index.data(AppGroupModel::GroupViewRole).value<QListView *>();
                    group_view->setCurrentIndex(it->m_index);
                    m_currentItem = item;
                }

                item->m_parent = it->m_widget;
                it->m_groups.append(item);
                m_widgetMap.insert(item->m_widget, item);
                m_groupMap.insert(item->m_widget, it);

                index++;
                break;
            }
        }
    }
}

void ShortcutManage::removeItem(QWidget *widget)
{
    if (widget == nullptr) return;

    auto it_group = m_groupMap.value(widget);
    if (it_group != nullptr) {
        foreach (auto item, it_group->m_groups) {
            if (item != nullptr && item->m_widget == widget) {
                it_group->m_groups.removeOne(item);
                break;
            }
        }
    }
}

void ShortcutManage::removeGroup(QWidget *widget)
{
    if (widget == nullptr) return;

    int index = 0;
    foreach (auto item, m_Shortcuts) {
        if (item->m_widget == widget) {
            m_Shortcuts.removeAt(index);
        }
        index ++;
    }
}

bool ShortcutManage::handKeyEvent(QObject *object, QKeyEvent *event)
{
    Q_UNUSED(object);

    if (event->key() == Qt::Key_Tab) {
        auto current_group = m_groupMap.value(m_currentItem->m_widget);
        if (current_group != nullptr) {
            if (current_group->m_groups.contains(m_currentItem)) {
                int index = current_group->m_groups.indexOf(m_currentItem);
                index++;

                if (index < current_group->m_groups.size()) {
                    m_currentItem = current_group->m_groups.at(index % current_group->m_groups.size());
                    if (m_currentItem != nullptr) {
                        QListView *notify_view = m_currentItem->m_index.data(NotifyModel::NotifyViewRole).value<QListView *>();
                        if (notify_view != nullptr) {
                            notify_view->setCurrentIndex(m_currentItem->m_index);
                            notify_view->scrollTo(m_currentItem->m_index);
                        }
                    }
                } else {
                    int group_index = m_Shortcuts.indexOf(current_group);
                    group_index++;

                    if (group_index < m_Shortcuts.size()) {
                        current_group = m_Shortcuts.at(group_index % m_Shortcuts.size());
                        QListView *group_view = current_group->m_index.data(AppGroupModel::GroupViewRole).value<QListView *>();
                        if (group_view != nullptr) {
                            group_view->setCurrentIndex(current_group->m_index);
                            group_view->scrollTo(current_group->m_index);

                            if (!current_group->m_groups.isEmpty()) {
                                m_currentItem = current_group->m_groups.first();
                                m_currentItem->m_widget->setFocus();
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool ShortcutManage::handMouseEvent(QObject *object, QMouseEvent *event)
{
    Q_UNUSED(event);
    BubbleItem *bubble = qobject_cast<BubbleItem *>(object);
    if (bubble) {
        auto item = m_widgetMap.value(bubble);
        if (item != nullptr) {
            m_currentItem = item;
            auto index = m_currentItem->m_index;
            QListView *view = index.data(NotifyModel::NotifyViewRole).value<QListView *>();
            if (view != nullptr && index.isValid()) {
                view->setCurrentIndex(index);
            }
        }
    }
    return false;
}

bool ShortcutManage::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object);
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        return handKeyEvent(object, key);
    } else if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouse = static_cast<QMouseEvent *>(event);
        return handMouseEvent(object, mouse);
    }

    return false;
}
