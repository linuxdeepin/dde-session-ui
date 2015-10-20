#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>
#include <QDesktopWidget>
#include <QDebug>

#include "sessionlogin.h"

SessionLogin::SessionLogin(QWidget* parent)
    : QFrame(parent),
      m_greeter(new QLightDM::Greeter(this)),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    if (!m_greeter->connectSync())
        qWarning() << "greeter connect fail !!!";

    setObjectName("SessionLoginTool");
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setFixedSize(qApp->desktop()->size());

    initUI();
    initConnect();

    m_passWdEdit->setFocusPolicy(Qt::StrongFocus);
    m_passWdEdit->setFocus();
}

SessionLogin::~SessionLogin()
{

}

void SessionLogin::initUI() {
    m_backgroundLabel = new BackgroundLabel(true, this);
    m_logoWidget = new LogoWidget(this);
    m_logoWidget->move(0, height() - m_logoWidget->height() - 20);
    m_switchFrame = new SwitchFrame(this);
    m_switchFrame->move(width()-m_switchFrame->width(), height() - m_switchFrame->height());
    m_userWidget = new UserWidget(this);
    m_userWidget->setObjectName("UserWidget");
//    m_userWidget->updateGeometry();
    m_passWdEdit = new PassWdEdit("LoginIcon", this);

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
    updateStyle(":/skin/login.qss", this);
    showFullScreen();

}
void SessionLogin::initConnect() {
    connect(m_passWdEdit, SIGNAL(jumpTo()), this, SLOT(Login()));
    connect(m_switchFrame, SIGNAL(switchItem(QString)), this, SLOT(testing(QString)));
    connect(m_greeter, &QLightDM::Greeter::showPrompt, this, &SessionLogin::prompt);
    connect(m_greeter, &QLightDM::Greeter::authenticationComplete, this, &SessionLogin::authenticationComplete);
}

void SessionLogin::prompt(QString text, QLightDM::Greeter::PromptType type)
{
    qDebug() << "prompt: " << text << type;

    switch (type)
    {
    case QLightDM::Greeter::PromptTypeSecret:   m_greeter->respond(m_passWdEdit->getText().trimmed());     break;
    default:;
    }
}

void SessionLogin::authenticationComplete()
{
    qDebug() << "authenticationComplete";

    if (!m_greeter->isAuthenticated()) {
        qWarning() << "auth fail !!!";
        return;
    }

    // TODO:
    qDebug() << "start session: " << m_greeter->startSessionSync(m_sessionModel->data(m_sessionModel->index(0), QLightDM::SessionsModel::KeyRole).toString());
}

void SessionLogin::keyPressEvent(QKeyEvent* e) {
#ifndef QT_DEBUG
    Q_UNUSED(e)
#else
    if (e->key() == Qt::Key_Escape) {
        qApp->exit();
    }
#endif
}
void SessionLogin::Login()
{
    if (m_greeter->inAuthentication())
        m_greeter->cancelAuthentication();

    // TODO:
    const QString &username = m_userWidget->currentUser();
    qDebug() << username;
    m_greeter->authenticate(username);

    qDebug() << "auth user: " << m_greeter->authenticationUser();
}

void SessionLogin::testing(QString id) {
    qDebug() << "switch User!" << id;
    if (id == "SwitchUser") {
        m_userWidget->expandWidget();
    }
}

void SessionLogin::moveUserWidget() {

}
