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
#include <DLabel>
#include "notification/constants.h"

DWIDGET_USE_NAMESPACE

class Persistence;
class QPropertyAnimation;

class NotifyCenterWidget : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit NotifyCenterWidget(Persistence *database = nullptr);
    void showWidget();
    void updateGeometry(OSD::DockPosition pos, int dock_size);

Q_SIGNALS:
    void notify();

protected:
    void showEvent(QShowEvent *event) override;

private:
    void initUI();
    void initAnimations();
    void refreshTheme();

private:
    QWidget *m_headWidget;
    NotifyWidget *m_notifyWidget;
    DLabel *title_label = nullptr;
    QRect m_screenGeometry;

    QPropertyAnimation *m_inAnimation = nullptr;
    QPropertyAnimation *m_outAnimation = nullptr;
};

#endif // MAINWIDGET_H
