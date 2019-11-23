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
#include <DBlurEffectWidget>
#include "notification/constants.h"

DWIDGET_BEGIN_NAMESPACE
class DTipLabel;
DWIDGET_END_NAMESPACE

class Persistence;
class QPropertyAnimation;

class NotifyCenterWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit NotifyCenterWidget(Persistence *database = nullptr);
    void showWidget();
    void updateGeometry(QRect screen, QRect dock, OSD::DockPosition pos);

Q_SIGNALS:
    void notify();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void initUI();
    void refreshTheme();

private:
    QWidget *m_headWidget;
    NotifyWidget *m_notifyWidget;
    DTipLabel *title_label = nullptr;
    bool m_visible = false;
};

#endif // MAINWIDGET_H
