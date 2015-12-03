#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>
#include <QDesktopWidget>
#include <QDebug>

#include "loginmanager.h"

LoginManager::LoginManager(QWidget* parent)
    : QFrame(parent),
      m_greeter(new QLightDM::Greeter(this))
{
    recordPid();
    if (!m_greeter->connectSync())
        qWarning() << "greeter connect fail !!!";

    initUI();
    initConnect();

    m_passWdEdit->updateKeybordLayoutStatus(m_userWidget->currentUser());
    m_sessionWidget->switchToUser(m_userWidget->currentUser());

    m_displayInter = new DBusDisplayManager("org.freedesktop.DisplayManager", "/org/freedesktop/DisplayManager", QDBusConnection::systemBus(), this);

    // delay to expand
    if (m_displayInter->isValid() && m_displayInter->sessions().count()) {
        QMetaObject::invokeMethod(m_switchFrame, "triggerSwitchUser", Qt::QueuedConnection);
        qDebug() << "m_displayInter:" << m_displayInter->isValid();
    }


    m_login1ManagerInterface =new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    if (!m_login1ManagerInterface->isValid()) {
        qDebug() <<"m_login1ManagerInterface:" << m_login1ManagerInterface->lastError().type();
    }

}

LoginManager::~LoginManager()
{

}

void LoginManager::initUI()
{
    setFixedSize(qApp->desktop()->size());
    setObjectName("LoginManagerTool");

    m_sessionWidget = new SessionWidget(this);
    m_sessionWidget->hide();
    m_logoWidget = new LogoWidget(this);
    m_switchFrame = new SwitchFrame(this);
    m_userWidget = new UserWidget(this);

    m_userWidget->setObjectName("UserWidget");
    m_passWdEdit = new PassWdEdit(this);
    m_passWdEdit->setEnterBtnStyle(":/img/login_normal.png", ":/img/login_normal.png", ":/img/login_press.png");

    m_passWdEdit->setFocusPolicy(Qt::StrongFocus);
    m_passWdEdit->setFocus();

    m_requireShutdownWidget = new ShutdownWidget(this);
    m_requireShutdownWidget->hide();
    m_passWdEditLayout = new QHBoxLayout;
    m_passWdEditLayout->setMargin(0);
    m_passWdEditLayout->setSpacing(0);
    m_passWdEditLayout->addStretch();
    m_passWdEditLayout->addWidget(m_passWdEdit);
    m_passWdEditLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addLayout(m_passWdEditLayout);
    m_Layout->addStretch();
    setLayout(m_Layout);

    showFullScreen();

    m_passWdEdit->updateKeyboardStatus();
    keyboardLayoutUI();
    leaveEvent(nullptr);

    m_switchFrame->setUserSwitchEnable(m_userWidget->count() > 1);

    updateStyle(":/skin/login.qss", this);
}

void LoginManager::recordPid() {
    qDebug() << "remember P i D" << qApp->applicationPid();

    QFile lockPIdFile(QString("%1%2").arg("/tmp/").arg(".dgreeterpid"));

    if (lockPIdFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream pidInfo(&lockPIdFile);
        pidInfo << qApp->applicationPid();

        lockPIdFile.close();
    } else {
        qDebug() << "file open failed!";
    }
}

void LoginManager::initConnect()
{
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, this, &LoginManager::chooseUserMode);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, m_passWdEdit, &PassWdEdit::hide);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);

    connect(m_switchFrame, &SwitchFrame::triggerPower, this, &LoginManager::showShutdownFrame);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchSession, this, &LoginManager::chooseSessionMode);

    connect(m_passWdEdit, &PassWdEdit::submit, this, &LoginManager::login);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, this, &LoginManager::choosedSession);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, m_switchFrame, &SwitchFrame::chooseToSession, Qt::QueuedConnection);

    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, &PassWdEdit::show);
    connect(m_userWidget, &UserWidget::userChanged, m_sessionWidget, &SessionWidget::switchToUser);
    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, &PassWdEdit::updateKeybordLayoutStatus);
    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, static_cast<void (PassWdEdit::*)()>(&PassWdEdit::setFocus));

    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LoginManager::prompt);
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
}

void LoginManager::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "prompt: " << text << type;

    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:   m_greeter->respond(m_passWdEdit->getText());     break;
    default:;
    }
}

void LoginManager::authenticationComplete()
{
    qDebug() << "authenticationComplete";
    m_userWidget->hideLoadingAni();

    if (!m_greeter->isAuthenticated()) {
        m_passWdEdit->setAlert(true, tr("Wrong password"));
        qWarning() << "auth fail !!!";
        return;
    }

    qDebug() << "session = " << m_sessionWidget->currentSessionName();
    qDebug() << "start session: " << m_greeter->startSessionSync(m_sessionWidget->currentSessionKey());

//////////////kill_ddelock
    m_lockInter = new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);
    if (m_lockInter->isValid()) {
        qDebug() << "LOck Greeter: " << m_lockInter->IsLiveCD(m_userWidget->currentUser());
        /////////////kill_lock
        m_lockInter->ExitLock(m_userWidget->currentUser(), m_passWdEdit->getText());
    } else {
        qDebug() << "m_lockInter error!";
    }
///////////////
}

void LoginManager::chooseUserMode()
{
    m_sessionWidget->hide();
    m_userWidget->show();
    m_requireShutdownWidget->hide();
}

void LoginManager::chooseSessionMode()
{
    m_sessionWidget->show();
    m_userWidget->hide();
    m_passWdEdit->hide();
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
        m_passWdEdit->show();
    }
    if (!m_keybdArrowWidget->isHidden()) {
        m_keybdArrowWidget->hide();
    }
}

void LoginManager::updateWidgetsPosition()
{
    const int height = this->height();
    const int width = this->width();

    m_userWidget->setFixedWidth(width);
    m_userWidget->move(0, (height - m_userWidget->height()) / 2 - 95); // center and margin-top: -95px
    qDebug() << "Change Screens" << m_userWidget->isChooseUserMode;
    m_sessionWidget->setFixedWidth(width);
    m_sessionWidget->move(0, (height - m_sessionWidget->height()) / 2 - 70); // 中间稍往上的位置
    m_logoWidget->move(48, height - m_logoWidget->height() - 36); // left 48px and bottom 36px
    m_switchFrame->move(width - m_switchFrame->width() - 20, height - m_switchFrame->height());
    m_requireShutdownWidget->setFixedWidth(width);
    m_requireShutdownWidget->setFixedHeight(300);
    m_requireShutdownWidget->move(0,  (height - m_requireShutdownWidget->height())/2 - 60);
}

void LoginManager::showShutdownFrame() {
    qDebug() << "showShutdownFrame!";
    m_userWidget->hide();
    m_passWdEdit->hide();
    m_sessionWidget->hide();
    m_requireShutdownWidget->show();
}


void LoginManager::keyPressEvent(QKeyEvent* e) {
    qDebug() << "qDebug loginManager:" << e->text();

    if (e->key() == Qt::Key_Escape) {
        if (!m_requireShutdownWidget->isHidden()) {
            m_requireShutdownWidget->hide();
            m_userWidget->show();
            m_passWdEdit->show();
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
            m_passWdEdit->show();
        }

        if (m_keybdArrowWidget->isHidden()) {
            m_keybdArrowWidget->hide();
        }
    }
}

void LoginManager::leaveEvent(QEvent *)
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList)
    {
        if (screen->geometry().contains(mousePoint))
        {
            const QRect &geometry = screen->geometry();
            setFixedSize(geometry.size());
            emit screenChanged(geometry);
            return;
        }
    }
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
        m_sessionWidget->chooseSession();
        return;
    }
    if (m_userWidget->isChooseUserMode && !m_userWidget->isHidden()) {
        m_userWidget->chooseButtonChecked();
        m_passWdEdit->getFocusTimer->start();
        qDebug() << "lineEditGrabKeyboard";
        return;
    }

    qDebug() << "sososos:" << m_passWdEdit->getText();
    const QString &username = m_userWidget->currentUser();

    if (!m_passWdEdit->isVisible())
        return;

    m_userWidget->showLoadingAni();

    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

    //    m_passWdEdit->setAlert(true, "asd");

    // save user last choice
    m_sessionWidget->saveUserLastSession(m_userWidget->currentUser());
    m_userWidget->saveLastUser();

    //    const QString &username = m_userWidget->currentUser();
    m_greeter->authenticate(username);
    qDebug() << "choose user: " << username;
    qDebug() << "auth user: " << m_greeter->authenticationUser();

}

void LoginManager::keyboardLayoutUI() {

    QStringList keyboardList;
    keyboardList = m_passWdEdit->keyboardLayoutList;

    xkbParse = new XkbParser(this);
    QStringList keyboardListContent =  xkbParse->lookUpKeyboardList(keyboardList);


    m_keybdLayoutWidget = new KbLayoutWidget(keyboardListContent);

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

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LoginManager::setCurrentKeyboardLayout);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, m_keybdArrowWidget, &DArrowRectangle::hide);
}

void LoginManager::setCurrentKeyboardLayout(QString keyboard_value) {
    qDebug() << "setCurrentKeyboardLayout";

    QString keyboard_key = xkbParse->lookUpKeyboardKey(keyboard_value);
    qDebug() << "parse:" << keyboard_value << keyboard_value;
    m_passWdEdit->utilSettings->setCurrentKbdLayout(m_userWidget->currentUser(),keyboard_key);

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
                m_passWdEdit->show();
            }
            m_sessionWidget->hide();
        break;}
        default:;
    }
}

void LoginManager::leftKeyPressed() {
    if (!m_userWidget->isHidden()) {
        m_userWidget->leftKeySwitchUser();
    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->leftKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
        m_sessionWidget->leftKeySwitch();
    }
}

void LoginManager::rightKeyPressed() {
    if (!m_userWidget->isHidden()) {
        m_userWidget->rightKeySwitchUser();
    }
    if (!m_requireShutdownWidget->isHidden()) {
        m_requireShutdownWidget->rightKeySwitch();
    }
    if (!m_sessionWidget->isHidden()) {
        m_sessionWidget->rightKeySwitch();
    }
}
