/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "lockframe.h"
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <QApplication>

LockFrame::LockFrame(QWidget* parent)
    : FullscreenBackground(parent)
{
    m_display = QX11Info::display();

    qDebug() << "LockFrame geometry:" << geometry();

    m_lockManager = new LockManager(this);

    setContent(m_lockManager);

#ifdef LOCK_NO_QUIT
    connect(m_lockManager, &LockManager::checkedHide, this, &LockFrame::hideFrame);
#endif
}

void LockFrame::showUserList() {
    show();
    emit m_lockManager->control()->requestSwitchUser();
//    m_lockManager->chooseUserMode();
}

void LockFrame::tryGrabKeyboard()
{
    int requestCode = XGrabKeyboard(m_display, winId(), true, GrabModeAsync, GrabModeAsync, CurrentTime);

    if (requestCode != 0) {
        m_failures++;

        if (m_failures == 15) {
            qDebug() << "Trying grabkeyboard has exceeded the upper limit. dde-lock will quit.";
            qApp->quit();
        }

        QTimer::singleShot(100, this, &LockFrame::tryGrabKeyboard);
    }
}

#ifdef LOCK_NO_QUIT
void LockFrame::hideFrame() {
    this->hide();
    m_lockManager->enableZone();
}
#endif

void LockFrame::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:    qApp->quit();       break;
#endif
    }
}

LockFrame::~LockFrame() {

}
