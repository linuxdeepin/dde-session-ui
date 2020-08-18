/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
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

#ifndef OverLapWidet_H
#define OverLapWidet_H

#include "bubbleitem.h"
#include "../notification/constants.h"

#include <DWidget>

DWIDGET_USE_NAMESPACE

class NotifyListView;
class NotifyModel;

class HalfRoundedRectWidget : public AlphaWidget
{
    Q_OBJECT

public:
    HalfRoundedRectWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

class OverLapWidet : public DWidget
{
    Q_OBJECT

public:
    OverLapWidet(NotifyModel *model,EntityPtr ptr,QWidget * parent = nullptr);

public:
    void setIndexRow(int row);
    void setParentView(NotifyListView *view);
    BubbleItem *getFaceItem() { return m_faceBubbleItem; }
    QList<QPointer<QWidget>> bubbleElements();
    void expandAppGroup();

private:
    void initOverlap();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE; //鼠标点击时展开重叠的气泡
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;          //当焦点移入或移出时背景发生变化
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    int m_indexRow = 0;
    EntityPtr m_entify;
    NotifyModel *m_model;
    NotifyListView *m_view;
    BubbleItem *m_faceBubbleItem;
};

#endif // OverLapWidet_H
