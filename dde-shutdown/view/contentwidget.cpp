#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QObject>

#include "contentwidget.h"

ShutDownFrame::ShutDownFrame(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
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
    connect(signalManager, SIGNAL(buttonStyleChanged()), this, SLOT(updateStyleSheet()));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_shutdownButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_restartButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_suspendButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_lockButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_userSwitchButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_logoutButton, SLOT(setButtonMutex(QString)));
    connect(signalManager, SIGNAL(setButtonCheck(QString)), this, SLOT(setButtonGroupMutex(QString)));


    connect(signalManager, SIGNAL(setButtonHover(QString)), m_shutdownButton, SLOT(setButtonHoverMutex(QString)));
    connect(signalManager, SIGNAL(setButtonHover(QString)), m_restartButton, SLOT(setButtonHoverMutex(QString)));
    connect(signalManager, SIGNAL(setButtonHover(QString)), m_suspendButton, SLOT(setButtonHoverMutex(QString)));
    connect(signalManager, SIGNAL(setButtonHover(QString)), m_lockButton, SLOT(setButtonHoverMutex(QString)));
    connect(signalManager, SIGNAL(setButtonHover(QString)), this, SLOT(test(QString)));
}

void ShutDownFrame::initUI() {
    m_shutdownButton = new SessionButton(tr("Shut down"), "ShutDownButton");
    m_shutdownButton->setObjectName("ShutDownButtonFrame");
    m_shutdownButton->setHover(true);
    m_restartButton = new SessionButton(tr("Restart"), "RestartButton");
    m_restartButton->setObjectName("RestartButtonFrame");
    m_suspendButton = new SessionButton(tr("Suspend"), "SuspendButton");
    m_suspendButton->setObjectName("SuspendButtonFrame");
    m_lockButton = new SessionButton(tr("Lock"), "LockButton");
    m_lockButton->setObjectName("LockButtonFrame");
//    m_userSwitchButton = new SessionButton("SwitchUser", "UserSwitchButton");
//    m_userSwitchButton->setObjectName("UserSwitchButtonFrame");
    m_logoutButton = new SessionButton(tr("Log out"), "LogoutButton");
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
}

void ShutDownFrame::setButtonGroupMutex(QString buttonId) {
    Q_UNUSED(buttonId);
}
void ShutDownFrame::test(QString test) {
    qDebug() << "hover" << test;
}
void ShutDownFrame::ShutDownAction( ) {

    QList<SessionButton*> m_children = this->findChildren<SessionButton *>();
    for (int i = 0; i < m_children.length(); i++) {
        if (m_children[i]->isChecked()) {
            emit m_children[i]->buttonAction(m_children[i]->buttonId());
        }
    }

}
void ShutDownFrame::updateStyleSheet(){
    qDebug() << "testing global style update";
    updateStyle(":/skin/shutdown.qss", this);
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

ShutDownFrame::~ShutDownFrame()
{}
