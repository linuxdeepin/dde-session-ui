/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef SUSPENDDIALOG_H
#define SUSPENDDIALOG_H

#include <ddialog.h>

#include <QTimer>

DWIDGET_USE_NAMESPACE

class SuspendDialog : public DDialog
{
    Q_OBJECT
public:
    explicit SuspendDialog(QRect screenGeometry);
    ~SuspendDialog() {}

protected:
    void showEvent(QShowEvent *event);

private:
    QRect m_screenGeometry;

    QTimer m_timer;
    int m_timerTick;
};

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager();
    ~Manager() {}

signals:
    void finished(const int code);

private:
    void setupDialogs();
};

#endif // SUSPENDDIALOG_H
