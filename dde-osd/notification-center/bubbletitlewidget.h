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

#ifndef BUBBLETITLEWIDGET_H
#define BUBBLETITLEWIDGET_H
#include "notification/constants.h"
#include "notifymodel.h"
#include "bubbleitem.h"
#include <DWidget>
#include <DIconButton>
#include <DLabel>

DWIDGET_USE_NAMESPACE

class NotifyListView;

class BubbleTitleWidget : public DWidget
{
    Q_OBJECT
public:
    BubbleTitleWidget(NotifyModel *model = nullptr, EntityPtr entity = nullptr, QWidget *parent = nullptr);
    void setParentView(NotifyListView *view);

public:
    void setIndexRow(int row);
    int indexRow() { return m_indexRow; }
    QList<QPointer<QWidget>> bubbleElements();
    static int bubbleTitleWidgetHeight();

protected:
    void enterEvent(QEvent *event) override;        //鼠标移动到窗口上,显示气泡组的关闭按钮
    void leaveEvent(QEvent *event) override;        //鼠标移出窗口,气泡组的关闭按钮不显示
    void focusInEvent(QFocusEvent *event) override;          //当焦点移入或移出时背景发生变化
    void focusOutEvent(QFocusEvent *event) override;

private:
    DLabel *m_titleLabel;
    DIconButton *m_closeButton;
    NotifyModel *m_model;
    int m_indexRow = 0;
    NotifyListView *m_view = nullptr;
};

#endif // BUBBLETITLEWIDGET_H
