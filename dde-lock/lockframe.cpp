/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
