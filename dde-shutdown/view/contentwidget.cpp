/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QObject>

#include <libintl.h>

#include "dbus/dbusvariant.h"
#include "contentwidget.h"
#include "multiuserswarningview.h"
#include "inhibitwarnview.h"

ContentWidget::ContentWidget(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initData();
    initConnect();
}

ContentWidget::~ContentWidget()
{
    m_userWidget->deleteLater();
}

void ContentWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        onCancel();
}

void ContentWidget::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.Launcher "
                            "/com/deepin/dde/Launcher "
                            "com.deepin.dde.Launcher.Hide");

    if (m_hotZoneInterface->isValid())
        m_hotZoneInterface->EnableZoneDetected(false);

    checkUsers();

    QTimer::singleShot(1, this, [=] {
        grabKeyboard();
    });
}

void ContentWidget::keyReleaseEvent(QKeyEvent *event)
{
    setFocus();

    switch (event->key()) {
    case Qt::Key_Escape: onCancel(); break;
    case Qt::Key_Return:
    case Qt::Key_Enter: enterKeyPushed(); break;
    case Qt::Key_Left: {
        if (m_systemMonitor)
            m_systemMonitor->setState(SystemMonitor::Leave);
        setPreviousChildFocus();
        break;
    }
    case Qt::Key_Right: {
        if (m_systemMonitor)
            m_systemMonitor->setState(SystemMonitor::Leave);
        setNextChildFocus();
        break;
    }
    case Qt::Key_Up: {
        if (m_systemMonitor) {
            m_systemMonitor->setState(SystemMonitor::Leave);
        }
        m_currentSelectedBtn->setChecked(true);
        break;
    }
    case Qt::Key_Down: {
        if (m_systemMonitor) {
            m_currentSelectedBtn->updateState(RoundItemButton::Normal);
            m_systemMonitor->setFocus();
            m_systemMonitor->setState(SystemMonitor::Enter);
        }
        break;
    }
    default:;
    }
}

void ContentWidget::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    if (m_systemMonitor) {
        const QRect &re = rect();
        m_systemMonitor->move((re.width() - m_systemMonitor->width()) / 2,
                              re.height() - m_systemMonitor->height() - 40);
    }
}

void ContentWidget::hideEvent(QHideEvent *event)
{
    QFrame::hideEvent(event);

    if (m_hotZoneInterface->isValid())
        m_hotZoneInterface->EnableZoneDetected(true);

    releaseKeyboard();
}

void ContentWidget::setConfirm(const bool confirm)
{
    m_confirm = confirm;
}

void ContentWidget::powerAction(const Actions action)
{
    switch (action)
    {
    case Shutdown:
    case Restart:
    case Logout:
        beforeInvokeAction(action);
        break;
    default:
        shutDownFrameActions(action);
    }
}

void ContentWidget::initConnect() {
    connect(m_shutdownButton, &RoundItemButton::clicked, [this] { beforeInvokeAction(Shutdown);});
    connect(m_restartButton, &RoundItemButton::clicked, [this] { beforeInvokeAction(Restart);});
    connect(m_suspendButton, &RoundItemButton::clicked, [this] { shutDownFrameActions(Suspend);});
    connect(m_lockButton, &RoundItemButton::clicked, [this] { shutDownFrameActions(Lock);});
    connect(m_logoutButton, &RoundItemButton::clicked, [this] {emit beforeInvokeAction(Logout);});
    connect(m_switchUserBtn, &RoundItemButton::clicked, [this] { shutDownFrameActions(SwitchUser);});

    connect(qApp, &QApplication::aboutToQuit, [this]{
        m_hotZoneInterface->EnableZoneDetected(true);
    });

    if (m_systemMonitor)
        connect(m_systemMonitor, &SystemMonitor::clicked, this, &ContentWidget::runSystemMonitor);
}

void ContentWidget::initData()
{
    m_sessionInterface = new DBusSessionManagerInterface("com.deepin.SessionManager", "/com/deepin/SessionManager",
            QDBusConnection::sessionBus(), this);
    m_hotZoneInterface = new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone",
                                         QDBusConnection::sessionBus(), this);
}

void ContentWidget::enterKeyPushed()
{
    if (m_warningView && m_warningView->isVisible()) return;

    if (m_currentSelectedBtn->isDisabled())
        return;

    if (m_currentSelectedBtn == m_shutdownButton)
        beforeInvokeAction(Shutdown);
    else if (m_currentSelectedBtn == m_restartButton)
        beforeInvokeAction(Restart);
    else if (m_currentSelectedBtn == m_suspendButton)
         shutDownFrameActions(Suspend);
    else if (m_currentSelectedBtn == m_lockButton)
         shutDownFrameActions(Lock);
    else if (m_currentSelectedBtn == m_logoutButton)
        emit beforeInvokeAction(Logout);
    else if (m_currentSelectedBtn == m_switchUserBtn)
         shutDownFrameActions(SwitchUser);
}

void ContentWidget::hideBtn(const QString &btnName)
{
    if (!btnName.compare("Shutdown", Qt::CaseInsensitive))
        m_shutdownButton->hide();
    else if (!btnName.compare("Restart", Qt::CaseInsensitive))
        m_restartButton->hide();
    else if (!btnName.compare("Suspend", Qt::CaseInsensitive))
        m_suspendButton->hide();
    else if (!btnName.compare("Lock", Qt::CaseInsensitive))
        m_lockButton->hide();
    else if (!btnName.compare("Logout", Qt::CaseInsensitive))
        m_logoutButton->hide();
    else if (!btnName.compare("SwitchUser", Qt::CaseInsensitive))
        m_switchUserBtn->hide();
    else
        return;
}

void ContentWidget::disableBtn(const QString &btnName)
{
    if (!btnName.compare("Shutdown", Qt::CaseInsensitive))
        m_shutdownButton->setDisabled(true);
    else if (!btnName.compare("Restart", Qt::CaseInsensitive))
        m_restartButton->setDisabled(true);
    else if (!btnName.compare("Suspend", Qt::CaseInsensitive))
        m_suspendButton->setDisabled(true);
    else if (!btnName.compare("Lock", Qt::CaseInsensitive))
        m_lockButton->setDisabled(true);
    else if (!btnName.compare("Logout", Qt::CaseInsensitive))
        m_logoutButton->setDisabled(true);
    else if (!btnName.compare("SwitchUser", Qt::CaseInsensitive))
        m_switchUserBtn->setDisabled(true);
    else
        return;
}

void ContentWidget::beforeInvokeAction(const Actions action)
{
    const QString inhibitReason = getInhibitReason();
    QStringList loggedInUsers = m_userWidget->getLoggedInUsers();

    // change ui
    if (m_confirm || !inhibitReason.isEmpty() || loggedInUsers.length() > 1)
    {
        for (RoundItemButton* btn : *m_btnsList) {
            btn->hide();
        }

        if (m_warningView != nullptr) {
            m_mainLayout->removeWidget(m_warningView);
            m_warningView->deleteLater();
            m_warningView = nullptr;
        }
    }


    if (!inhibitReason.isEmpty() && action != Logout)
    {
        InhibitWarnView *view = new InhibitWarnView;
        view->setAction(action);
        view->setInhibitReason(inhibitReason);
        view->setAcceptVisible(false);
        if (action == Shutdown)
            view->setAcceptReason(tr("Shut down"));
        else if (action == Restart)
            view->setAcceptReason(tr("Reboot"));
        else
            Q_UNREACHABLE();

        m_warningView = view;
        m_mainLayout->addWidget(m_warningView, 0, Qt::AlignCenter);

        connect(view, &InhibitWarnView::cancelled, this, &ContentWidget::onCancel);
        connect(view, &InhibitWarnView::actionInvoked, [this, action] {
             shutDownFrameActions(action);
        });

        m_warningView->show();
        m_warningView->raise();

        return;
    }

    if (loggedInUsers.length() > 1 && action != Logout) {

        MultiUsersWarningView *view = new MultiUsersWarningView(m_userWidget);
        view->setUsers(loggedInUsers);
        view->setAction(action);
        m_warningView = view;
        m_mainLayout->addWidget(m_warningView, 0, Qt::AlignCenter);

        connect(view, &MultiUsersWarningView::cancelled, this, &ContentWidget::onCancel);
        connect(view, &MultiUsersWarningView::actionInvoked, [this, action] {
             shutDownFrameActions(action);
        });

        m_warningView->show();

        return;
    }

    if (m_confirm)
    {
        m_confirm = false;

        InhibitWarnView *view = new InhibitWarnView;
        view->setAction(action);
        if (action == Shutdown)
        {
            view->setAcceptReason(tr("Shut down"));
            view->setInhibitReason(tr("Are you sure to shut down?"));
        }
        else if (action == Restart)
        {
            view->setAcceptReason(tr("Reboot"));
            view->setInhibitReason(tr("Are you sure to reboot?"));
        }
        else if (action == Logout)
        {
            view->setAcceptReason(tr("Log out"));
            view->setInhibitReason(tr("Are you sure to log out?"));
        }

        m_warningView = view;

        m_mainLayout->addWidget(m_warningView, 0, Qt::AlignCenter);

        connect(view, &InhibitWarnView::cancelled, this, &ContentWidget::onCancel);
        connect(view, &InhibitWarnView::actionInvoked, [this, action] {
             shutDownFrameActions(action);
        });

        m_warningView->show();
        m_warningView->raise();

        return;
    }

     shutDownFrameActions(action);
}

void ContentWidget::hideToplevelWindow()
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

void ContentWidget::checkUsers()
{
    UserWidget users;
    if (users.count() < 2) {
        hideBtns(QStringList() << "SwitchUser");
    }
}

void ContentWidget::shutDownFrameActions(const Actions action)
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

void ContentWidget::initUI() {
    m_btnsList = new QList<RoundItemButton *>;
    m_shutdownButton = new RoundItemButton(tr("Shut down"));
    m_shutdownButton->setAutoExclusive(true);
    m_shutdownButton->setObjectName("ShutDownButton");
    m_restartButton = new RoundItemButton(tr("Reboot"));
    m_restartButton->setAutoExclusive(true);
    m_restartButton->setObjectName("RestartButton");
    m_suspendButton = new RoundItemButton(tr("Suspend"));
    m_suspendButton->setAutoExclusive(true);
    m_suspendButton->setObjectName("SuspendButton");
    m_lockButton = new RoundItemButton(tr("Lock"));
    m_lockButton->setAutoExclusive(true);
    m_lockButton->setObjectName("LockButton");
    m_logoutButton = new RoundItemButton(tr("Log out"));
    m_logoutButton->setAutoExclusive(true);
    m_logoutButton->setObjectName("LogoutButton");

    m_switchUserBtn = new RoundItemButton(tr("Switch user"));
    m_switchUserBtn->setAutoExclusive(true);
    m_switchUserBtn->setObjectName("SwitchUserButton");

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_shutdownButton);
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addWidget(m_suspendButton);
    buttonLayout->addWidget(m_lockButton);
    buttonLayout->addWidget(m_switchUserBtn);
    buttonLayout->addWidget(m_logoutButton);
    buttonLayout->addStretch(0);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addLayout(buttonLayout);

    QFile file("/usr/bin/deepin-system-monitor");
    if (file.exists())
        m_systemMonitor = new SystemMonitor(this);
    else
        m_systemMonitor = nullptr;

    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_mainLayout);

    updateStyle(":/skin/shutdown.qss", this);

    m_userWidget = new UserWidget;
    m_userWidget->hide();

    m_btnsList->append(m_shutdownButton);
    m_btnsList->append(m_restartButton);
    m_btnsList->append(m_suspendButton);
    m_btnsList->append(m_lockButton);
    m_btnsList->append(m_switchUserBtn);
    m_btnsList->append(m_logoutButton);

    m_currentSelectedBtn = m_lockButton;
    m_currentSelectedBtn->updateState(RoundItemButton::Default);

    //// Inhibit to shutdown
    getInhibitReason();

    QTimer* checkTooltip = new QTimer(this);
    checkTooltip->setInterval(10000);
    checkTooltip->setSingleShot(false);
    connect(checkTooltip,  &QTimer::timeout, [this] {
        InhibitWarnView *view = qobject_cast<InhibitWarnView *>(m_warningView);
        if (!view)
            return;
        if (getInhibitReason().isEmpty())
            view->setAcceptVisible(true);
    });
    checkTooltip->start();
}

const QString ContentWidget::getInhibitReason() {
    m_login1Inter = new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    QString reminder_tooltip  = QString();

    if (m_login1Inter->isValid()) {
        qDebug() <<  "m_login1Inter is valid!";

        QDBusPendingReply<InhibitorsList> reply = m_login1Inter->ListInhibitors();
        reply.waitForFinished();

        if (!reply.isError()){
            InhibitorsList inhibitList = qdbus_cast<InhibitorsList>(reply.argumentAt(0));
            qDebug() << "inhibitList:" << inhibitList.count();

            for(int i = 0; i < inhibitList.count();i++) {
                // Just take care of DStore's inhibition, ignore others'.
                if (inhibitList.at(i).what == "shutdown"
                        && inhibitList.at(i).dosome == "block"
                        && inhibitList.at(i).who == "Deepin Store")
                {
                    setlocale(LC_ALL, "");
                    reminder_tooltip = dgettext("lastore-daemon", inhibitList.at(i).why.toUtf8());
                    break;
                }
            }
            qDebug() << "shutdown Reason!" << reminder_tooltip;
//            showTips(reminder_tooltip);
            return reminder_tooltip;
        } else {
            reminder_tooltip = "";
            qDebug() << reply.error().message();
        }
    } else {
        qDebug() << "shutdown login1Manager error!";
        reminder_tooltip = "";
    }

    return reminder_tooltip;
}

void ContentWidget::recoveryLayout()
{
    for (RoundItemButton* btn : *m_btnsList) {
        btn->show();
    }

    if (m_warningView != nullptr) {
        m_mainLayout->removeWidget(m_warningView);
        m_warningView->deleteLater();
        m_warningView = nullptr;
    }
}

void ContentWidget::runSystemMonitor()
{
    QProcess::startDetached("/usr/bin/deepin-system-monitor");


#ifdef SHUTDOWN_NO_QUIT
    if (m_systemMonitor) {
        m_systemMonitor->clearFocus();
        m_systemMonitor->setState(SystemMonitor::Leave);
    }
    hideToplevelWindow();
    recoveryLayout();
#else
    qApp->quit();
#endif
}

void ContentWidget::setPreviousChildFocus()
{
    if (m_warningView && m_warningView->isVisible()) return;

    if (!m_currentSelectedBtn->isDisabled() &&
        !m_currentSelectedBtn->isChecked())
        m_currentSelectedBtn->updateState(RoundItemButton::Normal);

    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    const int nextPos = lastPos ? lastPos : m_btnsList->count();

    m_currentSelectedBtn = m_btnsList->at(nextPos - 1);

    if (m_currentSelectedBtn->isDisabled() || !m_currentSelectedBtn->isVisible())
        setPreviousChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ContentWidget::setNextChildFocus()
{
    if (m_warningView && m_warningView->isVisible()) return;

    if (!m_currentSelectedBtn->isDisabled() &&
        !m_currentSelectedBtn->isChecked())
        m_currentSelectedBtn->updateState(RoundItemButton::Normal);

    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    m_currentSelectedBtn = m_btnsList->at((lastPos + 1) % m_btnsList->count());

    if (m_currentSelectedBtn->isDisabled() || !m_currentSelectedBtn->isVisible())
        setNextChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ContentWidget::showTips(const QString &tips)
{
    if (!tips.isEmpty()) {
//        m_tipsLabel->setText(tips);
        m_tipsWidget->show();
//        m_shutdownButton->setDisabled(true);
//        m_restartButton->setDisabled(true);
//        m_suspendButton->setDisabled(true);
//        m_logoutButton->setDisabled(true);
    } else {
        m_tipsWidget->hide();
        m_shutdownButton->setDisabled(false);
        m_restartButton->setDisabled(false);
        m_suspendButton->setDisabled(false);
        m_logoutButton->setDisabled(false);
        m_shutdownButton->updateState(RoundItemButton::Hover);
    }
}

void ContentWidget::hideBtns(const QStringList &btnsName)
{
    for (const QString &name : btnsName)
        hideBtn(name);
}

void ContentWidget::disableBtns(const QStringList &btnsName)
{
    for (const QString &name : btnsName)
        disableBtn(name);
}

void ContentWidget::onCancel()
{
#ifdef SHUTDOWN_NO_QUIT
    hideToplevelWindow();
    recoveryLayout();
#else
    qApp->quit();
#endif
}
