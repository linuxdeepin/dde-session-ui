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

#include <com_deepin_daemon_display.h>

#include <QBrush>
#include <QDialog>
#include <QPen>

using namespace com::deepin::daemon;

class RecognizeDialog : public QDialog {
    Q_OBJECT

public:
    explicit RecognizeDialog(Display *display, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private Q_SLOTS:
    void onScreenRectChanged();

private:
    void paintMonitorMark(QPainter &painter,
                          const QRect &rect,
                          const QString &name,
                          const QStringList &touchscreens);
    const QScreen *screenForGeometry(const QRect &rect) const;
    bool monitorsIsIntersect() const;

private:
    Display *m_displayInter;
    QPen m_backgroundPen;
    QPen m_textPen;
    QBrush m_backgroundBrush;
    QBrush m_textBrush;
};

#endif // !RECOGNIZEDIALOG_H
