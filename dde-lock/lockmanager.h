/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LOCKMANAGER_H
#define LOCKMANAGER_H

#include <QFrame>

#include "timewidget.h"
#include "shutdownwidget.h"
#include "userwidget.h"
#include "controlwidget.h"
#include "passwdedit.h"
#include "kblayoutwidget.h"

#include "dbus/dbuslockservice.h"
#include "dbus/dbusinputdevices.h"
#include "dbus/dbuskeyboard.h"
#include "dbus/dbusmediaplayer2.h"
#include "dbus/dbussessionmanager.h"
#include "dbus/dbushotzone.h"
#include "darrowrectangle.h"

#define LOCKSERVICE_PATH "/com/deepin/dde/lock"
#define LOCKSERVICE_NAME "com.deepin.dde.lock"

#define LOCK_KEYBOARDLAYOUT_PATH "/com/deepin/daemon/InputDevice/Keyboard"
#define LOCK_KEYBOARDLAYOUT_NAME "com.deepin.daemon.InputDevice"

class LockManager : public QFrame
{
    Q_OBJECT

public:
    enum Actions {Unlock, Restart, Suspend, Shutdown};

public:
    LockManager(QWidget* parent = 0);

    void initUI();
    void initConnect();
    void initData();
    void updateUI();
signals:
    void screenChanged(QRect rect);
#ifdef LOCK_NO_QUIT
    void checkedHide();
#endif
public slots:
    void setCurrentKeyboardLayout(QString keyboard_value);
    void passwordMode();
    void shutdownMode();
    void updateWidgetsPosition();
    void updateBackground(QString username);

    void keybdLayoutWidgetPosit();
    void leftKeyPressed();
    void rightKeyPressed();
protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

private:
    void loadMPRIS();
    void initBackend();
    void unlock();

private:
    Actions m_action = Unlock;

    ShutdownWidget* m_requireShutdownWidget;
    TimeWidget *m_timeWidget;
    UserWidget *m_userWidget;
    ControlWidget *m_controlWidget;
    PassWdEdit *m_passwordEdit;
    DBusLockService *m_lockInter;

    QMap<QString, QString> m_keybdInfoMap;
    DBusKeyboard* m_keyboardLayoutInterface;
    QStringList m_keybdLayoutNameList;
    QStringList keybdLayoutDescList;
    KbLayoutWidget* m_keybdLayoutWidget;
    DArrowRectangle* m_keybdArrowWidget;
    DBusSessionManagerInterface* m_sessionManagerIter;

    DBusMediaPlayer2 *m_mprisInter = nullptr;
    DBusHotzone* m_hotZoneInterface;
    int m_keybdLayoutItemIndex;


};

#endif // LOCKMANAGER_H
