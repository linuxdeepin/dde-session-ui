/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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
#include "shortcutmanage.h"
#include "bubbleitem.h"
#include "bubblegroup.h"
#include "appgroupmodel.h"
#include "notifymodel.h"
#include "notification/button.h"
#include "notification/iconbutton.h"
#include "notification/constants.h"
#include "bubbleoverlapwidget.h"

#include <QWidget>
#include <QListView>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

DWIDGET_USE_NAMESPACE

ShortcutManage *ShortcutManage::m_instance = nullptr;

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

void ShortcutManage::setAppModel(AppGroupModel *model)
{
    m_appModel = model;
    initIndex();
}

void ShortcutManage::initIndex()
{
    m_currentGroupIndex = m_appModel->index(0);
    QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
    if (app_view != nullptr) {
        app_view->setCurrentIndex(m_currentGroupIndex);
        app_view->scrollTo(m_currentGroupIndex);
    }

    if (!m_appModel->appGroups().isEmpty()) {
        auto m_currentApp = m_appModel->appGroups().first();
        auto notify_model = m_currentApp->notifyModel().value<std::shared_ptr<NotifyModel>>();

        if (notify_model != nullptr) {
            m_currentIndex = notify_model->index(0);
            QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
            if (group_view != nullptr) {
                group_view->setCurrentIndex(m_currentIndex);
                group_view->scrollTo(m_currentIndex);
                if (QWidget *widget = group_view->indexWidget(m_currentIndex)) {
                    widget->setFocus();
                }
            }
        }
    }
}

void ShortcutManage::onGroupIndexChanged(const QModelIndex &groupIndex)
{
    //group发生修改，重置currentIndex为当前group第一个(后面删除)
    m_currentGroupIndex = groupIndex;

    QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
    if (app_view != nullptr) {
        if (m_currentGroupIndex.isValid()) {
            app_view->setCurrentIndex(m_currentGroupIndex);
            app_view->scrollTo(m_currentGroupIndex);
        } else {
            initIndex();
        }

        auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        if (model != nullptr) {
            m_currentIndex = model->index(0);
            QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
            group_view->setCurrentIndex(m_currentIndex);
            group_view->scrollTo(m_currentIndex);
            if (QWidget *widget = group_view->indexWidget(m_currentIndex)) {
                widget->setFocus();
            }
        }
    }
}

void ShortcutManage::onGroupIndexChanged_(const QModelIndex &groupIndex, const QModelIndex &index)
{
    m_currentGroupIndex = groupIndex;

    QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
    if (app_view != nullptr) {
        if (m_currentGroupIndex.isValid()) {
            app_view->setCurrentIndex(m_currentGroupIndex);
            app_view->scrollTo(m_currentGroupIndex);
        } else {
            initIndex();
        }

        auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        if (model != nullptr) {
            m_currentIndex = index;
            QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
            group_view->setCurrentIndex(m_currentIndex);
            group_view->scrollTo(m_currentIndex);
            if (QWidget *widget = group_view->indexWidget(m_currentIndex)) {
                widget->setFocus();
            }
        }
    }
}

void ShortcutManage::onViewIndexChanged(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    m_currentIndex = index;

    auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    if (model != nullptr) {
        QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
        if (group_view) {
            QTimer::singleShot(AnimationTime + 5, [ = ] {
                group_view->setCurrentIndex(m_currentIndex);
                group_view->scrollTo(m_currentIndex);
                if (QWidget *widget = group_view->indexWidget(m_currentIndex))
                {
                    widget->setFocus();
                }
            });
        }
    }
}

bool ShortcutManage::handBubbleTab(QWidget *item)
{
    BubbleItem *bubble = qobject_cast<BubbleItem *>(item);
    BubbleOverlapWidget *bubble_overlap = qobject_cast<BubbleOverlapWidget *>(item);
    if (bubble_overlap != nullptr) {
        bubble = bubble_overlap->faceBubble();
    }

    if (bubble == nullptr) return true;

    if (!m_currentElement.isNull()) {
        Button *action_btn = qobject_cast<Button *>(m_currentElement.data());
        if (action_btn != nullptr) {
            action_btn->setHoverState(false);
        }
    }

    QList<QPointer<QWidget>> elements = bubble->bubbleElements();
    const int last_pos = elements.indexOf(m_currentElement) + 1;
    if (last_pos < elements.count()) {
        m_currentElement = elements.at(last_pos);
        IconButton *close_btn = qobject_cast<IconButton *>(m_currentElement);
        if (close_btn != nullptr) {
            close_btn->setFocusState(true);
        } else {
            Button *action_btn = qobject_cast<Button *>(m_currentElement);
            if (action_btn != nullptr) {
                action_btn->setHoverState(true);
            }
        }
    } else {
        Button *action_btn = qobject_cast<Button *>(m_currentElement);
        if (action_btn != nullptr) {
            action_btn->setHoverState(false);
        }
        m_currentElement = nullptr;
    }

    return m_currentElement == nullptr;
}

bool ShortcutManage::calcNextBubbleIndex()
{
    QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
    if (group_view != nullptr && handBubbleTab(group_view->indexWidget(m_currentIndex))) {
        int row = m_currentIndex.row();
        m_currentIndex = group_view->model()->index(row + 1, 0);

        if (m_currentIndex.isValid()) {
            group_view->setCurrentIndex(m_currentIndex);
            group_view->scrollTo(m_currentIndex);
            if (QWidget *widget = group_view->indexWidget(m_currentIndex)) {
                widget->setFocus();
            }
        } else {
            calcNextGroupIndex();
        }
        return true;
    }

    return false;
}

bool ShortcutManage::calcNextGroupIndex()
{
    QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
    if (app_view != nullptr) {
        int row = m_currentGroupIndex.row();
        m_currentGroupIndex = app_view->model()->index(row + 1, 0);
        if (m_currentGroupIndex.isValid()) {
            app_view->setCurrentIndex(m_currentGroupIndex);
            app_view->scrollTo(m_currentGroupIndex);
        } else {
            initIndex();
            return true;
        }

        auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        if (model != nullptr) {
            m_currentIndex = model->index(0);
            QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
            group_view->setCurrentIndex(m_currentIndex);
            group_view->scrollTo(m_currentIndex);
            if (QWidget *widget = group_view->indexWidget(m_currentIndex)) {
                widget->setFocus();
            }
        }
        return true;
    }
    return false;
}

//bool ShortcutManage::calcCurrentGroupIndex()
//{
//    QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
//    if (app_view != nullptr) {
//        if (m_currentGroupIndex.isValid()) {
//            app_view->setCurrentIndex(m_currentGroupIndex);
//            app_view->scrollTo(m_currentGroupIndex);
//        } else {
//            initIndex();
//            return true;
//        }

//        auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
//        if (model != nullptr) {
//            m_currentIndex = model->index(0);
//            QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
//            group_view->setCurrentIndex(m_currentIndex);
//            group_view->scrollTo(m_currentIndex);
//            group_view->indexWidget(m_currentIndex)->setFocus();
//        }
//        return true;
//    }
//    return false;
//}

bool ShortcutManage::handKeyEvent(QObject *object, QKeyEvent *event)
{
    Q_UNUSED(object);
    if (event->key() == Qt::Key_Tab) {
        calcNextBubbleIndex();
        return true;
    } else if (event->key() == Qt::Key_Return) {
        if (m_currentElement != nullptr) {
            IconButton *close_btn = qobject_cast<IconButton *>(m_currentElement.data());
            if (close_btn != nullptr) {
                close_btn->clicked();
            } else {
                Button *action_btn = qobject_cast<Button *>(m_currentElement.data());
                if (action_btn != nullptr) {
                    action_btn->clicked();
                }
            }
            m_currentElement = nullptr;
            return true;
        }
    }
    return false;
}

bool ShortcutManage::handPressEvent(QObject *object)
{
    BubbleGroup *group = qobject_cast<BubbleGroup *>(object);
    if (group != nullptr) {
        QListView *app_view = m_appModel->view();
        if (app_view != nullptr) {
            m_currentGroupIndex = app_view->indexAt(group->pos());
        }
    }

    BubbleOverlapWidget *bubbleParent = NULL;
    BubbleItem *bubble = qobject_cast<BubbleItem *>(object);
    if (bubble != NULL)
        bubbleParent = qobject_cast<BubbleOverlapWidget *>(bubble->parent());
    BubbleOverlapWidget *overlap = qobject_cast<BubbleOverlapWidget *>(object);
    if (bubble != nullptr || overlap != nullptr) {
        auto notify_model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        if (notify_model != nullptr) {
            QListView *group_view = notify_model->view();
            if (group_view != nullptr) {
                if (bubble != nullptr && bubbleParent == nullptr)
                    m_currentIndex = group_view->indexAt(bubble->pos());

                if (overlap != nullptr)
                    m_currentIndex = group_view->indexAt(overlap->pos());

            }
        }
    }

    return false;
}

bool ShortcutManage::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        return handKeyEvent(object, key);
    } else if (event->type() == QEvent::Enter) {
        return handPressEvent(object);
    }

    return false;
}
