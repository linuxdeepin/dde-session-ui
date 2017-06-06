/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>

#include "common.h"
#include "shutdownmanager.h"
#include "userwidget.h"

ShutdownManager::ShutdownManager(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
    initData();
}

void ShutdownManager::initConnect()
{
    connect(this, SIGNAL(DirectKeyLeft()) , m_content, SIGNAL(OutKeyLeft()));
    connect(this, SIGNAL(DirectKeyRight()), m_content, SIGNAL(OutKeyRight()));
    connect(this, SIGNAL(pressEnter()), m_content, SIGNAL(pressEnterAction()));

    connect(m_content->m_shutdownFrame, &ShutDownFrame::ShutDownFrameActions, this, &ShutdownManager::powerAction);
#ifdef SHUTDOWN_NO_QUIT
    connect(m_content->m_shutdownFrame, &ShutDownFrame::requestRecoveryLayout, this, [=]{
        hideToplevelWindow();
        m_content->m_shutdownFrame->recoveryLayout();
    });
#endif
//    connect(qApp, &QApplication::aboutToQuit, [this]{
//        m_hotZoneInterface->EnableZoneDetected(true);
//    });
}
//void ShutdownManager::shutDownFrameGrabKeyboard() {

//    qDebug() << "grabTimes:" << m_timerCount;
//    if (m_timerCount == 10) {
//        m_getFocusTimer->stop();
//    } else {
//        this->grabKeyboard();
//        m_timerCount++;
//    }
//}
void ShutdownManager::initUI()
{
    setObjectName("ShutdownManager");
    resize(qApp->desktop()->screenGeometry().size());

    m_content = new MainFrame(this);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addWidget(m_content);
    m_Layout->addStretch();

    checkUsers();

    QFile qssFile(":/skin/main.qss");
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if (qssFile.isOpen()) {
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }

    setLayout(m_Layout);
}

void ShutdownManager::showEvent(QShowEvent *e)
{
    if (!m_getFocusTimer) { m_getFocusTimer = new QTimer(this); }
    m_getFocusTimer->setInterval(100);
    m_getFocusTimer->start();
    connect(m_getFocusTimer,  &QTimer::timeout, this, &ShutdownManager::grabKeyboard);
    m_hotZoneInterface->EnableZoneDetected(false);

    checkUsers();

    QWidget::showEvent(e);
}

void ShutdownManager::hideEvent(QHideEvent *e)
{
    releaseKeyboard();
    m_hotZoneInterface->EnableZoneDetected(true);

    QWidget::hideEvent(e);
}

void ShutdownManager::initData()
{
    m_sessionInterface = new DBusSessionManagerInterface("com.deepin.SessionManager", "/com/deepin/SessionManager",
            QDBusConnection::sessionBus(), this);
    m_hotZoneInterface = new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone",
                                         QDBusConnection::sessionBus(), this);
//    m_hotZoneInterface->EnableZoneDetected(false);
}

void ShutdownManager::switchToGreeter()
{
    m_utilFile = new UtilFile(this);
    m_utilFile->setExpandState(1);

    QProcess *process = new QProcess;
    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);
    process->start("dde-locker -");

    hideToplevelWindow();
}

void ShutdownManager::hideToplevelWindow()
{
#ifdef SHUTDOWN_NO_QUIT
    QWidgetList widgets = qApp->topLevelWidgets();
    for (QWidget *widget : widgets) {
        if (widget->isVisible()) {
            widget->hide();
        }
    }
#else
    qApp->quit();
#endif
}

void ShutdownManager::checkUsers()
{
    // hide user switch btn when only 1 user avaliable
    UserWidget *users = new UserWidget;
    if (users->count() < 2) {
        m_content->hideBtns(QStringList() << "SwitchUser");
    }
    users->deleteLater();
}

void ShutdownManager::powerAction(const Actions action)
{
    // if we don't force this widget to hide, hideEvent will happen after
    // dde-lock showing, since hideEvent enables hot zone, hot zone will
    // take effect while dde-lock is showing.
#ifndef SHUTDOWN_NO_QUIT
    this->hide();
#endif

    switch (action) {
    case Shutdown:       m_sessionInterface->RequestShutdown();      break;
    case Restart:        m_sessionInterface->RequestReboot();        break;
    case Suspend:        m_sessionInterface->RequestSuspend();       break;
    case Lock:           m_sessionInterface->RequestLock();          break;
    case Logout:         m_sessionInterface->RequestLogout();        break;
    case SwitchUser:
    {
        QDBusInterface ifc("com.deepin.dde.lockFront",
                           "/com/deepin/dde/lockFront",
                           "com.deepin.dde.lockFront",
                           QDBusConnection::sessionBus(), NULL);
        ifc.asyncCall("ShowUserList");
        break;
    }
    default:
        qWarning() << "action: " << action << " not handled";
        break;
    }

    hideToplevelWindow();
}

void ShutdownManager::hideBtns(const QStringList &btnsName)
{
    m_content->hideBtns(btnsName);
}

void ShutdownManager::disableBtns(const QStringList &btnsName)
{
    m_content->disableBtns(btnsName);
}

void ShutdownManager::setConfrim(const bool confrim)
{
    m_content->setConfrim(confrim);
}

void ShutdownManager::keyPressEvent(QKeyEvent *e)
{

    switch (e->key()) {
    case Qt::Key_Escape:        hideToplevelWindow(); break; // must break;
    case Qt::Key_Return:        /* same as enter */
    case Qt::Key_Enter:         emit pressEnter();              break;
    case Qt::Key_Left:          emit DirectKeyLeft();           break;
    case Qt::Key_Right:         emit DirectKeyRight();          break;
    default:;
    }
}
void ShutdownManager::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        hideToplevelWindow();
        m_content->m_shutdownFrame->recoveryLayout();
    }
}

ShutdownManager::~ShutdownManager()
{
}

void ShutdownManager::powerActionFromExternal(const Actions action)
{
    m_content->powerAction(action);
}
