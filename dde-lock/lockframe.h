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
    void updateScreenPosition(QRect rect);
#ifdef LOCK_NO_QUIT
    void hideFrame();
#endif
protected:
    void keyPressEvent(QKeyEvent *e);
#ifdef LOCK_NO_QUIT
    void showEvent(QShowEvent *);
#endif
private:
    LockManager* m_lockManager;
};

#endif // LOCKFRAME

