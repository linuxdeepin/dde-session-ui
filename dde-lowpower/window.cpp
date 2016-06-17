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
    : QWidget(parent),
	m_image(new QLabel(this)),
	m_text(new QLabel(tr("Low battery, please plug in"), this))
{
    // set window flags as dde-lock, so we can easily cover it.
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::FramelessWindowHint);

    m_image->setPixmap(QPixmap(":/images/lowpower.png"));
    m_text->setStyleSheet("QLabel{ color:#ff8000; font-size:15px }");

    setupSize();
    setStyleSheet("Window { background: black }");
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

    QPoint cursorPos = QCursor::pos();
    int screenNum = desktop->screenNumber(cursorPos);
    QWidget * screen = desktop->screen(screenNum);

    m_image->setFixedSize(m_image->pixmap()->size());
    m_image->move(screen->geometry().center() - m_image->rect().center());

    m_text->setFixedSize(screen->geometry().width(), 30);
    m_text->setAlignment(Qt::AlignHCenter);
    m_text->move(0, m_image->y() + m_image->pixmap()->height());
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
