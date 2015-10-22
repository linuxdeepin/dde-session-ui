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

ShutDownFrame::~ShutDownFrame()
{

}

void ShutDownFrame::initConnect() {
    connect(this, SIGNAL(keyLeft()), SLOT(setPreviousChildFocus()));
    connect(this, SIGNAL(keyRight()), SLOT(setNextChildFocus()));
    connect(this, &ShutDownFrame::pressEnterAction, this, &ShutDownFrame::buttonClicked);

    connect(m_shutdownButton, &RoundItemButton::clicked, this, &ShutDownFrame::buttonClicked);
    connect(m_restartButton, &RoundItemButton::clicked, this, &ShutDownFrame::buttonClicked);
    connect(m_suspendButton, &RoundItemButton::clicked, this, &ShutDownFrame::buttonClicked);
    connect(m_lockButton, &RoundItemButton::clicked, this, &ShutDownFrame::buttonClicked);
    connect(m_logoutButton, &RoundItemButton::clicked, this, &ShutDownFrame::buttonClicked);
//    connect(m_shutdownButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_restartButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_suspendButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_lockButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_userSwitchButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));
//    connect(m_logoutButton, SIGNAL(buttonAction(QString)), this, SIGNAL(ShutDownFrameActions(QString)));

//    connect(this, SIGNAL(pressEnterAction()), this, SLOT(ShutDownAction()));
//    connect(signalManager, SIGNAL(buttonStyleChanged()), this, SLOT(updateStyleSheet()));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_shutdownButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_restartButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_suspendButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_lockButton, SLOT(setButtonMutex(QString)));
////    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_userSwitchButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), m_logoutButton, SLOT(setButtonMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonCheck(QString)), this, SLOT(setButtonGroupMutex(QString)));


//    connect(signalManager, SIGNAL(setButtonHover(QString)), m_shutdownButton, SLOT(setButtonHoverMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonHover(QString)), m_restartButton, SLOT(setButtonHoverMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonHover(QString)), m_suspendButton, SLOT(setButtonHoverMutex(QString)));
//    connect(signalManager, SIGNAL(setButtonHover(QString)), m_lockButton, SLOT(setButtonHoverMutex(QString)));
    //    connect(signalManager, SIGNAL(setButtonHover(QString)), this, SLOT(test(QString)));
}

void ShutDownFrame::buttonClicked()
{
    if (m_currentSelectedBtn == m_shutdownButton)
        emit ShutDownFrameActions("ShutDownButton");
    else if (m_currentSelectedBtn == m_restartButton)
        emit ShutDownFrameActions("RestartButton");
    else if (m_currentSelectedBtn == m_suspendButton)
        emit ShutDownFrameActions("SuspendButton");
    else if (m_currentSelectedBtn == m_lockButton)
        emit ShutDownFrameActions("LockButton");
    else if (m_currentSelectedBtn == m_logoutButton)
        emit ShutDownFrameActions("LogoutButton");
}

void ShutDownFrame::initUI() {
    m_btnsList = new QList<RoundItemButton *>;
    m_shutdownButton = new RoundItemButton(tr("Shut down"));
    m_shutdownButton->setObjectName("ShutDownButtonFrame");
    m_btnsList->append(m_shutdownButton);
    m_shutdownButton->setState(RoundItemButton::Checked);
    m_currentSelectedBtn = m_shutdownButton;
    // TODO:
//    m_shutdownButton->setHover(true);
    m_restartButton = new RoundItemButton(tr("Restart"));
    m_restartButton->setObjectName("RestartButtonFrame");
    m_btnsList->append(m_restartButton);
    m_suspendButton = new RoundItemButton(tr("Suspend"));
    m_suspendButton->setObjectName("SuspendButtonFrame");
    m_btnsList->append(m_suspendButton);
    m_lockButton = new RoundItemButton(tr("Lock"));
    m_lockButton->setObjectName("LockButtonFrame");
    m_btnsList->append(m_lockButton);
//    m_userSwitchButton = new RoundItemButton("SwitchUser", "UserSwitchButton");
//    m_userSwitchButton->setObjectName("UserSwitchButtonFrame");
    m_logoutButton = new RoundItemButton(tr("Log out"));
    m_logoutButton->setObjectName("LogoutButtonFrame");
    m_btnsList->append(m_logoutButton);

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

    updateStyle(":/skin/shutdown.qss", this);
//    QFile qssFile(":/skin/shutdown.qss");
//    QString qss;
//    qssFile.open(QFile::ReadOnly);
//    if(qssFile.isOpen())
//    {
//        qss = QLatin1String(qssFile.readAll());
//        this->setStyleSheet(qss);
//        qssFile.close();
//    }
}

//void ShutDownFrame::setButtonGroupMutex(QString buttonId) {
//    Q_UNUSED(buttonId);
//}
//void ShutDownFrame::test(QString test) {
//    qDebug() << "hover" << test;
//}
//void ShutDownFrame::ShutDownAction( ) {

//    QList<RoundItemButton*> m_children = this->findChildren<RoundItemButton *>();
//    for (int i = 0; i < m_children.length(); i++) {
//        if (m_children[i]->state() == RoundItemButton::Checked) {
//            emit m_children[i]->clicked();
////            emit m_children[i]->buttonAction(m_children[i]->buttonId());
//        }
//    }

//}
//void ShutDownFrame::updateStyleSheet(){
//    qDebug() << "testing global style update";
//    updateStyle(":/skin/shutdown.qss", this);
//}
/*void ShutDownFrame::setUserSwitchButton(bool showing) {
    if (showing) {
        m_userSwitchButton->show();
    } else {
        m_userSwitchButton->hide();
    }
}*/

void ShutDownFrame::setPreviousChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    const int nextPos = lastPos ? lastPos : m_btnsList->count();

    setCurrentSelectedBtn(m_btnsList->at(nextPos - 1));
//    QList<RoundItemButton*> m_children = this->findChildren<RoundItemButton *>();
//    m_count--;
//    if (m_count <= -1) {
//        m_count = m_children.length()-1;
//    }
//    emit signalManager->setButtonCheck(m_children[m_count]->objectName());
}

void ShutDownFrame::setNextChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    setCurrentSelectedBtn(m_btnsList->at((lastPos + 1) % m_btnsList->count()));

//    QList<RoundItemButton*> m_children = this->findChildren<RoundItemButton *>();
//    m_count++;
//    if (m_count >= m_children.length()) {
//        m_count = 0;
//    }
//    m_children[m_count]->setState(RoundItemButton::Checked);
//    emit signalManager->setButtonCheck(m_children[m_count]->objectName());
}

void ShutDownFrame::setCurrentSelectedBtn(RoundItemButton *btn)
{
    // toggle last btn state
    m_currentSelectedBtn->setState(RoundItemButton::Normal);
    btn->setState(RoundItemButton::Checked);
    m_currentSelectedBtn = btn;
}
