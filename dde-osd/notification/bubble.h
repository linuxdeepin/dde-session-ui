/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *             listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *             fanpengcheng <fanpengcheng_cm@deepin.com>
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

#ifndef BUBBLE_H
#define BUBBLE_H

#include <DBlurEffectWidget>
#include <DDialogCloseButton>

#include <QDBusArgument>
#include <QVariantAnimation>
#include <QParallelAnimationGroup>
#include <memory>

#include "constants.h"

DWIDGET_USE_NAMESPACE

class QPropertyAnimation;
class NotificationEntity;
class AppIcon;
class AppBody;
class Button;
class ActionButton;
class Bubble : public DBlurEffectWidget
{
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE setFixedGeometry)
public:
    Bubble(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr,
           OSD::ShowStyle style = OSD::ShowStyle::BUBBLEWINDOW);

    std::shared_ptr<NotificationEntity> entity() const;
    void setEntity(std::shared_ptr<NotificationEntity> entity);
    void setEnabled(bool enable);

    inline int bubbleIndex() {return m_bubbleIndex;}

    void startMove(const QRect &startRect, const QRect &endRect);       // 负责位置的移动
    void setBubbleIndex(int index);                                     // 设置通知的索引
    void updateGeometry();                                              // 更新通知的位置,分辨率被修改时使用

Q_SIGNALS:
    void expired(Bubble *);                                             // 超时消失时发出
    void dismissed(Bubble *);                                           // 点击后发出
    void notProcessedYet(Bubble *);                                     // 触发'暂不处理'操作时发出

    void actionInvoked(Bubble *, QString);

public Q_SLOTS:
    void onDelayQuit();
    void setFixedGeometry(QRect rect);

protected:
    virtual void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    virtual void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onOutTimerTimeout();
    void onOutAnimFinished();

private:
    void initUI();
    void initConnections();
    void initAnimations();
    void initTimers();

    void updateContent();
    bool containsMouse() const;

protected:
    std::shared_ptr<NotificationEntity> m_entity;

    //animation
<<<<<<< HEAD
    QPropertyAnimation *m_outAnimation = nullptr;
    QPropertyAnimation *m_moveAnimation = nullptr;//负责移入和变更位置
=======
//    QPropertyAnimation *m_opacityAnimation;
    QPropertyAnimation *m_geometryAnimation;

>>>>>>> feat：format code
    //controls
    AppIcon *m_icon;
    AppBody *m_body;
    ActionButton *m_actionButton;
    DDialogCloseButton *m_closeButton;

    QTimer *m_outTimer;
    QTimer *m_quitTimer;

    //---very private ,no get method
    QPoint m_clickPos;
    bool m_pressed = false;
<<<<<<< HEAD
    bool m_isDelete = false;
    OSD::ShowStyle m_showStyle;
=======
>>>>>>> feat：format code
    QString m_defaultAction;
    bool m_canClose = false;
    int m_bubbleIndex;

    QVariantAnimation *m_tranAnimation;
    QVariantAnimation *m_opacityAnimation;
};
#endif // BUBBLE_H
