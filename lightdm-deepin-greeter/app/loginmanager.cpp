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
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setFocusPolicy(Qt::NoFocus);

    m_backgroundLabel = new BackgroundLabel(true, this);
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

void LoginManager::initConnect()
{
    connect(m_switchFrame, &SwitchFrame::triggerPower, this, &LoginManager::showShutdownFrame);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchSession, this, &LoginManager::chooseSessionMode);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, this, &LoginManager::chooseUserMode);

    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, m_passWdEdit, &PassWdEdit::hide);
    connect(m_switchFrame, &SwitchFrame::triggerSwitchUser, m_userWidget, &UserWidget::expandWidget, Qt::QueuedConnection);
    connect(m_passWdEdit, &PassWdEdit::submit, this, &LoginManager::login);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, this, &LoginManager::chooseUserMode);
    connect(m_sessionWidget, &SessionWidget::sessionChanged, m_switchFrame, &SwitchFrame::chooseToSession);
    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, &PassWdEdit::show);
    connect(m_userWidget, &UserWidget::userChanged, m_sessionWidget, &SessionWidget::switchToUser);
    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, &PassWdEdit::updateKeybordLayoutStatus);
    connect(m_userWidget, &UserWidget::userChanged, m_passWdEdit, static_cast<void (PassWdEdit::*)()>(&PassWdEdit::setFocus));
    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &LoginManager::prompt);
    connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &LoginManager::authenticationComplete);
    connect(m_passWdEdit, &PassWdEdit::updateKeyboardStatus, this, &LoginManager::keyboardLayoutUI);
    connect(m_passWdEdit, &PassWdEdit::keybdLayoutButtonClicked, this, &LoginManager::keybdLayoutWidget);

    connect(m_passWdEdit, &PassWdEdit::leftKeyPressed, this, &LoginManager::leftKeyPressed);
    connect(m_passWdEdit, &PassWdEdit::rightKeyPressed, this, &LoginManager::rightKeyPressed);
    connect(this, &LoginManager::leftKeyPressed, m_userWidget, &UserWidget::leftKeySwitchUser);
    connect(this, &LoginManager::rightKeyPressed, m_userWidget, &UserWidget::rightKeySwitchUser);
    connect(m_requireShutdownWidget, &ShutdownWidget::shutDownWidgetAction, this, &LoginManager::setShutdownAction);
}

void LoginManager::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "prompt: " << text << type;

    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:   m_greeter->respond(m_passWdEdit->getText().trimmed());     break;
    default:;
    }
}

void LoginManager::authenticationComplete()
{
    qDebug() << "authenticationComplete";
    m_userWidget->hideLoadingAni();

    if (!m_greeter->isAuthenticated()) {
        m_passWdEdit->setAlert(true);
        qWarning() << "auth fail !!!";
        return;
    }

    qDebug() << "session = " << m_sessionWidget->currentSessionName();
    qDebug() << "start session: " << m_greeter->startSessionSync(m_sessionWidget->currentSessionKey());
}

void LoginManager::chooseUserMode()
{
    m_sessionWidget->hide();
    m_userWidget->show();
    m_passWdEdit->show();
    m_requireShutdownWidget->hide();
}

void LoginManager::chooseSessionMode()
{
    m_sessionWidget->show();
    m_userWidget->hide();
    m_passWdEdit->hide();
    m_requireShutdownWidget->hide();
}

void LoginManager::updateWidgetsPosition()
{
    const int height = this->height();
    const int width = this->width();

    m_userWidget->setFixedWidth(width);
    m_userWidget->move(0, (height - m_userWidget->height()) / 2 - 95); // center and margin-top: -95px
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
    m_requireShutdownWidget->show();
}


void LoginManager::keyPressEvent(QKeyEvent* e) {
#ifndef QT_DEBUG
    Q_UNUSED(e)
#else
    if (e->key() == Qt::Key_Escape) {
        qApp->exit();
    }
#endif
}

void LoginManager::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        if (!m_keybdLayoutWidget->isHidden()) {
            m_keybdLayoutWidget->hide();
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
            move(QPoint(geometry.left(), geometry.top()));

            updateWidgetsPosition();
            return;
        }
    }
}

void LoginManager::login()
{
    m_userWidget->showLoadingAni();

    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

//    m_passWdEdit->setAlert(true);

    // save user last choice
    m_sessionWidget->saveUserLastSession(m_userWidget->currentUser());
    m_userWidget->saveLastUser();

    const QString &username = m_userWidget->currentUser();
    m_greeter->authenticate(username);
    qDebug() << "choose user: " << username;
    qDebug() << "auth user: " << m_greeter->authenticationUser();
}

void LoginManager::keyboardLayoutUI() {

    QStringList keyboardList;
    keyboardList = m_passWdEdit->keyboardLayoutList;

    xkbParse = new XkbParser(this);
    QStringList keyboardListContent =  xkbParse->lookUpKeyboardList(keyboardList);


    m_keybdLayoutWidget = new KbLayoutWidget(keyboardListContent, this);

    m_keybdLayoutWidget->move(m_passWdEdit->x(), m_passWdEdit->y() + m_passWdEdit->height() + 10);
    m_keybdLayoutWidget->hide();

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LoginManager::setCurrentKeyboardLayout);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, &KbLayoutWidget::hide);
}

void LoginManager::setCurrentKeyboardLayout(QString keyboard_value) {
    qDebug() << "setCurrentKeyboardLayout";

    QString keyboard_key = xkbParse->lookUpKeyboardKey(keyboard_value);
    qDebug() << "parse:" << keyboard_value << keyboard_value;
    m_passWdEdit->utilSettings->setCurrentKbdLayout(m_userWidget->currentUser(),keyboard_key);

}

void LoginManager::keybdLayoutWidget() {
    if (m_keybdLayoutWidget->isHidden()) {
        m_keybdLayoutWidget->show();
    } else {
        m_keybdLayoutWidget->hide();
    }

}

void LoginManager::setShutdownAction(const ShutdownWidget::Actions action) {

    switch (action) {
        case ShutdownWidget::RequireShutdown: { m_login1ManagerInterface->PowerOff(true); break;}
        case ShutdownWidget::RequireRestart: { m_login1ManagerInterface->Reboot(true); break;}
        case ShutdownWidget::RequireSuspend: { m_login1ManagerInterface->Suspend(true); break;}
        default:;
    }
}
