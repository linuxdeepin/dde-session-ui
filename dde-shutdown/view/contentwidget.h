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

#include "userwidget.h"
#include "rounditembutton.h"
#include "util_updateui.h"
#include "dbus/dbusvariant.h"
#include "dbus/dbuslogin1manager.h"
#include "common.h"
#include "dbus/dbussessionmanager.h"
#include "dbus/dbushotzone.h"
#include "systemmonitor.h"

class MultiUsersWarningView;

class ContentWidget: public QFrame
{
    Q_OBJECT
public:
    ContentWidget(QWidget* parent=0);
    ~ContentWidget();

signals:
#ifdef SHUTDOWN_NO_QUIT
    void requestRecoveryLayout();
#endif

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

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
    void runSystemMonitor();

private:
    void initUI();
    void initConnect();
    void initData();
    void enterKeyPushed();
    void hideBtn(const QString &btnName);
    void disableBtn(const QString &btnName);
    void beforeInvokeAction(const Actions action);
    void hideToplevelWindow();
    void checkUsers();
    void shutDownFrameActions(const Actions action);

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
    UserWidget *m_userWidget;

    bool m_confirm = false;

    DBusSessionManagerInterface* m_sessionInterface = nullptr;
    DBusHotzone* m_hotZoneInterface = nullptr;
    SystemMonitor *m_systemMonitor;
};
#endif // CONTENTVIEWWIDGET
