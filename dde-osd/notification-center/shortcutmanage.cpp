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
#include "notifylistview.h"

#include <QWidget>
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

DWIDGET_USE_NAMESPACE

ShortcutManage *ShortcutManage::m_instance = nullptr;

ShortcutManage::ShortcutManage(QObject *parent)
    : QObject(parent)
    , m_displayInter(new DBusDisplay)
    , m_dockDeamonInter(new DBusDock)
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
    connect(this, &ShortcutManage::setScrollBar, m_appModel, &AppGroupModel::setScrollBarValue);
    initIndex();
}

void ShortcutManage::initIndex()
{
    m_appRow = 0;
    NotifyListView *app_view = m_appModel->view();
    if (app_view != nullptr) {
        app_view->setCurrentIndex(m_appRow);
    }

    if (!m_appModel->appGroups().isEmpty()) {
        auto m_currentApp = m_appModel->appGroups().first();
        auto model = m_currentApp->notifyModel().value<std::shared_ptr<NotifyModel>>();

        if (model != nullptr) {
            m_notifyRow = 0;
            NotifyListView *group_view = model->view();
            if (group_view != nullptr) {
                group_view->setCurrentIndex(m_notifyRow);
                if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
                    widget->setFocus();
                }
            }
        }
    }
}

void ShortcutManage::onGroupIndexChanged(int groupRow)
{
    //group发生修改，重置currentIndex为当前group第一个(后面删除)
    m_appRow = groupRow;

    NotifyListView *app_view = m_appModel->view();
    if (app_view != nullptr) {
        if (app_view->model()->index(m_appRow, 0).isValid()) {
            app_view->setCurrentIndex(m_appRow);
        } else {
            initIndex();
        }

        auto model = app_view->model()->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        if (model != nullptr) {
            m_notifyRow = 0;
            NotifyListView *group_view = model->view();
            group_view->setCurrentIndex(m_notifyRow);
            if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
                widget->setFocus();
            }
        }
    }
}

//void ShortcutManage::onGroupIndexChanged_(const QModelIndex &groupIndex, const QModelIndex &index)
//{
//    m_appRow = groupIndex.row();

//    NotifyListView *app_view = m_appModel->view();
//    if (app_view != nullptr) {
//        if (m_appModel->index(m_appRow).isValid()) {
//            app_view->setCurrentIndex(m_appRow);
//        } else {
//            initIndex();
//        }
//        auto model = app_view->model()->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
//        if (model != nullptr) {
//            m_notifyRow = index.row();
//            NotifyListView *group_view = model->view();
//            group_view->setCurrentIndex(m_notifyRow);
//            if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
//                widget->setFocus();
//            }
//        }
//    }
//}

void ShortcutManage::onViewIndexChanged(int notifyRow)
{
    auto model = m_appModel->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    if (model && !model->index(notifyRow).isValid()) {
        return;
    }

    m_notifyRow = notifyRow;

    if (model != nullptr) {
        NotifyListView *group_view = model->view();
        if (group_view) {
            group_view->setCurrentIndex(m_notifyRow);
            if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
                widget->setFocus();
            }
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
    auto model = m_appModel->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    NotifyListView *group_view = model->view();
    if (group_view != nullptr && handBubbleTab(group_view->indexWidget(model->index(m_notifyRow)))) {
        m_notifyRow ++;
        if (model->index(m_notifyRow).isValid()) {
            group_view->setCurrentIndex(m_notifyRow);
            if (calcScrollValue() > 0) {
                Q_EMIT setScrollBar(calcScrollValue());
            }
            if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
                widget->setFocus();
            }
        } else {
            calcNextGroupIndex();
            if (calcScrollValue() > 0) {
                Q_EMIT setScrollBar(calcScrollValue());
            }

        }
        return true;
    }
    return false;
}

bool ShortcutManage::calcNextGroupIndex()
{
    NotifyListView *app_view = m_appModel->view();

    if (app_view != nullptr) {
        m_appRow ++;
        if (m_appModel->index(m_appRow).isValid()) {
            app_view->setCurrentIndex(m_appRow);
        } else {
            initIndex();
            Q_EMIT setScrollBar(0);
            return true;
        }
        auto model = app_view->model()->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();

        if (model != nullptr) {
            m_notifyRow = 0;
            NotifyListView *group_view = model->view();
            group_view->setCurrentIndex(m_notifyRow);
            if (QWidget *widget = group_view->indexWidget(model->index(m_notifyRow))) {
                widget->setFocus();
            }
        }
        return true;
    }
    return false;
}

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
        NotifyListView *app_view = m_appModel->view();
        if (app_view != nullptr) {
            m_appRow = app_view->indexAt(group->pos()).row();
        }
    }

    BubbleOverlapWidget *bubbleParent = NULL;
    BubbleItem *bubble = qobject_cast<BubbleItem *>(object);
    if (bubble)
        bubbleParent = qobject_cast<BubbleOverlapWidget *>(bubble->parent());
    BubbleOverlapWidget *overlap = qobject_cast<BubbleOverlapWidget *>(object);
    if (bubble != nullptr || overlap != nullptr) {

        NotifyListView *app_view = m_appModel->view();
        auto model = app_view->model()->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();

        if (app_view && model) {
            NotifyListView *group_view = model->view();
            if (group_view != nullptr) {
                if (bubble != nullptr && bubbleParent == nullptr)
                    m_notifyRow = group_view->indexAt(bubble->pos()).row();
                if (overlap != nullptr)
                    m_notifyRow = group_view->indexAt(overlap->pos()).row();
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

int ShortcutManage::calcScrollValue()
{
    int currentPos = 0;
    NotifyListView *app_view = m_appModel->view();

    int groupRow = m_appRow; //获取当前气泡组的索引

    for (int row = 0; row < groupRow; row ++) { //计算当前组前面气泡组的高度之和
        QModelIndex groupIndex = app_view->model()->index(row, 0);
        auto notify_model = groupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
        QWidget *widget = app_view->indexWidget(groupIndex);
        currentPos = currentPos + widget->height();
    }

    auto model = app_view->model()->index(m_appRow, 0).data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
    NotifyListView *group_view = model->view();

    QWidget *currentBubbleWidget = group_view->indexWidget(model->index(m_notifyRow));

    currentPos = currentPos + m_notifyRow * 100 + (currentBubbleWidget->height() + 10) + Notify::GroupTitleHeight; //计算当前气泡的位置
    QRect display = m_displayInter->primaryRawRect();
    QRect dock = m_dockDeamonInter->frontendRect();

    int notifiWidgetHeight = display.height() - dock.height() - Notify::CenterTitleHeight - Notify::CenterMargin * 2 - Notify::CenterMargin;

    int scrollValue = currentPos - notifiWidgetHeight + Notify::CenterMargin * 2;
    return scrollValue;
}

