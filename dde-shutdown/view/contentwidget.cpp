#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include "contentwidget.h"
#include <QtCore/QObject>

ShutDownFrame::ShutDownFrame(QWidget *parent)
    : QFrame(parent)
{


    m_shutdownButton = new SessionButton("ShutDown", "ShutDownButton");
    m_shutdownButton->setObjectName("ShutDownButtonFrame");
    m_shutdownButton->setButtonChecked();
    m_restartButton = new SessionButton("Restart", "RestartButton");
    m_restartButton->setObjectName("RestartButtonFrame");
    m_suspendButton = new SessionButton("Suspend", "SuspendButton");
    m_suspendButton->setObjectName("SuspendButtonFrame");
    m_lockButton = new SessionButton("Lock", "LockButton");
    m_lockButton->setObjectName("LockButtonFrame");
//    m_userSwitchButton = new SessionButton("SwitchUser", "UserSwitchButton");
//    m_userSwitchButton->setObjectName("UserSwitchButtonFrame");
    m_logoutButton = new SessionButton("Logout", "LogoutButton");
    m_logoutButton->setObjectName("LogoutButtonFrame");

//    setUserSwitchButton(false);

    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->setMargin(0);
    m_buttonLayout->setSpacing(10);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_shutdownButton);
    m_buttonLayout->addWidget(m_restartButton);
    m_buttonLayout->addWidget(m_suspendButton);
    m_buttonLayout->addWidget(m_lockButton);
//    m_buttonLayout->addWidget(m_userSwitchButton);
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

    QFile qssFile(":/skin/shutdown.qss");
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }

    initConnect();
}

ShutDownFrame::~ShutDownFrame()
{
}
void ShutDownFrame::initConnect() {
    connect(this, SIGNAL(keyLeft()), SLOT(setPreviousChildFocus()));
    connect(this, SIGNAL(keyRight()), SLOT(setNextChildFocus()));
    connect(m_shutdownButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
    connect(m_restartButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
    connect(m_suspendButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
    connect(m_lockButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_userSwitchButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
    connect(m_logoutButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));

    connect(this, SIGNAL(pressEnterAction()), this, SLOT(ShutDownAction()));

    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_shutdownButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_restartButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_suspendButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_lockButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_userSwitchButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_logoutButton, SLOT(setButtonMutex(QString)));

    connect(signalManager, SIGNAL(setButtonCheck(QString)), this, SLOT(setButtonGroupMutex(QString)));
}
void ShutDownFrame::setButtonGroupMutex(QString buttonId) {
    Q_UNUSED(buttonId);
}

void ShutDownFrame::ShutDownAction( ) {

    QList<SessionButton*> m_children = this->findChildren<SessionButton *>();
    for (int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->isChecked()) {
            emit m_children[i]->buttonAction(m_children[i]->buttonId());
        }
    }

}
/*void ShutDownFrame::setUserSwitchButton(bool showing) {
    if (showing) {
        m_userSwitchButton->show();
    } else {
        m_userSwitchButton->hide();
    }
}*/
void ShutDownFrame::setPreviousChildFocus() {
    QList<SessionButton*> m_children = this->findChildren<SessionButton *>();
    m_count--;
    if (m_count <= -1) {
        m_count = m_children.length()-1;
    }
    emit signalManager->setButtonCheck(m_children[m_count]->objectName());
}
void ShutDownFrame::setNextChildFocus() {

    QList<SessionButton*> m_children = this->findChildren<SessionButton *>();
    m_count++;
    if (m_count >= m_children.length()) {
        m_count = 0;
    }
    m_children[m_count]->setChecked(true);
    emit signalManager->setButtonCheck(m_children[m_count]->objectName());

}
LoginFrame::LoginFrame(QWidget *parent)
    : QFrame(parent)
{
    m_passwordEdit = new PassWdEdit;
    m_userFrame = new UserFrame;

    QHBoxLayout* userLayout = new QHBoxLayout;
    userLayout->setMargin(0);
    userLayout->setSpacing(0);
    userLayout->addStretch();
    userLayout->addWidget(m_userFrame);
    userLayout->addStretch();

    QHBoxLayout* passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(0);
    passwdLayout->setSpacing(0);
    passwdLayout->addStretch(0);
    passwdLayout->addWidget(m_passwordEdit);
    passwdLayout->addStretch(0);

    m_passwdLineEditLayout = new QHBoxLayout;
    m_passwdLineEditLayout->setMargin(0);
    m_passwdLineEditLayout->setSpacing(0);
    m_passwdLineEditLayout->addStretch(0);
    m_passwdLineEditLayout->addLayout(passwdLayout);
    m_passwdLineEditLayout->addStretch();

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(10);
    m_Layout->addStretch();
    m_Layout->addLayout(userLayout);
    m_Layout->addLayout(m_passwdLineEditLayout);
    m_Layout->addStretch();
    setLayout(m_Layout);

}
LoginFrame::~LoginFrame()
{
}
void LoginFrame::setMode(int mode) {
    if (mode == 0) {
        m_passwordEdit->switchStatus(true);
    } else  {
        m_passwordEdit->switchStatus(false);
    }
}
QuickShutDownFrame::QuickShutDownFrame(QWidget *parent)
    : QFrame(parent)
{
    m_quickShutDownButton = new SessionButton("ShutDown", "QuickShutDownButton");
    m_quickShutDownButton->setObjectName("QuickShutDownButtonFrame");
    m_quickRestartButton = new SessionButton("Restart", "QuickRestartButton");
    m_quickRestartButton->setObjectName("QuickRestartButtonFrame");
    m_quickSuspendButton = new SessionButton("Suspend", "QuickSuspendButton");
    m_quickSuspendButton->setObjectName("QuickSuspendButtonFrame");

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(35);
    m_Layout->addStretch();
    m_Layout->addWidget(m_quickShutDownButton);
    m_Layout->addWidget(m_quickRestartButton);
    m_Layout->addWidget(m_quickSuspendButton);
    m_Layout->addStretch();
    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_Layout);

    QFile qssFile(":/skin/quickshutdown.qss");
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }


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
