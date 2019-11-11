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

#include "bubbleabstract.h"
#include <memory>

#include "constants.h"

DWIDGET_USE_NAMESPACE

class QPropertyAnimation;
class QParallelAnimationGroup;
class NotificationEntity;
class QGraphicsDropShadowEffect;
class Bubble : public BubbleAbStract
{
    Q_OBJECT
public:
    Bubble(QWidget *parent = nullptr, std::shared_ptr<NotificationEntity> entity = nullptr,
           OSD::ShowStyle style = OSD::ShowStyle::BUBBLEWINDOW);

    std::shared_ptr<NotificationEntity> entity() const;
    void StartMoveIn(int x, int y);
    void StartMoveIn(const QRect &startRect, const QRect &endRect);
    void setEntity(std::shared_ptr<NotificationEntity> entity);

    OSD::ShowStyle showStyle() {return m_showStyle;}

    void setEnabled(bool enable);

Q_SIGNALS:
    void expired(Bubble *);
    void dismissed(Bubble *);
    void ignored(Bubble *);     //暂不处理信号
    void replacedByOther(Bubble *);
    void actionInvoked(Bubble *, QString);

public Q_SLOTS:
    void onDelayQuit();
    void startMoveAnimation(const QRect &startRect, const QRect &endRect);

protected:
    virtual void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void onOutTimerTimeout();
    void onOutAnimFinished();
    void onDismissAnimFinished();

private:
    void initUI();
    void initConnections();
    void initAnimations();
    void initTimers();

protected:
    //animation
    QPropertyAnimation *m_outAnimation = nullptr;
    QPropertyAnimation *m_dismissAnimation = nullptr;
    QPropertyAnimation *m_moveAnimation = nullptr;//负责移入和变更位置

    QTimer *m_outTimer = nullptr;
    QTimer *m_quitTimer = nullptr;

    //---very private ,no get method
    QPoint m_clickPos;
    bool m_pressed = false;
    OSD::ShowStyle m_showStyle;
};

class BubbleTemplate : public DBlurEffectWidget
{
    Q_OBJECT
public:
    BubbleTemplate();

private:
    void initUI();

protected:
    bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;
};

#endif // BUBBLE_H
