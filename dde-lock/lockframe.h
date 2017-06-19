/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LOCKFRAME
#define LOCKFRAME

#include <QKeyEvent>

#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

#include "lockmanager.h"
#include "boxframe.h"

const QString DBUS_PATH = "/com/deepin/dde/lockFront";
const QString DBUS_NAME = "com.deepin.dde.lockFront";


class DBusLockService;
class LockFrame: public BoxFrame
{
    Q_OBJECT
public:
    LockFrame(QWidget* parent=0);
    ~LockFrame();

public slots:
    void showUserList();
    void updateScreenPosition();
    void tryGrabKeyboard();
#ifdef LOCK_NO_QUIT
    void hideFrame();
#endif
protected:
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *);

private:
    LockManager* m_lockManager;
    Display *m_display;
    int m_failures = 0;
};

#endif // LOCKFRAME

