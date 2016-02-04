/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>
#include <QDebug>
#include <QWindow>

#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    // set window flags as dde-lock, so we can easily cover it.
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::FramelessWindowHint);

    setupSize();
    setStyleSheet("Window { background: black }");

    m_image = new QLabel(this);
    m_image->setPixmap(QPixmap(":/images/lowpower.png"));

    setupImagePosition();
}

Window::~Window()
{

}

void Window::setupSize()
{
    QDesktopWidget * desktop = QApplication::desktop();
    int screenCount = desktop->screenCount();

    int totalWidth = 0;
    int totalHeight = 0;
    for (int i = 0; i < screenCount; i++) {
        QWidget * screen = desktop->screen(i);
        totalWidth = qMax(totalWidth, screen->x() + screen->width());
        totalHeight = qMax(totalHeight, screen->y() + screen->height());
    }

    setFixedSize(totalWidth, totalHeight);
}

void Window::setupImagePosition()
{
    QPoint cursorPos = QCursor::pos();

    QDesktopWidget * desktop = QApplication::desktop();
    int screenNum = desktop->screenNumber(cursorPos);
    QWidget * screen = desktop->screen(screenNum);

    m_image->move(screen->geometry().center() - m_image->rect().center());
}

LowPowerAdaptor::LowPowerAdaptor(Window * parent) :
    QDBusAbstractAdaptor(parent)
{

}

LowPowerAdaptor::~LowPowerAdaptor()
{

}

void LowPowerAdaptor::Raise()
{
    Window * w = qobject_cast<Window*>(parent());
    if (w) {
        w->raise();
        w->activateWindow();
        w->grabMouse();
        w->grabKeyboard();
    }
}

void LowPowerAdaptor::Quit()
{
    qApp->quit();
}
