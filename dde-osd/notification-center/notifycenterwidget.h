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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "notifywidget.h"
#include "notification/constants.h"

#include <DBlurEffectWidget>
#include <DLabel>
#include <DWindowManagerHelper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class Persistence;
class QVariantAnimation;
class QPropertyAnimation;
class QSequentialAnimationGroup;
class NotifyCenterWidget : public DBlurEffectWidget
{
    Q_OBJECT
    Q_PROPERTY(int width WRITE setFixedWidth)
    Q_PROPERTY(int x WRITE setX)
public:
    explicit NotifyCenterWidget(Persistence *database = nullptr);
    void showWidget();
    void updateGeometry(QRect screen, QRect dock, OSD::DockPosition pos);

Q_SIGNALS:
    void notify();

protected:
    bool eventFilter(QObject *watched, QEvent *e) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void initUI();
    void initAnimations();
    void initConnections();
    void refreshTheme();

    void showAni();
    void hideAni();

private Q_SLOTS:
    void CompositeChanged();
    void setX(int x);

private:
    QWidget *m_headWidget;
    NotifyWidget *m_notifyWidget;
    DLabel *title_label = nullptr;
    QRect m_notifyRect;
    QRect m_dockRect;
    QPropertyAnimation *m_xAni;
    QPropertyAnimation *m_widthAni;
    QSequentialAnimationGroup *m_aniGroup;
    DWindowManagerHelper *m_wmHelper;
    bool m_hasComposite = false;
    QTimer *m_refreshTimer = nullptr;
};

#endif // MAINWIDGET_H
