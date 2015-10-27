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
    connect(this, &ShutDownFrame::pressEnterAction, this, &ShutDownFrame::enterKeyPushed);

    connect(m_shutdownButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions("ShutDownButton");});
    connect(m_restartButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions("RestartButton");});
    connect(m_suspendButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions("SuspendButton");});
    connect(m_lockButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions("LockButton");});
    connect(m_logoutButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions("LogoutButton");});
}

void ShutDownFrame::enterKeyPushed()
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
    m_shutdownButton->setAutoExclusive(true);
    m_shutdownButton->updateState(RoundItemButton::Hover);
    m_shutdownButton->setObjectName("ShutDownButtonFrame");
    m_btnsList->append(m_shutdownButton);

    m_currentSelectedBtn = m_shutdownButton;
    m_restartButton = new RoundItemButton(tr("Restart"));
    m_restartButton->setAutoExclusive(true);
    m_restartButton->setObjectName("RestartButtonFrame");
    m_btnsList->append(m_restartButton);
    m_suspendButton = new RoundItemButton(tr("Suspend"));
    m_suspendButton->setAutoExclusive(true);
    m_suspendButton->setObjectName("SuspendButtonFrame");
    m_btnsList->append(m_suspendButton);
    m_lockButton = new RoundItemButton(tr("Lock"));
    m_lockButton->setAutoExclusive(true);
    m_lockButton->setObjectName("LockButtonFrame");
    m_btnsList->append(m_lockButton);
    m_logoutButton = new RoundItemButton(tr("Log out"));
    m_logoutButton->setAutoExclusive(true);
    m_logoutButton->setObjectName("LogoutButtonFrame");
    m_btnsList->append(m_logoutButton);

    m_buttonLayout = new QHBoxLayout;
    m_buttonLayout->setMargin(0);
    m_buttonLayout->setSpacing(10);
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_shutdownButton);
    m_buttonLayout->addWidget(m_restartButton);
    m_buttonLayout->addWidget(m_suspendButton);
    m_buttonLayout->addWidget(m_lockButton);
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
}

void ShutDownFrame::setPreviousChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    const int nextPos = lastPos ? lastPos : m_btnsList->count();

    m_currentSelectedBtn = m_btnsList->at(nextPos - 1);
    m_currentSelectedBtn->setChecked(true);

}

void ShutDownFrame::setNextChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    m_currentSelectedBtn = m_btnsList->at((lastPos + 1) % m_btnsList->count());
    m_currentSelectedBtn->setChecked(true);
}
