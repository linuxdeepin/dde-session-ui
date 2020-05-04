/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef NOTIFYLISTVIEW_H
#define NOTIFYLISTVIEW_H

#include "notification/constants.h"
#include "notifymodel.h"

#include <QListView>
#include <QPoint>

class QPropertyAnimation;
class QScrollBar;
class QTimer;

class NotifyListView : public QListView
{
    Q_OBJECT
public:
    NotifyListView(QWidget *parent = nullptr);

public:
    void createAddedAnimation(EntityPtr entity, const ListItem appItem);
    void createRemoveAnimation(int idx);
    void createExpandAnimation(int idx, const ListItem appItem);
    void setAniState(bool state);
    bool aniState() { return m_aniState; }

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    bool tabKeyEvent(QObject *object, QKeyEvent *event);   //处理键盘TAB键按下的事件,QListView过滤了TAB按键事件
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

private:
    bool canShow(EntityPtr ptr); // 判断消息是否应该层叠[即超过4小时]
    void handleScrollValueChanged();
    void handleScrollFinished();

signals:
    void removeAniFinished();
    void expandAniFinished(QString appName);
    void addedAniFinished(EntityPtr entity);
    void refreshItemTime();

private:
    bool m_aniState = false;
    bool m_scrollState = false;
    bool m_pressState = false;
    int m_carrentIndex = 0;
    int m_moveCount = 0;
    double m_speedTime = 2.0;
    QPropertyAnimation *m_scrollAni;
    QPointer<QWidget> m_prevElement = nullptr;
    QPointer<QWidget> m_currentElement = nullptr;
    QTimer *m_refreshTimer = nullptr;
};

#endif // NOTIFYLISTVIEW_H
