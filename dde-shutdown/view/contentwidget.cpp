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

    connect(m_shutdownButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Shutdown);});
    connect(m_restartButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Restart);});
    connect(m_suspendButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Suspend);});
    connect(m_lockButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Lock);});
    connect(m_logoutButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Logout);});
}

void ShutDownFrame::enterKeyPushed()
{
    if (m_currentSelectedBtn == m_shutdownButton)
        emit ShutDownFrameActions(Shutdown);
    else if (m_currentSelectedBtn == m_restartButton)
        emit ShutDownFrameActions(Restart);
    else if (m_currentSelectedBtn == m_suspendButton)
        emit ShutDownFrameActions(Suspend);
    else if (m_currentSelectedBtn == m_lockButton)
        emit ShutDownFrameActions(Lock);
    else if (m_currentSelectedBtn == m_logoutButton)
        emit ShutDownFrameActions(Logout);
}

void ShutDownFrame::initUI() {
    m_btnsList = new QList<RoundItemButton *>;
    m_shutdownButton = new RoundItemButton(tr("Shut down"));
    m_shutdownButton->setAutoExclusive(true);
    m_shutdownButton->updateState(RoundItemButton::Checked);
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
    m_switchUserBtn = new RoundItemButton(tr("Switch user"));
    m_switchUserBtn->setAutoExclusive(true);
    m_switchUserBtn->setObjectName("SwitchUserBtn");
    m_btnsList->append(m_switchUserBtn);

    QLabel *tipsIcon = new QLabel;
    tipsIcon->setPixmap(QPixmap(":/img/waring.png"));
    m_tipsLabel = new QLabel;
    m_tipsLabel->setAlignment(Qt::AlignCenter);
    m_tipsLabel->setStyleSheet("color:white;");
    QHBoxLayout *tipsLayout = new QHBoxLayout;
    tipsLayout->addStretch();
    tipsLayout->addWidget(tipsIcon);
    tipsLayout->addWidget(m_tipsLabel);
    tipsLayout->addStretch();

    m_tipsWidget = new QWidget;
    m_tipsWidget->hide();
    m_tipsWidget->setLayout(tipsLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_shutdownButton);
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addWidget(m_suspendButton);
    buttonLayout->addWidget(m_lockButton);
    buttonLayout->addWidget(m_logoutButton);
    buttonLayout->addWidget(m_switchUserBtn);
    buttonLayout->addStretch(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_tipsWidget);
    mainLayout->addStretch();
    setFocusPolicy(Qt::StrongFocus);
    setLayout(mainLayout);

    updateStyle(":/skin/shutdown.qss", this);

    // TODO: remove
    showTips("不要关机 不要关机 不要关机 不要关机 不要关机 不要关机 ");
    m_logoutButton->setDisabled(true);
}

void ShutDownFrame::setPreviousChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    const int nextPos = lastPos ? lastPos : m_btnsList->count();

    m_currentSelectedBtn = m_btnsList->at(nextPos - 1);

    if (m_currentSelectedBtn->isDisabled())
        setPreviousChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ShutDownFrame::setNextChildFocus()
{
    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    m_currentSelectedBtn = m_btnsList->at((lastPos + 1) % m_btnsList->count());

    if (m_currentSelectedBtn->isDisabled())
        setNextChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ShutDownFrame::showTips(const QString &tips)
{
    m_tipsLabel->setText(tips);
    m_tipsWidget->show();
}
