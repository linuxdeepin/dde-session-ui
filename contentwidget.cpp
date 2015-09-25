#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include "contentwidget.h"
#include <QtCore/QObject>
ShutDownFrame::ShutDownFrame(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("shutdownFrame");
    m_shutdownButton = new SessionButton("ShutDown", "shutdownButton1");
    m_shutdownButton->setObjectName("shutdownButtonFrame1");
    m_restartButton = new SessionButton("ReStart", "restartButton1");
    m_restartButton->setObjectName("restartButtonFrame1");
    m_suspendButton = new SessionButton("Suspend", "suspendButton1");
    m_suspendButton->setObjectName("suspendButtonFrame1");
    m_lockButton = new SessionButton("Lock", "lockButton1");
    m_lockButton->setObjectName("lockButtonFrame1");
    m_userSwitchButton = new SessionButton("SwitchUser", "userSwitchButton1");
    m_userSwitchButton->setObjectName("userSwitchButtonFrame1");
    m_logoutButton = new SessionButton("Logout", "logoutButton1");
    m_logoutButton->setObjectName("logoutButtonFrame1");

    setUserSwitchButton(false);


    m_buttonLayout = new QHBoxLayout(this);
    m_buttonLayout->setMargin(0);
    m_buttonLayout->setSpacing(35);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_shutdownButton);
    m_buttonLayout->addWidget(m_restartButton);
    m_buttonLayout->addWidget(m_suspendButton);
    m_buttonLayout->addWidget(m_lockButton);
    m_buttonLayout->addWidget(m_userSwitchButton);
    m_buttonLayout->addWidget(m_logoutButton);
    m_buttonLayout->addStretch(0);

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addLayout(m_buttonLayout);
    m_Layout->addStretch();
    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_Layout);
    setFixedHeight(150);
    qDebug() << "SHUTDOWNFRAME!!!";

    connect(this, SIGNAL(keyLeft()), SLOT(setPreviousChildFocus()));
    connect(this, SIGNAL(keyRight()), SLOT(setNextChildFocus()));

}

ShutDownFrame::~ShutDownFrame()
{
}
void ShutDownFrame::setUserSwitchButton(bool showing) {
    if (showing) {
        m_userSwitchButton->hide();
    } else {
        m_userSwitchButton->show();
    }
}
void ShutDownFrame::setPreviousChildFocus() {

    m_shutdownButton->setFocusPolicy(Qt::StrongFocus);
    m_restartButton->setFocusPolicy(Qt::StrongFocus);
    m_suspendButton->setFocusPolicy(Qt::StrongFocus);
    m_lockButton->setFocusPolicy(Qt::StrongFocus);
    m_userSwitchButton->setFocusPolicy(Qt::StrongFocus);
    m_logoutButton->setFocusPolicy(Qt::StrongFocus);
    qDebug() << "left child";
     this->focusPreviousChild();
}
void ShutDownFrame::setNextChildFocus() {
    qDebug() << "right child";
    m_shutdownButton->setFocusPolicy(Qt::StrongFocus);
    m_restartButton->setFocusPolicy(Qt::StrongFocus);
    m_suspendButton->setFocusPolicy(Qt::StrongFocus);
    m_lockButton->setFocusPolicy(Qt::StrongFocus);
    m_logoutButton->setFocusPolicy(Qt::StrongFocus);

    this->focusNextChild();
}
LoginFrame::LoginFrame(bool login, QWidget *parent)
    : QFrame(parent)
{
    m_login = login;
    m_passwordEdit = new PassWdEdit;
    m_passwordEdit->switchStatus(m_login);
    m_userNameLayout = new QHBoxLayout;
    m_userNameLayout->addStretch();
    m_userNameLayout->addWidget(m_userNameLabel);
    m_userNameLayout->addStretch();

    QHBoxLayout* passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(0);
    passwdLayout->setSpacing(0);
    passwdLayout->addStretch(0);
    passwdLayout->addWidget(m_passwordEdit);
    passwdLayout->addStretch(0);

//    m_passwdLineEditLayout = new QHBoxLayout;
//    m_passwdLineEditLayout->setMargin(0);
//    m_passwdLineEditLayout->setSpacing(0);
//    m_passwdLineEditLayout->addStretch(0);
//    m_passwdLineEditLayout->addLayout(passwdLayout);
//    m_passwdLineEditLayout->addStretch();


    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(50);
//    m_Layout->addLayout(m_passwdLineEditLayout);

    setLayout(m_Layout);
}
LoginFrame::~LoginFrame()
{
}
QuickShutDownFrame::QuickShutDownFrame(QWidget *parent)
    : QFrame(parent)
{
    m_quickShutDownButton = new SessionButton("ShutDown", "shutdownButton2");
    m_quickShutDownButton->setObjectName("shutdownButtonFrame2");
    m_quickRestartButton = new SessionButton("ReStart", "restartButton2");
    m_quickRestartButton->setObjectName("restartButtonFrame2");
    m_quickSuspendButton = new SessionButton("Suspend", "suspendButton2");
    m_quickSuspendButton->setObjectName("suspendButtonFrame2");

//    m_Layout = new QHBoxLayout;
//    m_Layout->setMargin(0);
//    m_Layout->setSpacing(20);
//    m_Layout->addStretch();
//    m_Layout->addWidget(m_quickShutDownButton);
//    m_Layout->addWidget(m_quickRestartButton);
//    m_Layout->addWidget(m_quickSuspendButton);
//    m_Layout->addStretch();
    setFocusPolicy(Qt::StrongFocus);
//    setLayout(m_Layout);
    connect(this, SIGNAL(keyLeft()), SLOT(setPreviousChildFocus()));
    connect(this, SIGNAL(keyRight()), SLOT(setNextChildFocus()));
}
void QuickShutDownFrame::setPreviousChildFocus() {
    m_quickShutDownButton->setFocusPolicy(Qt::StrongFocus);
    m_quickRestartButton->setFocusPolicy(Qt::StrongFocus);
    m_quickSuspendButton->setFocusPolicy(Qt::StrongFocus);

    this->focusPreviousChild();
}
void QuickShutDownFrame::setNextChildFocus() {
    m_quickShutDownButton->setFocusPolicy(Qt::StrongFocus);
    m_quickRestartButton->setFocusPolicy(Qt::StrongFocus);
    m_quickSuspendButton->setFocusPolicy(Qt::StrongFocus);

   this->focusNextChild();
}
QuickShutDownFrame::~QuickShutDownFrame()
{
}
