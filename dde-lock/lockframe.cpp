/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "lockframe.h"

LockFrame::LockFrame(QWidget* parent)
    : BoxFrame(parent)
{

    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "LockFrame geometry:" << geometry();

    m_lockManager = new LockManager(this);
    QPoint mousePoint = QCursor::pos();

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        if (rect.contains(mousePoint)) {
            m_lockManager->setFixedSize(rect.size());
            m_lockManager->move(rect.x(), rect.y());
            qDebug() << "loginManager:" << m_lockManager->geometry();
            m_lockManager->updateWidgetsPosition();
            continue;
        }
    }

    connect(this, &LockFrame::screenChanged, this, &LockFrame::updateScreenPosition);
    connect(m_lockManager, &LockManager::screenChanged, this, &LockFrame::updateScreenPosition);
#ifdef LOCK_NO_QUIT
    connect(m_lockManager, &LockManager::checkedHide, this, &LockFrame::hideFrame);
#endif
}

void LockFrame::showUserList() {
    show();
    emit m_lockManager->control()->switchUser();
//    m_lockManager->chooseUserMode();
}

void LockFrame::updateScreenPosition()
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList)
    {
        if (screen->geometry().contains(mousePoint))
        {
            const QRect &geometry = screen->geometry();
            m_lockManager->setFixedSize(geometry.size());
            m_lockManager->move(geometry.x(), geometry.y());
            m_lockManager->updateGeometry();
            m_lockManager->updateWidgetsPosition();

            return;
        }
    }
}

#ifdef LOCK_NO_QUIT
void LockFrame::hideFrame() {
    this->hide();
    m_lockManager->enableZone();
}

void LockFrame::showEvent(QShowEvent *) {
    this->raise();
    this->activateWindow();
    m_lockManager->disableZone();
}
#endif

void LockFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

LockFrame::~LockFrame() {

}

