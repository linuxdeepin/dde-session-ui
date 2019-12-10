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
    m_currentGroupIndex = m_appModel->index(0);
    auto m_currentApp = m_appModel->appGroups().first();
    auto notify_model = m_currentApp->notifyModel().value<std::shared_ptr<NotifyModel>>();

    if (notify_model != nullptr) {
        m_currentIndex = notify_model->index(0);
    }
}

bool ShortcutManage::handKeyEvent(QObject *object, QKeyEvent *event)
{
    Q_UNUSED(object);
    if (event->key() == Qt::Key_Tab) {
        QListView *group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
        if (group_view != nullptr) {
            int row = m_currentIndex.row();
            m_currentIndex = group_view->model()->index(row + 1, 0);
            if (m_currentIndex.isValid()) {
                group_view->setCurrentIndex(m_currentIndex);
                group_view->scrollTo(m_currentIndex);
            } else {
                QListView *app_view = m_currentGroupIndex.data(AppGroupModel::GroupViewRole).value<QListView *>();
                if (app_view != nullptr) {
                    int row = m_currentGroupIndex.row();
                    m_currentGroupIndex = app_view->model()->index(row + 1, 0);
                    if (m_currentGroupIndex.isValid()) {
                        app_view->setCurrentIndex(m_currentGroupIndex);
                        app_view->scrollTo(m_currentGroupIndex);
                    }

                    auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
                    if (model != nullptr) {
                        m_currentIndex = model->index(0);
                        group_view = m_currentIndex.data(NotifyModel::NotifyViewRole).value<QListView *>();
                        group_view->setCurrentIndex(m_currentIndex);
                        group_view->scrollTo(m_currentIndex);
                    }

                }
            }
        }
    }

    return true;
}

bool ShortcutManage::handMouseEvent(QObject *object, QMouseEvent *event)
{
    Q_UNUSED(object);
    QListView *app_view = m_appModel->view();
    if (app_view != nullptr) {
        m_currentGroupIndex =  app_view->indexAt(event->pos());
        qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << m_currentGroupIndex;
        if (m_currentGroupIndex.isValid()) {
            auto model = m_currentGroupIndex.data(AppGroupModel::NotifyModelRole).value<std::shared_ptr<NotifyModel>>();
            if (model != nullptr) {
                QListView *group_view = model->view();
                m_currentIndex = group_view->indexAt(event->pos());
                qDebug() << "######################" << m_currentIndex;
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
