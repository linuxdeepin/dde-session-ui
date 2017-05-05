/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "rounditembutton.h"
#include "util_signalmanager.h"
#include "util_updateui.h"
#include "dbus/dbusvariant.h"
#include "dbus/dbuslogin1manager.h"
#include "common.h"

class MultiUsersWarningView;

class ShutDownFrame: public QFrame
{
    Q_OBJECT
public:
    ShutDownFrame(QWidget* parent=0);
    ~ShutDownFrame();

signals:
    void keyLeft();
    void keyRight();
    void pressEnterAction();
    void ShutDownFrameActions(const Actions action);
#ifdef SHUTDOWN_NO_QUIT
    void requestRecoveryLayout();
#endif

public slots:
    void setConfirm(const bool confirm);
    void powerAction(const Actions action);
    void setPreviousChildFocus();
    void setNextChildFocus();
    void showTips(const QString &tips);
    void hideBtns(const QStringList &btnsName);
    void disableBtns(const QStringList &btnsName);
    void onCancel();
    const QString getInhibitReason();
    void recoveryLayout();
private:
    void initUI();
    void initConnect();
    void enterKeyPushed();
    void hideBtn(const QString &btnName);
    void disableBtn(const QString &btnName);
    void beforeInvokeAction(const Actions action);

    RoundItemButton *m_currentSelectedBtn = nullptr;
    RoundItemButton *m_shutdownButton;
    RoundItemButton *m_restartButton;
    RoundItemButton *m_suspendButton;
    RoundItemButton *m_lockButton;
    RoundItemButton *m_logoutButton;
    RoundItemButton *m_switchUserBtn;
    QList<RoundItemButton *> *m_btnsList;

    QWidget *m_tipsWidget;
    QLabel *m_tipsLabel;
    DBusLogin1Manager* m_login1Inter;

    QWidget *m_warningView = nullptr;
    QVBoxLayout *m_mainLayout;

    bool m_confirm = false;
};
#endif // CONTENTVIEWWIDGET
