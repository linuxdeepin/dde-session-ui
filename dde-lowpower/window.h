/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDBusAbstractAdaptor>

class QLabel;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    QLabel * m_image;
    QLabel * m_text;

    void setupSize();
    void setupUI();
};

class LowPowerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.LowPower")

public:
    LowPowerAdaptor(Window * parent);
    ~LowPowerAdaptor();

public slots:
    void Raise();
    void Quit();
};

#endif // WINDOW_H
