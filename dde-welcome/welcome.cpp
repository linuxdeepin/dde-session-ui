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

#include "welcome.h"

#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <QStandardPaths>

DWIDGET_USE_NAMESPACE

Welcome::Welcome(QWidget *parent)
    : QWidget(parent),
      m_loadingSpinner(new DPictureSequenceView(this))
{
    QStringList spinner;
    for (int i(0); i != 90; ++i)
        spinner << QString(":/loading_spinner/resources/loading_spinner/loading_spinner_%1.png").arg(QString::number(i), 3, '0');
    m_loadingSpinner->setPictureSequence(spinner);
    m_loadingSpinner->setFixedSize(32, 32);

    QTimer::singleShot(1, this, [this] () {
        raise();
        grabMouse();
        clearCursor();

        m_loadingSpinner->play();
    });

#ifdef QT_DEBUG
    show();
#endif
}

Welcome::~Welcome()
{
    qApp->restoreOverrideCursor();
}

void Welcome::clearCursor()
{
    qApp->setOverrideCursor(Qt::BlankCursor);

    const auto disp = XOpenDisplay(nullptr);
    Q_ASSERT(disp);
    const auto window = DefaultRootWindow(disp);

    Cursor invisibleCursor;
    Pixmap bitmapNoData;
    XColor black;
    static char noData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    black.red = black.green = black.blue = 0;

    bitmapNoData = XCreateBitmapFromData(disp, window, noData, 8, 8);
    invisibleCursor = XCreatePixmapCursor(disp, bitmapNoData, bitmapNoData,
                                          &black, &black, 0, 0);
    XDefineCursor(disp, window, invisibleCursor);
    XFixesChangeCursorByName(disp, invisibleCursor, "watch");
    XFreeCursor(disp, invisibleCursor);
    XFreePixmap(disp, bitmapNoData);
    XFlush(disp);
}

void Welcome::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    case Qt::Key_Enter:
    case Qt::Key_Return:
        qApp->quit();
    default:;
    }

    QWidget::keyPressEvent(e);
}

void Welcome::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    m_loadingSpinner->move(width() / 2 - m_loadingSpinner->width() / 2,
                           height() / 2 - m_loadingSpinner->height() / 2);
}
