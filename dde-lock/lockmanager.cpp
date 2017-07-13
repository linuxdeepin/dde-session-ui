/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "userwidget.h"
#include "passwdedit.h"
#include "util_updateui.h"
#include "dbus/dbusinputdevices.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QMap>
#include <QProcess>
#include <QDBusConnection>

#include "lockmanager.h"
#include "lockframe.h"
#include "dbus/dbuslockfront.h"

static const QSize ZoreSize = QSize(0, 0);

LockManager::LockManager(QWidget *parent)
    : QFrame(parent),

      m_activatedUser(UserWidget::loginUser())
{
    initUI();
    initConnect();
    initBackend();
    updateUI();
    loadMPRIS();

    leaveEvent(nullptr);
}

void LockManager::initConnect()
{

    connect(m_passwordEdit, &PassWdEdit::keybdLayoutButtonClicked, this, &LockManager::keybdLayoutWidgetPosit);
    connect(m_controlWidget, &ControlWidget::shutdownClicked, this, &LockManager::shutdownMode);
    connect(m_controlWidget, &ControlWidget::switchUser, this, &LockManager::chooseUserMode);
    connect(m_controlWidget, &ControlWidget::switchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);

    connect(m_requireShutdownWidget, &ShutdownWidget::shutDownWidgetAction, [this](const ShutdownWidget::Actions action) {
        switch (action) {
        case ShutdownWidget::RequireRestart:    m_action = Restart;         break;
        case ShutdownWidget::RequireShutdown:   m_action = Shutdown;        break;
        case ShutdownWidget::RequireSuspend:    m_action = Suspend;         break;
        }

        passwordMode();
    });
    connect(qApp, &QApplication::aboutToQuit, [this] {
        m_hotZoneInterface->EnableZoneDetected(true);
    });

    connect(m_passwordEdit, &PassWdEdit::leftKeyPressed, this, &LockManager::leftKeyPressed);
    connect(m_passwordEdit, &PassWdEdit::rightKeyPressed, this, &LockManager::rightKeyPressed);

    connect(m_unlockButton, &QPushButton::clicked, this, &LockManager::unlock);
}

void LockManager::keybdLayoutWidgetPosit()
{
    m_keybdArrowWidget->show(m_passwordEdit->x() + 123, m_passwordEdit->y() + m_passwordEdit->height() - 15);
    m_keybdLayoutWidget->show();
}

void LockManager::leftKeyPressed()
{
    if (!m_userWidget->isHidden() && m_passwordEdit->isHidden()) {
        m_userWidget->leftKeySwitchUser();
    }
//    if (!m_userWidget->isHidden() && !m_passwordEdit->isHidden() &&
//            m_passwordEdit->getText().isEmpty()) {
//        m_userWidget->leftKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->leftKeySwitch();
    }
}

void LockManager::rightKeyPressed()
{
    if (!m_userWidget->isHidden() && m_passwordEdit->isHidden()) {
        m_userWidget->rightKeySwitchUser();
    }
//    if (!m_userWidget->isHidden() && !m_passwordEdit->isHidden() &&
//            m_passwordEdit->getText().isEmpty()) {
//        m_userWidget->rightKeySwitchUser();
//    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->rightKeySwitch();
    }
}

//void LockManager::onUserUnlock(const QString &username)
//{
//    qDebug()<< "recive message---------------------------";
//    if (username == m_userWidget->currentUser()) {
//        qApp->exit();
//    }
//}

void LockManager::initUI()
{
    setFixedSize(qApp->desktop()->size());
    setFocusPolicy(Qt::NoFocus);

    m_timeWidget = new TimeWidget(this);
    m_timeWidget->setFixedSize(400, 300);

    m_userWidget = new UserWidget(this);
    m_userWidget->setFixedWidth(width());
    m_userWidget->move(0, (height() - m_userWidget->height()) / 2 - 95);

    m_passwordEdit = new PassWdEdit(this);
    m_passwordEdit->setEnterBtnStyle(":/img/action_icons/unlock_normal.png", ":/img/action_icons/unlock_hover.png", ":/img/action_icons/unlock_press.png");
    m_passwordEdit->setFocusPolicy(Qt::StrongFocus);
    m_passwordEdit->setFocus();
    m_passwdEditSize = m_passwordEdit->size();

    m_unlockButton = new QPushButton(this);
    m_unlockButton->setText(tr("Login"));
    m_unlockButton->setFixedSize(160, 36);
    m_unlockButton->hide();

    m_requireShutdownWidget = new ShutdownWidget(this);
    m_requireShutdownWidget->hide();
    m_requireShutdownWidget->setFixedWidth(width());
    m_requireShutdownWidget->setFixedHeight(300);


    m_controlWidget = new ControlWidget(this);
    m_controlWidget->setUserSwitchEnable(m_userWidget->count() > 1);

    QHBoxLayout *passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(0);
    passwdLayout->setSpacing(0);
    passwdLayout->addStretch();
    passwdLayout->addWidget(m_passwordEdit);
    passwdLayout->addWidget(m_unlockButton);
    passwdLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addLayout(passwdLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
    showFullScreen();
    activateWindow();
    updateWidgetsPosition();
    updateStyle(":/skin/lock.qss", this);

    m_lockInter = new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH,
                                      QDBusConnection::systemBus(), this);
    m_lockInter->AuthenticateUser(m_activatedUser);

    connect(m_lockInter, &DBusLockService::Event, this, &LockManager::lockServiceEvent);

    connect(m_passwordEdit, &PassWdEdit::submit, this, &LockManager::unlock);
    connect(m_userWidget, &UserWidget::userChanged, this, [=] (const QString & username) {
        m_passwordEdit->show();

        qDebug() << "current User:" << username << "11 m_activatedUser:" << m_activatedUser;

        if (username != m_activatedUser) {
            // goto greeter
            QProcess *process = new QProcess;
            process->start("dde-switchtogreeter " + username);
            process->waitForFinished();
            process->deleteLater();

            return;
        } else {
            this->updateBackground(m_activatedUser);
            this->updateUserLoginCondition(m_activatedUser);
        }
    });

    updateBackground(m_activatedUser);
    updateUserLoginCondition(m_activatedUser);
}

void LockManager::updateWidgetsPosition()
{
    qDebug() << "lockManager: positon" << this->width() << this->height();
    const int width = this->width();
    const int height = this->height();
    m_timeWidget->move(48, height - m_timeWidget->height() - 36); // left 48px and bottom 36px
    m_userWidget->setFixedWidth(width);
    if (!m_userWidget->isChooseUserMode)
        m_userWidget->move(0, (height - m_userWidget->height()) / 2 - 95);
    m_requireShutdownWidget->setFixedWidth(width);
    m_requireShutdownWidget->move(0, (height - m_requireShutdownWidget->height()) / 2 - 50);
    m_controlWidget->move(width - m_controlWidget->width(),
                          height - m_controlWidget->height() - 36); // margin right 0 margin bottom 36

}

void LockManager::chooseUserMode()
{
    m_passwordEdit->hide();
    m_unlockButton->hide();
    m_userWidget->show();
    m_requireShutdownWidget->hide();
    m_keybdLayoutWidget->hide();
    m_keybdArrowWidget->hide();
}

void LockManager::onUnlockFinished(const bool unlocked)
{
    if (!unlocked) {
        qDebug() << "Authorization failed!";

        m_lockInter->AuthenticateUser(m_activatedUser);

        m_authFailureCount++;
        m_userWidget->hideLoadingAni();
        if (m_authFailureCount < UtilFile::GetAuthLimitation()) {
            m_passwordEdit->setAlert(true, tr("Wrong Password"));
        } else {
            m_authFailureCount = 0;
            m_passwordEdit->setReadOnly(true);
            m_passwordEdit->setEnabled(false);
            m_passwordEdit->setAlert(true, tr("Please retry after 10 minutes"));
        }

        return;
    }

    // Auth success
    switch (m_action) {
    case Restart:       m_sessionManagerIter->RequestReboot();    break;
    case Shutdown:      m_sessionManagerIter->RequestShutdown();    break;
    case Suspend:       m_sessionManagerIter->RequestSuspend();     break;
    default: break;
    }

#ifdef LOCK_NO_QUIT
    m_userWidget->hideLoadingAni();
    m_passwordEdit->clearText();
    emit checkedHide();
#else
    qApp->exit();
#endif
}

void LockManager::updateBackground(QString username)
{
    const QSettings settings("/var/lib/AccountsService/users/" + username, QSettings::IniFormat);
    const QString background = settings.value("User/GreeterBackground").toString();

    LockFrame *frame = qobject_cast<LockFrame *>(parent());
    frame->setBackground(background);
}

void LockManager::updateUserLoginCondition(QString username)
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
            m_passwordEdit->setFixedSize(ZoreSize);
            m_unlockButton->show();
            return;
        }
    }

    m_passwordEdit->setFixedSize(m_passwdEditSize);
    m_unlockButton->hide();
}

void LockManager::leaveEvent(QEvent *)
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList) {
        if (screen->geometry().contains(mousePoint)) {
            const QRect &geometry = screen->geometry();
            setFixedSize(geometry.size());
            emit screenChanged(geometry);
            return;
        }
    }
}

void LockManager::showEvent(QShowEvent *event)
{
    m_keybdLayoutWidget->hide();
    m_keybdArrowWidget->hide();

    m_controlWidget->setUserSwitchEnable(m_userWidget->count() > 1);
    updateBackground(m_activatedUser);

    m_keybdInfoMap.clear();
    m_keybdLayoutNameList.clear();
    keybdLayoutDescList.clear();

    m_keybdLayoutNameList = m_keyboardLayoutInterface->userLayoutList();
    QString currentKeybdLayout = m_keyboardLayoutInterface->currentLayout();

    for (int i = 0; i < m_keybdLayoutNameList.length(); i++) {
        if (m_keybdLayoutNameList[i] == currentKeybdLayout) {
            m_keybdLayoutItemIndex = i;
        }
        QDBusPendingReply<QString> tmpValue =  m_keyboardLayoutInterface->GetLayoutDesc(m_keybdLayoutNameList[i]);
        tmpValue.waitForFinished();

        keybdLayoutDescList << tmpValue;
        m_keybdInfoMap.insert(m_keybdLayoutNameList[i], tmpValue);
    }
    qDebug() << "QStringList" << m_keybdLayoutNameList;
    m_passwordEdit->updateKeybdLayoutUI(keybdLayoutDescList);
    m_keybdLayoutWidget->updateButtonList(keybdLayoutDescList);
    m_keybdLayoutWidget->setListItemChecked(m_keybdLayoutItemIndex);

    loadMPRIS();

    QFrame::showEvent(event);
}

void LockManager::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            if (!m_userWidget->isChooseUserMode) {
                if (m_passwordEdit->size() != ZoreSize) {
                    m_passwordEdit->show();
                    m_passwordEdit->setFocus();
                } else {
                    m_unlockButton->show();
                }
            }
        }
#ifdef QT_DEBUG
//        qApp->quit();   break;
#endif
    default:;
    }
}

void LockManager::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "ReleaseEvent";
    m_action = Unlock;
    if (!m_userWidget->isChooseUserMode) {
        passwordMode();
    }
    if (e->button() == Qt::LeftButton) {
        if (!m_keybdArrowWidget->isHidden()) {
            m_keybdArrowWidget->hide();
        }
    }
}

void LockManager::unlock()
{
    m_keybdLayoutWidget->hide();
    m_keybdArrowWidget->hide();

    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->shutdownAction();
        return;
    }

    if (m_userWidget->isChooseUserMode && !m_userWidget->isHidden()) {
        m_userWidget->chooseButtonChecked();
        m_passwordEdit->getFocusTimer->start();
        qDebug() << "lineEditGrabKeyboard";
        return;
    }

    m_userWidget->showLoadingAni();

    if (!m_passwordEdit->isVisible())
        return;

    if (m_authenticating)
        return;

    m_authenticating = true;

//    qDebug() << "unlock" << m_userWidget->currentUser() << m_passwordEdit->getText();
    const QString &username = m_activatedUser;
    const QString &password = m_passwordEdit->getText();

    m_lockInter->UnlockCheck(username, password);
}

void LockManager::loadMPRIS()
{
    if (m_mprisInter) {
        m_mprisInter->deleteLater();
    }

    QDBusInterface *dbusInter = new QDBusInterface("org.freedesktop.DBus", "/", "org.freedesktop.DBus", QDBusConnection::sessionBus(), this);
    if (!dbusInter) {
        return;
    }

    QDBusReply<QStringList> response = dbusInter->call("ListNames");
    const QStringList &serviceList = response.value();
    QString service = QString();
    for (const QString &serv : serviceList) {
        if (!serv.startsWith("org.mpris.MediaPlayer2.")) {
            continue;
        }
        service = serv;
        break;
    }

    if (service.isEmpty()) {
        return;
    }

    dbusInter->deleteLater();

    qDebug() << "got service: " << service;

    m_mprisInter = new DBusMediaPlayer2(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
    m_controlWidget->bindDBusService(m_mprisInter);
}

void LockManager::lockServiceEvent(quint32 eventType, quint32 pid, const QString &username, const QString &message)
{
    if (username != m_activatedUser)
        return;

    qDebug() << eventType << pid << username << message;

    // Don't show password prompt from standard pam modules since
    // we'll provide our own prompt or just not.
    const QString msg = message == "Password: " ? "" : message;

    m_authenticating = false;

    switch (eventType) {
    case DBusLockService::PromptQuestion:
        qDebug() << "prompt quesiton from pam: " << message;
        m_passwordEdit->setMessage(message);
        break;
    case DBusLockService::PromptSecret:
        qDebug() << "prompt secret from pam: " << message;
        if (!msg.isEmpty())
            m_passwordEdit->setMessage(msg);
        break;
    case DBusLockService::ErrorMsg:
        qWarning() << "error message from pam: " << message;
        break;
    case DBusLockService::TextInfo:
        m_passwordEdit->setMessage(message);
        break;
    case DBusLockService::Failure:
        onUnlockFinished(false);
        break;
    case DBusLockService::Successed:
        onUnlockFinished(true);
        break;
    default:
        break;
    }
}

void LockManager::initBackend()
{
    m_hotZoneInterface = new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone",
                                         QDBusConnection::sessionBus(), this);
#ifndef LOCK_NO_QUIT
    m_hotZoneInterface->EnableZoneDetected(false);
#endif


    DBusInputDevices *dbusInputDevices = new DBusInputDevices(this);
    foreach(InputDevice device, dbusInputDevices->infos()) {
        if (device.deviceType == "keyboard") {
            m_keyboardLayoutInterface = new DBusKeyboard(this);
            break;
        }
    }

    m_keybdLayoutNameList = m_keyboardLayoutInterface->userLayoutList();
    QString currentKeybdLayout = m_keyboardLayoutInterface->currentLayout();

    for (int i = 0; i < m_keybdLayoutNameList.length(); i++) {
        if (m_keybdLayoutNameList[i] == currentKeybdLayout) {
            m_keybdLayoutItemIndex = i;
        }
        QDBusPendingReply<QString> tmpValue =  m_keyboardLayoutInterface->GetLayoutDesc(m_keybdLayoutNameList[i]);
        tmpValue.waitForFinished();

        keybdLayoutDescList << tmpValue;
        m_keybdInfoMap.insert(m_keybdLayoutNameList[i], tmpValue);
    }
    qDebug() << "QStringList" << m_keybdLayoutNameList;
    m_passwordEdit->updateKeybdLayoutUI(keybdLayoutDescList);

    m_sessionManagerIter = new DBusSessionManagerInterface("com.deepin.SessionManager", "/com/deepin/SessionManager",
            QDBusConnection::sessionBus(), this);
}

void LockManager::updateUI()
{
    m_keybdLayoutWidget = new KbLayoutWidget(keybdLayoutDescList);

    m_keybdLayoutWidget->setListItemChecked(m_keybdLayoutItemIndex);
    m_keybdArrowWidget = new DArrowRectangle(DArrowRectangle::ArrowTop, this);

    m_keybdArrowWidget->setBackgroundColor(QColor::fromRgbF(1, 1, 1, 0.15));
    m_keybdArrowWidget->setBorderColor(Qt::transparent);
    m_keybdArrowWidget->setArrowX(17);
    m_keybdArrowWidget->setArrowWidth(12);
    m_keybdArrowWidget->setArrowHeight(6);
    m_keybdArrowWidget->setMargin(1);

    m_keybdArrowWidget->setContent(m_keybdLayoutWidget);
    m_keybdLayoutWidget->setParent(m_keybdArrowWidget);
    m_keybdLayoutWidget->show();
    m_keybdArrowWidget->move(m_passwordEdit->x() + 123, m_passwordEdit->y() + m_passwordEdit->height() - 15);
    m_keybdArrowWidget->hide();

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LockManager::setCurrentKeyboardLayout);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, m_keybdArrowWidget, &DArrowRectangle::hide);
}

void LockManager::enableZone()
{
    m_hotZoneInterface->EnableZoneDetected(true);
}

void LockManager::disableZone()
{
    m_hotZoneInterface->EnableZoneDetected(false);
}

ControlWidget *LockManager::control()
{
    return m_controlWidget;
}

void LockManager::setCurrentKeyboardLayout(QString keyboard_value)
{
    QMap<QString, QString>::iterator lookupIt;

    for (lookupIt = m_keybdInfoMap.begin(); lookupIt != m_keybdInfoMap.end(); ++lookupIt) {
        if (lookupIt.value() == keyboard_value && m_keyboardLayoutInterface->currentLayout() != lookupIt.key()) {

            m_keyboardLayoutInterface->setCurrentLayout(lookupIt.key());
        }
    }

}

void LockManager::passwordMode()
{
//    m_userWidget->setCurrentUser(m_activatedUser);
    m_userWidget->show();
    m_requireShutdownWidget->hide();

    if (m_passwordEdit->size() != ZoreSize) {
        m_passwordEdit->show();
        m_passwordEdit->setFocus();
    } else {
        m_unlockButton->show();
    }

    if (m_action == Suspend) {
        m_sessionManagerIter->RequestSuspend().waitForFinished();
        m_action = Unlock;
        return;
    }

    if (m_action == Restart) {
        m_passwordEdit->setAlert(true, tr("Enter your password to restart"));
        m_passwordEdit->setEnterBtnStyle(":/img/action_icons/reboot_normal.png", ":/img/action_icons/reboot_hover.png", ":/img/action_icons/reboot_press.png");
    } else if (m_action == Shutdown) {
        m_passwordEdit->setAlert(true, tr("Enter your password to shutdown"));
        m_passwordEdit->setEnterBtnStyle(":/img/action_icons/shutdown_normal.png", ":/img/action_icons/shutdown_hover.png", ":/img/action_icons/shutdown_press.png");
    } else if (m_action == Unlock) {
        m_passwordEdit->setEnterBtnStyle(":/img/action_icons/unlock_normal.png", ":/img/action_icons/unlock_hover.png", ":/img/action_icons/unlock_press.png");
    }
}

void LockManager::shutdownMode()
{
    m_userWidget->hide();
    m_passwordEdit->hide();
	m_unlockButton->hide();
    m_requireShutdownWidget->show();
}
