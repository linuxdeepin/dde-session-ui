/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MAINWINDOWWITHCLICK_H
#define MAINWINDOWWITHCLICK_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>

#include "dbus/dbuszone.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    QStringList m_ButtonNames;
    QStringList m_ActionStrs;
    QStringList m_ActionStrs2;

    ZoneInterface *m_dbusZoneInter;

    // all the actions' dbus-about string
    const QString CONTROL_CENTER_FROM_LEFT_STR = "dbus-send --print-reply --dest=com.deepin.dde.ControlCenter /com/deepin/dde/ControlCenter com.deepin.dde.ControlCenter.ToggleInLeft";
    const QString CONTROL_CENTER_FROM_RIGHT_STR = "dbus-send --print-reply --dest=com.deepin.dde.ControlCenter /com/deepin/dde/ControlCenter com.deepin.dde.ControlCenter.Toggle";
    const QString ALL_WINDOWS_STR = "dbus-send --session --dest=com.deepin.wm --print-reply /com/deepin/wm com.deepin.wm.PerformAction int32:6";
    const QString LAUNCHER_STR = "/usr/bin/dde-launcher";
    const QString SHOW_DESKTOP_STR = "/usr/lib/deepin-daemon/desktop-toggle";
    const QString NONE_STR = "";

    // MAIN_ITEM_TOP_MARGIN is aimed to steer clear of the fade-zone of mouseEvent of mainWindow Item.
    const int MAIN_ITEM_TOP_MARGIN = 30;
};

#endif // MAINWINDOWWITHCLICK_H
