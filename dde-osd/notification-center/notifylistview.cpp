/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "notifylistview.h"
#include "notification/bubbletool.h"
#include "bubbleitem.h"
#include "overlapwidet.h"
#include "bubbletitlewidget.h"
#include "notification/iconbutton.h"
#include "notification/button.h"

#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include <DIconButton>
#include <QDateTime>
#include <QScrollBar>
#include <QScroller>
#include <QTimer>

#define RefreshTime 900

NotifyListView::NotifyListView(QWidget *parent)
    : QListView(parent)
    , m_scrollAni(new QPropertyAnimation(verticalScrollBar(), "value"))
    , m_refreshTimer(new QTimer(this))
{
    qApp->installEventFilter(this);
    m_scrollAni->setEasingCurve(QEasingCurve::OutQuint);
    m_scrollAni->setDuration(800);

    m_refreshTimer->setInterval(RefreshTime);
    m_refreshTimer->start();

    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(m_refreshTimer, &QTimer::timeout, this, &NotifyListView::refreshItemTime);
    connect(m_scrollAni, &QPropertyAnimation::valueChanged, this, &NotifyListView::handleScrollValueChanged);
    connect(m_scrollAni, &QPropertyAnimation::finished, this, &NotifyListView::handleScrollFinished);

    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
    QScroller *scroller = QScroller::scroller(this);
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    scroller->setScrollerProperties(sp);
}

void NotifyListView::createAddedAnimation(EntityPtr entity, const ListItem appItem)
{
    const QModelIndex index = this->model()->index(1, 0);
    QWidget *widget = this->indexWidget(index);
    QPoint benchMarkPos = widget->pos(); //动画基准位置

    QParallelAnimationGroup *addedAniGroup = new QParallelAnimationGroup(this);
    connect(addedAniGroup, &QParallelAnimationGroup::finished, this, [ = ] { //动画完成发出动画完成的信号
        m_aniState = false;
        Q_EMIT addedAniFinished(entity);
    });

    //新通知插入的动画
    BubbleItem *newItem = new BubbleItem(this, entity);
    newItem->show();
    QRect startRect = QRect(benchMarkPos, QSize(widget->width(), 0));
    QRect endRect = QRect(benchMarkPos, QSize(widget->width(), widget->height()));
    QPropertyAnimation *addAni = new QPropertyAnimation(newItem, "geometry", this);
    connect(addAni, &QPropertyAnimation::finished, newItem, [ = ] {
        newItem->deleteLater();
    });
    addAni->setStartValue(startRect);
    addAni->setEndValue(endRect);
    addAni->setDuration(AnimationTime);
    addedAniGroup->addAnimation(addAni);

    //已存在的通知向下移动的动画
    if (appItem.isExpand || (appItem.showCount != appItem.appList.size() && appItem.showCount < BubbleEntities && canShow(appItem.appList[0]))) {
        for (int i = 1; i < this->model()->rowCount(QModelIndex()); ++i) {
            const QModelIndex index = this->model()->index(i, 0);
            Q_ASSERT(index.isValid());
            QWidget *widget = this->indexWidget(index);
            if (!widget) {
                qDebug() << "index widget not created,shoule be returned;";
                break;
            }
            QPropertyAnimation *downMoveAni = new QPropertyAnimation(widget, "pos", this);
            downMoveAni->setStartValue(widget->pos());
            downMoveAni->setEndValue(widget->pos() + QPoint(0, OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing));
            downMoveAni->setDuration(AnimationTime);
            addedAniGroup->addAnimation(downMoveAni);
        }
    } else {
        for (int i = 0; i < appItem.showCount; ++i) {
            const QModelIndex index = this->model()->index(i + 1, 0);
            Q_ASSERT(index.isValid());
            QWidget *widget = this->indexWidget(index);
            if (!widget) {
                qDebug() << "index widget not created,shoule be returned;";
                break;
            }
            if (i + 1 == appItem.showCount) { //即将折叠的位置
                BubbleItem *faceWidget = nullptr;
                if (appItem.appList[i]->hideCount() > 0) {
                    OverLapWidet *overLapWidget = qobject_cast<OverLapWidet *> (widget);
                    faceWidget = overLapWidget->getFaceItem();
                } else {
                    faceWidget = qobject_cast<BubbleItem *> (widget);
                }
                if (faceWidget == nullptr) {
                    qDebug()<<"overLapWidget is null";
                }

                QRect startRect1 = QRect(widget->pos(), widget->size());
                QRect endRect1 = QRect(startRect1.x(), startRect1.y() + OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET), startRect.width(), 0);
                QPropertyAnimation *rightMoveAni = new QPropertyAnimation(faceWidget, "geometry", this);
                rightMoveAni->setStartValue(startRect1);
                rightMoveAni->setEndValue(endRect1);
                rightMoveAni->setDuration(AnimationTime);
                addedAniGroup->addAnimation(rightMoveAni);
            } else {
                QPropertyAnimation *downMoveAni = new QPropertyAnimation(widget, "pos", this);
                downMoveAni->setStartValue(widget->pos());
                downMoveAni->setEndValue(widget->pos() + QPoint(0, OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing));
                downMoveAni->setDuration(AnimationTime);
                addedAniGroup->addAnimation(downMoveAni);
            }
        }
    }
    addedAniGroup->start(QPropertyAnimation::DeleteWhenStopped);
    m_aniState = true;
}

void NotifyListView::createRemoveAnimation(int idx)
{
    QParallelAnimationGroup *removeAniGroup = new QParallelAnimationGroup(this);
    connect(removeAniGroup, &QParallelAnimationGroup::finished, this, [ = ] {
        m_aniState = false;
    });
    for (int i = idx + 1; i < this->model()->rowCount(QModelIndex()); ++i) {
        const QModelIndex index = this->model()->index(i, 0);
        Q_ASSERT(index.isValid());
        QWidget *widget = this->indexWidget(index);
        if (!widget) {
            qDebug() << "index widget not created,shoule be returned;";
            break;
        }
        QPropertyAnimation *topMoveAni = new QPropertyAnimation(widget, "pos", this);
        topMoveAni->setStartValue(widget->pos());
        topMoveAni->setEndValue(widget->pos() + QPoint(0, -OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) - BubbleSpacing));
        topMoveAni->setDuration(AnimationTime);
        removeAniGroup->addAnimation(topMoveAni);
    }

    removeAniGroup->start(QPropertyAnimation::DeleteWhenStopped);
    m_aniState = true;
}

void NotifyListView::createExpandAnimation(int idx, const ListItem appItem)
{
    if (m_aniState)
        return;
    const QModelIndex index = this->model()->index(idx, 0);
    QWidget *widget = this->indexWidget(index);
    QPoint benchMarkPos = widget->pos(); //动画基准位置
    int maxCount = (height() - benchMarkPos.y()) / (OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing);

    QSequentialAnimationGroup *insertAniGroup = new QSequentialAnimationGroup(this);// 逐个插入的串行动画
    QParallelAnimationGroup *downMoveAniGroup = new QParallelAnimationGroup(this);// 整体向下移动的动画
    // 所有动画结束后发出expandAniFinished信号;
    connect(downMoveAniGroup, &QParallelAnimationGroup::finished, this, [ = ] {
        m_aniState = false;
        Q_EMIT expandAniFinished(appItem.appName);
    });
    int needCount = appItem.hideCount > maxCount ? maxCount : appItem.hideCount;
    // item逐个插入的动画
    for (int i = 0; i < appItem.appList.size(); i++) {
        if (appItem.appList[i]->hideCount() > 0) {
            for (int j = 0; j < needCount; j++) {
                BubbleItem *item = new BubbleItem(this, appItem.appList[i + j + 1]);
                QPoint startPos = benchMarkPos + QPoint(0, (OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing) * j);
                QPropertyAnimation *ani = new QPropertyAnimation(item, "pos", this);
                connect(ani, &QPropertyAnimation::stateChanged, item, [ = ] (QAbstractAnimation::State newState, QAbstractAnimation::State oldState) {
                    if (newState == QAbstractAnimation::Running && oldState == QAbstractAnimation::Stopped) {
                        item->move(startPos);
                        item->show();
                    }
                });
                connect(downMoveAniGroup, &QParallelAnimationGroup::finished, item, &BubbleItem::deleteLater);
                ani->setStartValue(startPos);
                ani->setEndValue(startPos + QPoint(0, OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing));
                ani->setDuration(ExpandAnimationTime);
                insertAniGroup->addAnimation(ani);
            }
        }
    }
    QPoint offsetPos = QPoint(0, (OSD::BubbleHeight(OSD::ShowStyle::BUBBLEWIDGET) + BubbleSpacing) * needCount -
                              (appItem.hideCount > BubbleEntities - 1 ? 1 : appItem.hideCount) * BubbleOverLapHeight);
    // 向下移动的动画
    for (int i = idx; i < this->model()->rowCount(QModelIndex()); ++i) {
        const QModelIndex index = this->model()->index(i + 1, 0);
        if (!index.isValid()) { // 如果是最后一组需要一个空的动画,不然可能会导致没有动画结束的信号发出
            QWidget *widget = new QWidget(this);
            QPropertyAnimation *ani = new QPropertyAnimation(widget, "pos", this);
            ani->setStartValue(widget->pos());
            ani->setEndValue(widget->pos() + offsetPos);
            connect(downMoveAniGroup, &QParallelAnimationGroup::finished, widget, &QWidget::deleteLater);
            ani->setDuration(ExpandAnimationTime * needCount);
            downMoveAniGroup->addAnimation(ani);
            break;
        }
        QWidget *widget = this->indexWidget(index);
        if (!widget) {
            break;
        }
        QPropertyAnimation *ani = new QPropertyAnimation(widget, "pos", this);
        ani->setStartValue(widget->pos());
        ani->setEndValue(widget->pos() + offsetPos);
        ani->setDuration(ExpandAnimationTime * needCount);
        downMoveAniGroup->addAnimation(ani);
    }
    insertAniGroup->start(QPropertyAnimation::DeleteWhenStopped);
    downMoveAniGroup->start(QPropertyAnimation::DeleteWhenStopped);
    m_aniState = true;
}

void NotifyListView::setAniState(bool state)
{
    m_aniState = state;
}

void NotifyListView::setCurrentRow(int row)
{
    m_currentIndex = row;
}

void NotifyListView::mousePressEvent(QMouseEvent *event)
{
    if (m_aniState)
        return;
    return QListView::mousePressEvent(event);
}

void NotifyListView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return) {
        QModelIndex index = this->model()->index(m_currentIndex, 0);
        QWidget *widget = this->indexWidget(index);

        if (qobject_cast<IconButton *> (m_prevElement) != nullptr) {
            IconButton *itemCloseBtn = qobject_cast<IconButton *> (m_prevElement);
            itemCloseBtn->clicked();
            m_currentIndex --;
        } else if (qobject_cast<Button *> (m_prevElement) != nullptr) {
            Button *actionBtn = qobject_cast<Button *> (m_prevElement);
            actionBtn->clicked();
            m_currentIndex --;
        } else {
            OverLapWidet *overLabWidget = qobject_cast<OverLapWidet *> (widget);
            if (overLabWidget != nullptr) {
                overLabWidget->expandAppGroup();
            }
        }

    }
    return QListView::keyPressEvent(event);
}

bool NotifyListView::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent *>(event);
        if (key->key() == Qt::Key_Tab) {
            if (m_aniState)
                return true;
            return tabKeyEvent(object, key);
        } else if (key->key() == Qt::Key_Down) {
            return true;
        } else if (key->key() == Qt::Key_Up) {
            return true;
        } else if (key->key() == Qt::Key_Backtab){
            m_currentIndex --;
            if (m_currentIndex < 0) {
                m_currentIndex = 0;
            }
            QModelIndex index = this->model()->index(m_currentIndex, 0);
            setCurrentIndex(index);
            return true;
        }
    }
    return QListView::eventFilter(object, event);
}

void NotifyListView::hideEvent(QHideEvent *event)
{
    m_currentIndex = 0;
    m_currentElement = nullptr;
    m_prevElement = nullptr;
    verticalScrollBar()->setValue(0);
    return QListView::hideEvent(event);
}

bool NotifyListView::tabKeyEvent(QObject *object, QKeyEvent *event)
{
    Q_UNUSED(object)
    Q_UNUSED(event)
    if (m_currentIndex >= model()->rowCount()) {
        m_currentIndex = 0;
    }
    QModelIndex index = this->model()->index(m_currentIndex, 0);
    QWidget *widget = this->indexWidget(index);
    scrollTo(index);
    if (widget == nullptr)
        return true;
    // 更新按钮列表
    QList<QPointer<QWidget>> elements;
    if (qobject_cast<BubbleTitleWidget *> (widget) != nullptr) {
        m_currentIndex ++;
        index = this->model()->index(m_currentIndex, 0);
        widget = this->indexWidget(index);
    }
    setCurrentIndex(index);
    m_prevElement = nullptr;
    if (qobject_cast<BubbleItem *> (widget) != nullptr) {
        BubbleItem *itemWidget = qobject_cast<BubbleItem *> (widget);
        elements = itemWidget->bubbleElements();
    } else if (qobject_cast<OverLapWidet *> (widget) != nullptr) {
        OverLapWidet *overLabWidget = qobject_cast<OverLapWidet *> (widget);
        elements = overLabWidget->bubbleElements();
        overLabWidget->setFocus();
    }
    if (m_currentElement == nullptr && !elements.isEmpty()) {
        m_currentElement = elements.first();
    } else {
        if (qobject_cast<IconButton *> (m_currentElement) != nullptr) {
            IconButton *itemCloseBtn = qobject_cast<IconButton *> (m_currentElement);
            itemCloseBtn->setFocusState(true);
        } else if (qobject_cast<Button *> (m_currentElement) != nullptr) {
            Button *actionBtn = qobject_cast<Button *> (m_currentElement);
            actionBtn->setFocus();
        }
        m_prevElement = m_currentElement;
        const int last_pos = elements.indexOf(m_currentElement) + 1;
        if (last_pos < elements.size()) {
            m_currentElement = elements.at(last_pos);
        } else { // 到item最后一个按钮
            m_currentElement = nullptr;
            m_currentIndex ++;
        }
    }
    return true;
}

void NotifyListView::wheelEvent(QWheelEvent *event)
{
    if (m_aniState)
        return;

    int offset = -event->delta();

    m_scrollAni->stop();
    m_scrollAni->setStartValue(verticalScrollBar()->value());
    m_scrollAni->setEndValue(verticalScrollBar()->value() + offset * m_speedTime);
    m_scrollAni->start();
    return QWidget::wheelEvent(event);
}

bool NotifyListView::canShow(EntityPtr ptr)
{
    QDateTime t = QDateTime::fromMSecsSinceEpoch(ptr->ctime().toLongLong());
    return t.secsTo(QDateTime::currentDateTime()) < OVERLAPTIMEOUT_4_HOUR;
}

void NotifyListView::handleScrollValueChanged()
{
    QScrollBar *vscroll = verticalScrollBar();

    if (vscroll->value() == vscroll->maximum() ||
        vscroll->value() == vscroll->minimum()) {
        blockSignals(false);
    } else {
        blockSignals(true);
    }
}

void NotifyListView::handleScrollFinished()
{
    blockSignals(false);

    QPoint pos = mapFromGlobal(QCursor::pos());
    emit entered(indexAt(pos));
}
