/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef SHUTDOWNWIDGET
#define SHUTDOWNWIDGET

#include <QFrame>

#include "util_updateui.h"
#include "rounditembutton.h"

class ShutdownWidget: public QFrame
{
    Q_OBJECT
public:
    ShutdownWidget(QWidget* parent = 0);
    ~ShutdownWidget();

    enum Actions {
        RequireShutdown,
        RequireRestart,
        RequireSuspend,
    };

signals:
    void shutDownWidgetAction(const Actions sd);
    void directLeft();
    void directRight();
public slots:
    void leftKeySwitch();
    void rightKeySwitch();
    void shutdownAction();
private:
    void initUI();
    void initConnect();

    int m_index = 0;
    QHBoxLayout* m_Layout;
    QList<RoundItemButton*>* m_btnList;
    RoundItemButton* m_currentSelectedBtn = NULL;
    RoundItemButton* m_requireShutdownButton;
    RoundItemButton* m_requireRestartButton;
    RoundItemButton* m_requireSuspendBUtton;

};
#endif // SHUTDOWNWIDGET

