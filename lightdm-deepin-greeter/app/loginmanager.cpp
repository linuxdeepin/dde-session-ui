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
#include <QDesktopWidget>
#include <QDebug>

#include "loginmanager.h"
#include "dbus/dbuslockservice.h"
#include "dbus/dbusaccounts.h"

#include <com_deepin_daemon_accounts_user.h>

#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>

#define LOCKSERVICE_PATH "/com/deepin/dde/LockService"
#define LOCKSERVICE_NAME "com.deepin.dde.LockService"

using DBusUser = com::deepin::daemon::accounts::User;

static const QSize ZoreSize = QSize(0, 0);

//Load the System cursor --begin
static XcursorImages*
xcLoadImages(const char *image, int size)
{
    QSettings settings(DDESESSIONCC::DEFAULT_CURSOR_THEME, QSettings::IniFormat);
    //The default cursor theme path
    qDebug() << "Theme Path:" << DDESESSIONCC::DEFAULT_CURSOR_THEME;
    QString item = "Icon Theme";
    const char* defaultTheme = "";
    QVariant tmpCursorTheme = settings.value(item + "/Inherits");
    if (tmpCursorTheme.isNull()) {
        qDebug() << "Set a default one instead, if get the cursor theme failed!";
        defaultTheme = "Deepin";
    } else {
        defaultTheme = tmpCursorTheme.toString().toLocal8Bit().data();
    }

    qDebug() << "Get defaultTheme:" << tmpCursorTheme.isNull()
             << defaultTheme;
    return XcursorLibraryLoadImages(image, defaultTheme, size);
}

static unsigned long loadCursorHandle(Display *dpy, const char *name, int size)
{
    if (size == -1) {
        size = XcursorGetDefaultSize(dpy);
    }

    // Load the cursor images
    XcursorImages *images = NULL;
    images = xcLoadImages(name, size);

    if (!images) {
        images = xcLoadImages(name, size);
        if (!images) {
            return 0;
        }
    }

    unsigned long handle = (unsigned long)XcursorImagesLoadCursor(dpy,
                          images);
    XcursorImagesDestroy(images);

    return handle;
}

static int set_rootwindow_cursor() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        qDebug() << "Open display failed";
        return -1;
    }

    Cursor cursor = (Cursor)XcursorFilenameLoadCursor(display, "/usr/share/icons/deepin/cursors/loginspinner");
    if (cursor == 0) {
        cursor = (Cursor)loadCursorHandle(display, "watch", 24);
    }
    XDefineCursor(display, XDefaultRootWindow(display),cursor);

    // XFixesChangeCursorByName is the key to change the cursor
    // and the XFreeCursor and XCloseDisplay is also essential.

    XFixesChangeCursorByName(display, cursor, "watch");

    XFreeCursor(display, cursor);
    XCloseDisplay(display);

    return 0;
}
// Load system cursor --end

LoginManager::LoginManager(QWidget* parent)
    : QFrame(parent),
      m_keyboardMonitor(KeyboardMonitor::instance())
{
//    recordPid();

    initUI();
    initData();
    initConnect();
    initDateAndUpdate();

    m_keyboardMonitor->start(QThread::LowestPriority);
    QTimer::singleShot(0, this, &LoginManager::restoreNumlockStatus);

    const QString u = m_userWidget->currentUser();
    qDebug() << Q_FUNC_INFO << "current user: " << u;

    m_passWdEdit->show();
    m_passWdEdit->setFocus();
    updateUserLoginCondition(u);

    m_sessionWidget->switchToUser(u);
    m_controlWidget->chooseToSession(m_sessionWidget->currentSessionName());

    QTimer::singleShot(1, this, [=] { updateBackground(u); });
}

void LoginManager::updateWidgetsPosition()
{
    const int h = height();
    const int w = width();

    m_userWidget->setFixedWidth(w);
    m_userWidget->move(0, (h - m_userWidget->height()) / 2 - 95); // center and margin-top: -95px
    qDebug() << "Change Screens" << m_userWidget->isChooseUserMode;
    m_sessionWidget->setFixedWidth(w);
    m_sessionWidget->move(0, (h - m_sessionWidget->height()) / 2 - 70); // 中间稍往上的位置
    m_logoWidget->move(48, h - m_logoWidget->height() - 36); // left 48px and bottom 36px
    m_controlWidget->move(w - m_controlWidget->width(), h - m_controlWidget->height() - 36);
    m_passWdEdit->move((w - m_passWdEdit->width()) / 2, (h - m_passWdEdit->height()) / 2);
    m_requireShutdownWidget->setFixedWidth(w);
    m_requireShutdownWidget->setFixedHeight(300);
    m_requireShutdownWidget->move(0,  (h - m_requireShutdownWidget->height())/2 - 60);
}

void LoginManager::updateBackground(QString username)
{
    emit requestBackground(m_userWidget->getUserGreeterBackground(username));
}

void LoginManager::updateUserLoginCondition(QString username)
{
    QProcess p;
    p.start("groups", QStringList(username));
    p.waitForFinished();

    QString output = p.readAllStandardOutput().trimmed();
    QStringList tokens = output.split(":");
    if (tokens.length() > 1) {
        QStringList groups = tokens.at(1).split(" ");
        qDebug() << groups;
        if (groups.contains("nopasswdlogin")) {
            m_passWdEdit->setFixedSize(ZoreSize);
//            m_loginButton->show();

            return;
        }
    }

//    m_loginButton->hide();
}

void LoginManager::startSession()
{
    qDebug() << "start session = " << m_sessionWidget->currentSessionName();

    // save session
    m_userWidget->saveLastUser();
    m_sessionWidget->saveUserSession();

    m_greeter->startSessionSync(m_sessionWidget->currentSessionKey());
}

void LoginManager::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

     QTimer::singleShot(1, this, &LoginManager::updateWidgetsPosition);
}

void LoginManager::keyPressEvent(QKeyEvent* e) {
    qDebug() << "key pressed: " << e->text();

    if (e->key() == Qt::Key_Escape) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (!m_userWidget->isChooseUserMode) {
                if (m_passWdEdit->size() != ZoreSize) {
                    m_passWdEdit->show();
                    m_passWdEdit->setFocus();
                } else {
//                    m_loginButton->show();
                }
            }
        }
#ifdef QT_DEBUG
        qApp->exit();
#endif
    }
}

void LoginManager::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (!m_userWidget->isChooseUserMode) {
                if (m_passWdEdit->size() != ZoreSize) {
                    m_passWdEdit->show();
                    m_passWdEdit->setFocus();
                } else {
//                    m_loginButton->show();
                }
            }
        }

        if (m_keybdArrowWidget->isHidden()) {
            m_keybdArrowWidget->hide();
        }
    }
}

void LoginManager::initUI()
{
//    setFixedSize(qApp->desktop()->size());
    setObjectName("LoginManagerTool");

    m_controlWidget = new ControlWidget(this);

    m_sessionWidget = new SessionWidget(this);
    m_sessionWidget->setFixedHeight(200);
//    m_sessionWidget->setFixedWidth(qApp->primaryScreen()->geometry().width());
    m_sessionWidget->hide();
    m_logoWidget = new LogoWidget(this);
    m_userWidget = new UserWidget(this);

    m_userWidget->setObjectName("UserWidget");
    m_passWdEdit = new PassWdEdit(this);
    m_passWdEdit->setEnterBtnStyle(":/img/action_icons/login_normal.png", ":/img/action_icons/login_hover.png", ":/img/action_icons/login_press.png");
    m_passWdEdit->setFocusPolicy(Qt::StrongFocus);
    m_passWdEdit->setFocus();

//    m_loginButton = new QPushButton(this);
//    m_loginButton->setText(tr("Login"));
//    m_loginButton->setFixedSize(160, 36);
//    m_loginButton->hide();

    m_requireShutdownWidget = new ShutdownWidget(this);
    m_requireShutdownWidget->hide();
//    m_passWdEditLayout = new QHBoxLayout;
//    m_passWdEditLayout->setMargin(0);
//    m_passWdEditLayout->setSpacing(0);
//    m_passWdEditLayout->addStretch();
//    m_passWdEditLayout->addWidget(m_passWdEdit);
//    m_passWdEditLayout->addWidget(m_loginButton);
//    m_passWdEditLayout->addStretch();

//    m_Layout = new QVBoxLayout;
//    m_Layout->setMargin(0);
//    m_Layout->setSpacing(0);
//    m_Layout->addStretch();
//    m_Layout->addLayout(m_passWdEditLayout);
//    m_Layout->addStretch();

//    QWidget *centralWidget = new QWidget;
//    centralWidget->setLayout(m_Layout);

//    setContent(centralWidget);
//    setLayout(m_Layout);
    showFullScreen();

    m_passWdEdit->updateKeyboardStatus();
    keyboardLayoutUI();

    m_controlWidget->setUserSwitchEnable(m_userWidget->count() > 1);
    m_controlWidget->setSessionSwitchEnable(m_sessionWidget->sessionCount() > 1);
#ifndef SHENWEI_PLATFORM
    updateStyle(":/skin/login.qss", this);
#endif
//    set_rootwindow_cursor();

    const auto disp = XOpenDisplay(nullptr);
    Q_ASSERT(disp);
    const auto window = DefaultRootWindow(disp);

    Cursor invisibleCursor;
    Pixmap bitmapNoData;
    XColor black;
    static char noData[] = { 0,0,0,0,0,0,0,0 };
    black.red = black.green = black.blue = 0;

    bitmapNoData = XCreateBitmapFromData(disp, window, noData, 8, 8);
    invisibleCursor = XCreatePixmapCursor(disp, bitmapNoData, bitmapNoData,
                                          &black, &black, 0, 0);
    XDefineCursor(disp, window, invisibleCursor);
    XFixesChangeCursorByName(disp, invisibleCursor, "watch");
    XFreeCursor(disp, invisibleCursor);
    XFreePixmap(disp, bitmapNoData);
    XFlush(disp);
}

//void LoginManager::recordPid() {
//    qDebug() << "remember P i D" << qApp->applicationPid();

//    QFile tmpPidFile(QString("%1%2").arg("/tmp/").arg(".dgreeterpid"));

//    if (tmpPidFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
//        QTextStream pidInfo(&tmpPidFile);
//        pidInfo << qApp->applicationPid();

//        tmpPidFile.close();
//    } else {
//        qDebug() << "file open failed!";
//    }
//}

void LoginManager::initData() {
    m_greeter = new QLightDM::Greeter(this);
    if (!m_greeter->connectSync())
        qWarning() << "greeter connect fail !!!";
}

void LoginManager::initConnect()
{
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, this, &LoginManager::chooseUserMode);
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);
    connect(m_controlWidget, &ControlWidget::requestShutdown, this, &LoginManager::showShutdownFrame);
    connect(m_controlWidget, &ControlWidget::requestSwitchSession, this, &LoginManager::chooseSessionMode);

    connect(m_passWdEdit, &PassWdEdit::submit, this, &LoginManager::login);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, this, &LoginManager::choosedSession);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, m_controlWidget, &ControlWidget::chooseToSession, Qt::QueuedConnection);

    connect(m_userWidget, &UserWidget::userChanged, [&](const QString username) {

        qDebug()<<"selected user: " << username;
        qDebug()<<"previous selected user: " << m_sessionWidget->currentSessionOwner();
        m_passWdEdit->show();
        m_passWdEdit->setFocus();

        qDebug() << username << m_sessionWidget->currentSessionOwner();

        // goto previous lock
        if (m_userWidget->getLoggedInUsers().contains(username))
        {
            QProcess *process = new QProcess;
            connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);
            process->start("dde-switchtogreeter " + username);

            return;
        }

        m_sessionWidget->switchToUser(username);
        m_greeter->authenticate(username);

        updateBackground(username);
        updateUserLoginCondition(username);
    });

    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LoginManager::prompt);
    connect(m_greeter, &QLightDM::Greeter::showMessage, this, &LoginManager::message);
    connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LoginManager::authenticationComplete);

    connect(m_passWdEdit, &PassWdEdit::updateKeyboardStatus, this, &LoginManager::keyboardLayoutUI);
    connect(m_passWdEdit, &PassWdEdit::keybdLayoutButtonClicked, this, &LoginManager::keybdLayoutWidgetPosit);
    connect(m_passWdEdit, &PassWdEdit::leftKeyPressed, this, &LoginManager::leftKeyPressed);
    connect(m_passWdEdit, &PassWdEdit::rightKeyPressed, this, &LoginManager::rightKeyPressed);

    connect(m_passWdEdit, &PassWdEdit::focusIn, [this]{
        if (!m_keybdArrowWidget->isHidden()) {
           m_keybdArrowWidget->hide();
        }});

    connect(m_requireShutdownWidget, &ShutdownWidget::shutDownWidgetAction, this, &LoginManager::setShutdownAction);

//    connect(m_loginButton, &QPushButton::clicked, this, &LoginManager::login);

    connect(m_keyboardMonitor, &KeyboardMonitor::numlockStatusChanged, this, &LoginManager::saveNumlockStatus);
}

void LoginManager::initDateAndUpdate() {
    /*Update the m_passWdEdit UI after get the current user
    *If the current user owns multi-keyboardLayouts,
    *then the button to choose keyboardLayout need show
    */

    //Get the session of current user
    m_sessionWidget->switchToUser(m_userWidget->currentUser());
    //To control the expanding the widgets of all the user list(m_userWidget)
    expandUserWidget();

    //The dbus is used to control the power actions
    m_login1ManagerInterface =new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    if (!m_login1ManagerInterface->isValid()) {
        qDebug() <<"m_login1ManagerInterface:" << m_login1ManagerInterface->lastError().type();
    }
}

void LoginManager::expandUserWidget() {
//    int expandState = m_utilFile->getExpandState();
//    qDebug() << "expandState:" << expandState;
//    if (expandState == 1) {
//        qDebug() << "expandState:" << expandState;
//        QMetaObject::invokeMethod(m_controlWidget, "requestSwitchUser", Qt::QueuedConnection);
//    }

//    m_utilFile->setExpandState(0);
}

void LoginManager::message(QString text, QLightDM::Greeter::MessageType type)
{
    switch (type) {
    case QLightDM::Greeter::MessageTypeInfo:
        m_passWdEdit->setMessage(text);
        break;
    case QLightDM::Greeter::MessageTypeError:
        qWarning() << "error message from lightdm: " << text;
        break;
    default:
        break;
    }
}

void LoginManager::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "pam prompt: " << text << type;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = text == "Password: " ? "" : text;

    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:
        if (msg.isEmpty() && !m_passWdEdit->getText().isEmpty())
            m_greeter->respond(m_passWdEdit->getText());

        if (!msg.isEmpty())
            m_passWdEdit->setMessage(msg);
        break;
    case QLightDM::Greeter::PromptTypeQuestion:
        // trim the right : in the message if exists.
        m_passWdEdit->setMessage(text.replace(":", ""));
        break;
    default:
        break;
    }
}

void LoginManager::authenticationComplete()
{
    qDebug() << "authentication complete, authenticated " << m_greeter->isAuthenticated();

    m_userWidget->hideLoadingAni();
    if (!m_greeter->isAuthenticated() && m_passWdEdit->isVisible()) {
        m_passWdEdit->setAlert(true, tr("Wrong Password"));
        return;
    }

    QTimer::singleShot(100, this, SLOT(startSession()));
}

void LoginManager::login()
{
    if(!m_requireShutdownWidget->isHidden()) {
        qDebug() << "SHUTDOWN";
        m_requireShutdownWidget->shutdownAction();
        return;
    }

    if (!m_sessionWidget->isHidden()) {
        qDebug() << "SESSIONWIDGET";
//        m_sessionWidget->chooseSession();
        return;
    }
    if (m_userWidget->isChooseUserMode && !m_userWidget->isHidden()) {
        m_userWidget->chooseButtonChecked();
        m_passWdEdit->getFocusTimer->start();
        qDebug() << "lineEditGrabKeyboard";
        return;
    }

    if (!m_passWdEdit->isVisible())
        return;

    m_userWidget->showLoadingAni();

    if (m_greeter->isAuthenticated())
        startSession();
    else if (m_greeter->inAuthentication())
        m_greeter->respond(m_passWdEdit->getText());
    else
        authenticate();
}

void LoginManager::authenticate()
{
    const QString &username = m_userWidget->currentUser();

    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

    m_greeter->authenticate(username);

    qDebug() << "start authentication of user: " << m_greeter->authenticationUser();
}

void LoginManager::chooseUserMode()
{
    m_passWdEdit->hide();
//    m_loginButton->hide();
    m_sessionWidget->hide();
    m_userWidget->show();
    m_requireShutdownWidget->hide();
}

void LoginManager::chooseSessionMode()
{
    m_sessionWidget->show();
    m_userWidget->hide();
    m_passWdEdit->hide();
//    m_loginButton->hide();
    m_requireShutdownWidget->hide();
    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::choosedSession() {
    m_requireShutdownWidget->hide();
    m_sessionWidget->hide();
    m_userWidget->show();
    if (m_userWidget->isChooseUserMode) {
        m_passWdEdit->hide();
    } else {
        if (m_passWdEdit->size() != ZoreSize) {
            m_passWdEdit->show();
            m_passWdEdit->setFocus();
        } else {
//            m_loginButton->show();
        }
    }
    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::showShutdownFrame() {
    qDebug() << "showShutdownFrame!";
    m_userWidget->hide();
    m_passWdEdit->hide();
//    m_loginButton->hide();
    m_sessionWidget->hide();
    m_requireShutdownWidget->show();
}

void LoginManager::keyboardLayoutUI() {
    m_passWdEdit->updateKeybdLayoutUI(m_userWidget->getUserKBHistory(m_userWidget->currentUser()));
    m_keybdLayoutWidget = new KbLayoutWidget;
    m_keybdLayoutWidget->updateButtonList(m_userWidget->getUserKBHistory(m_userWidget->currentUser()));
    m_keybdLayoutWidget->setDefault(m_userWidget->getUserKBLayout(m_userWidget->currentUser()));

    m_keybdArrowWidget = new DArrowRectangle(DArrowRectangle::ArrowTop, this);
    m_keybdArrowWidget->setBackgroundColor(QColor(0, 0, 0, 78));
    m_keybdArrowWidget->setBorderColor(QColor(0, 0, 0, 100));
    m_keybdArrowWidget->setArrowX(13);
    m_keybdArrowWidget->setArrowWidth(12);
    m_keybdArrowWidget->setArrowHeight(6);
    m_keybdArrowWidget->setMargin(1);

    m_keybdArrowWidget->setContent(m_keybdLayoutWidget);
    m_keybdLayoutWidget->setParent(m_keybdArrowWidget);
    m_keybdLayoutWidget->show();
    m_keybdArrowWidget->move(m_passWdEdit->x() + 123, m_passWdEdit->y() + m_passWdEdit->height() - 15);

    m_keybdArrowWidget->hide();

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LoginManager::setCurrentKeybdLayoutList);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, m_keybdArrowWidget, &DArrowRectangle::hide);
    connect(m_userWidget, &UserWidget::chooseUserModeChanged, m_passWdEdit, &PassWdEdit::recordUserPassWd);
}

void LoginManager::setCurrentKeybdLayoutList(QString keyboard_value)
{
    m_userWidget->setUserKBlayout(m_userWidget->currentUser(), keyboard_value);
}

void LoginManager::keybdLayoutWidgetPosit() {
    if (m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->show(m_passWdEdit->x() + 123, m_passWdEdit->y() + m_passWdEdit->height() - 15);
    } else {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::setShutdownAction(const ShutdownWidget::Actions action) {

    switch (action) {
        case ShutdownWidget::RequireShutdown: { m_login1ManagerInterface->PowerOff(true); break;}
        case ShutdownWidget::RequireRestart: { m_login1ManagerInterface->Reboot(true); break;}
        case ShutdownWidget::RequireSuspend: { m_login1ManagerInterface->Suspend(true);

            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (m_userWidget->isChooseUserMode) {
                m_passWdEdit->hide();
            } else {
                if (m_passWdEdit->size() != ZoreSize) {
                    m_passWdEdit->show();
                    m_passWdEdit->setFocus();
                } else {
//                    m_loginButton->show();
                }
            }
            m_sessionWidget->hide();
        break;}
        default:;
    }
}

void LoginManager::leftKeyPressed() {
    if (!m_userWidget->isHidden() && m_passWdEdit->isHidden()) {
        m_userWidget->leftKeySwitchUser();
    }
//    if (!m_userWidget->isHidden() && !m_passWdEdit->isHidden() &&
//            m_passWdEdit->getText().isEmpty()) {
//        m_userWidget->leftKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->leftKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
//        m_sessionWidget->leftKeySwitch();
    }
}

void LoginManager::rightKeyPressed() {
    if (!m_userWidget->isHidden() && m_passWdEdit->isHidden()) {
        m_userWidget->rightKeySwitchUser();
    }
//   if (!m_userWidget->isHidden() && !m_passWdEdit->isHidden() &&
//            m_passWdEdit->getText().isEmpty()) {
//        m_userWidget->rightKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->rightKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
//        m_sessionWidget->rightKeySwitch();
    }
}

void LoginManager::saveNumlockStatus(const bool &on)
{
    qDebug() << "save numlock statuc to " << on;
    QSettings settings;
    settings.beginGroup("NumlockStatus");
    settings.setValue(m_userWidget->currentUser(), on);
    settings.endGroup();
}

void LoginManager::restoreNumlockStatus()
{
    bool value = false;

    QSettings settings;
    settings.beginGroup("NumlockStatus");
    value = settings.value(m_userWidget->currentUser(), m_keyboardMonitor->isNumlockOn()).toBool();
    settings.endGroup();

    qDebug() << "restore numlock status to " << value;
    m_keyboardMonitor->setNumlockStatus(value);
}
