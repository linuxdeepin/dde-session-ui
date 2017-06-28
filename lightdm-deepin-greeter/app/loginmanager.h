/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LoginManager_H
#define LoginManager_H

#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include "util_updateui.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "view/logowidget.h"
#include "view/switchframe.h"
#include "sessionwidget.h"
#include "kblayoutwidget.h"
#include "shutdownwidget.h"
#include "xkbparser.h"
#include "dbus/dbuskeyboard.h"
#include "keyboardmonitor.h"

#include "dbus/dbusdisplaymanager.h"
#include "dbus/dbusvariant.h"
#include "dbus/dbuslogin1manager.h"
#include "dbus/dbuslockservice.h"

#include "darrowrectangle.h"
#include "util_file.h"

class LoginManager: public QFrame {
    Q_OBJECT
public:
    LoginManager(QWidget* parent=0);
    ~LoginManager();
signals:
    /*This signals is used to change the
    widgets position in different screens*/
    void screenChanged(QRect geom);

public slots:
    /*Update the position of the widgets after finished the layout of ui*/
    void updateWidgetsPosition();
    void updateBackground(QString username);
    void updateUserLoginCondition(QString username);

    void authenticate();
    void startSession();

protected:
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

private:
    void recordPid();
    void initUI();
    void initData();
    void initConnect();

    void initDateAndUpdate();

    void expandUserWidget();

    void message(QString text, QLightDM::Greeter::MessageType type);
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void authenticationComplete();

    void login();

    void chooseUserMode();
    void chooseSessionMode();
    void choosedSession();

    void showShutdownFrame();
    void keyboardLayoutUI();
    void keybdLayoutWidgetPosit();
    void setCurrentKeybdLayoutList(QString keyboard_value);

    void setShutdownAction(const ShutdownWidget::Actions action);
    void leftKeyPressed();
    void rightKeyPressed();

    void saveNumlockStatus(const bool &on);
    void restoreNumlockStatus();

    LogoWidget* m_logoWidget;
    SwitchFrame* m_switchFrame;
    UserWidget* m_userWidget;
    PassWdEdit* m_passWdEdit;
    QPushButton * m_loginButton;
    SessionWidget *m_sessionWidget;
    QHBoxLayout* m_passWdEditLayout;
    QVBoxLayout* m_Layout;

    ShutdownWidget* m_requireShutdownWidget;
    KbLayoutWidget* m_keybdLayoutWidget;
    DArrowRectangle* m_keybdArrowWidget;
    XkbParser* xkbParse;

    QLightDM::Greeter *m_greeter;
    DBusLogin1Manager* m_login1ManagerInterface;
    UtilFile* m_utilFile;

    KeyboardMonitor *m_keyboardMonitor;

    QSize m_passwdEditSize;
    QStringList m_kbdList;
    QStringList m_kbdParseList;

    int m_authFailureCount;
};
#endif // LoginManager

