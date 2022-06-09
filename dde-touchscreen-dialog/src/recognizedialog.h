/*
 * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
 *
 * Author:     Zhihsian Que <i@zhihsian.me>
 *
 * Maintainer: Zhihsian Que <i@zhihsian.me>
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

#ifndef RECOGNIZEDIALOG_H
#define RECOGNIZEDIALOG_H

#include <com_deepin_daemon_display_monitor.h>

#include <DBlurEffectWidget>

#include <QBrush>
#include <QPen>

DWIDGET_USE_NAMESPACE
using namespace com::deepin::daemon::display;

class RecognizeDialog : public DBlurEffectWidget
{
    Q_OBJECT

public:
    explicit RecognizeDialog(Monitor *monitor, QWidget *parent = nullptr);
    void setText(const QString text);

protected:
    void paintEvent(QPaintEvent *);

public Q_SLOTS:
    void onScreenRectChanged();

private:
    Monitor *m_monitor;
    QRect m_rect;
    QString m_text;
};

#endif // !RECOGNIZEDIALOG_H
