/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

class QLabel;
class DeepinWave;
class DBusUpdateJob;
class DBusUpdateJobManager;
class Frame : public QWidget
{
    Q_OBJECT

public:
    Frame(QWidget *parent = 0);
    ~Frame();

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void showFullScreen();
    void distUpgrade();

private:
    DeepinWave * m_wave;
    QLabel * m_progressTip;
    QLabel * m_warningTip;
    DBusUpdateJobManager *m_dbusJobManagerInter;

private slots:
    void updateProgress(double progress);
    void tryReboot();
};

#endif // FRAME_H
