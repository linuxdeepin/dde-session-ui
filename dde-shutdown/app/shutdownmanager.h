/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef  SHUTDOWNMANAGER_H
#define SHUTDOWNMANAGER_H
#include <QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>

#include "view/mainframe.h"
#include "dbus/dbussessionmanager.h"
#include "dbus/dbushotzone.h"
#include "backgroundlabel.h"
#include "util_signalmanager.h"
#include "util_file.h"

class ShutdownManager : public QFrame
{
    Q_OBJECT
public:
    ShutdownManager(QWidget* parent = 0);
    ~ShutdownManager();

signals:
    void DirectKeyLeft();
    void DirectKeyRight();
    void pressEnter();

public slots:
    void powerAction(const ShutDownFrame::Actions action);
    void hideBtns(const QStringList &btnsName);
    void disableBtns(const QStringList &btnsName);
//    void shutDownFrameGrabKeyboard();

protected:
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    QTimer* m_getFocusTimer = nullptr;
    QHBoxLayout* m_Layout = nullptr;
    MainFrame* m_content = nullptr;

    DBusSessionManagerInterface* m_sessionInterface = nullptr;
    DBusHotzone* m_hotZoneInterface = nullptr;
    UtilFile* m_utilFile = nullptr;

//    int m_timerCount = 0;
    void initUI();
    void initConnect();
    void initData();
    void switchToGreeter();
    void hideToplevelWindow();
};

#endif // SessionManagerTool_H
