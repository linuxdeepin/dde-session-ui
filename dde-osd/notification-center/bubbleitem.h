/*
 * Copyright (C) 2011 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zorowk <near.kingzero@gmail.com>
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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

#ifndef BUBBLEITEM_H
#define BUBBLEITEM_H

#include <DWidget>
#include <DLabel>

#include <QDBusArgument>

#include "notification/constants.h"

class NotificationEntity;
class NotifyModel;
class AppIcon;
class AppBody;
class Button;
class ActionButton;
class IconButton;
class NotifyModel;
class NotifyListView;

DWIDGET_USE_NAMESPACE
/*!
 * \~chinese \class AlphaWidget
 * \~chinese \brief 绘制一个无边框,带有透明度的窗口
 */
class AlphaWidget : public DWidget
{
public:
    AlphaWidget(QWidget *parent = nullptr);

    //设置和获取窗口属性的接口
    void setAlpha(int alpha) {m_hoverAlpha = alpha; m_unHoverAlpha = alpha; update();}

    int hoverAlpha() {return m_hoverAlpha;}
    void setHoverAlpha(int alpha) {m_hoverAlpha = alpha; update();}

    int unHoverAlpha() {return m_unHoverAlpha;}
    void setUnHoverAlpha(int alpha) {m_unHoverAlpha = alpha; update();}

    void setHasFocus(bool focus) { m_hasFocus = focus; }
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

protected:
    int m_hoverAlpha = 0;
    int m_unHoverAlpha = 0;
    bool m_hasFocus = false;
};

/*!
 * \~chinese \class ItemWidget
 * \~chinese \brief 通知中心中的气泡窗口类
 */
class BubbleItem : public QWidget
{
    Q_OBJECT
public:
    BubbleItem(QWidget *parent = nullptr, EntityPtr entity = nullptr);
    ~BubbleItem() override;
    void setParentModel(NotifyModel *model);
    void setParentView(NotifyListView *view);
    const QPixmap converToPixmap(const QDBusArgument &value);
    void setAlpha(int alpha);
    QList<QPointer<QWidget>> bubbleElements();
    void setIndexRow(int row);
    int indexRow() { return m_indexRow; }
    void setHasFocus(bool focus);

Q_SIGNALS:
    void havorStateChanged(bool);

public Q_SLOTS:
    void onHavorStateChanged(bool hover);
    void onCloseBubble();
    void onRefreshTime();
    void setOverlapWidget(bool isOverlap);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;       //获取当前鼠标点击的位置
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;     //判断鼠标松开的位置是否与点击的位置相同,相同移除通知
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;                 //鼠标移动到通知窗口上,隐藏时间显示关闭按钮
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;                 //鼠标移出通知窗口,隐藏关闭按钮显示时间
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;          //当焦点移入或移出时背景发生变化
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();          //初始化UI界面
    void initContent();     //初始化信号槽连接
    void refreshTheme();    //刷新主题

private:
    EntityPtr m_entity;
    NotifyModel *m_model = nullptr;
    NotifyListView *m_view = nullptr;

    //controls
    AlphaWidget *m_bgWidget = nullptr;
    AlphaWidget *m_titleWidget = nullptr;
    AlphaWidget *m_bodyWidget = nullptr;
    DLabel *m_appNameLabel = nullptr;
    DLabel *m_appTimeLabel = nullptr;

    AppIcon *m_icon = nullptr;
    AppBody *m_body = nullptr;
    ActionButton *m_actionButton = nullptr;
    IconButton *m_closeButton = nullptr;
    QWidget *m_currentElement = nullptr;

    bool m_showContent = true;
    QString m_defaultAction;
    QPoint m_pressPoint;
    int m_indexRow = 0;
    bool m_isOverlapWidget = false;
};

#endif // BUBBLEITEM_H
