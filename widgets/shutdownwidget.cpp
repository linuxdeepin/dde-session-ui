#include "shutdownwidget.h"

ShutdownWidget::ShutdownWidget(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

void ShutdownWidget::initConnect() {

    connect(m_requireShutdownButton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireShutdown);});
    connect(m_requireRestartButton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireRestart);});
    connect(m_requireSuspendBUtton, &RoundItemButton::clicked, [this]{ emit shutDownWidgetAction(RequireSuspend);});

}

void ShutdownWidget::initUI() {
    m_requireShutdownButton = new RoundItemButton(tr("Shut down"), this);
    m_requireShutdownButton->setObjectName("RequireShutdownButton");
    m_requireShutdownButton->setAutoExclusive(true);

    m_requireRestartButton = new RoundItemButton(tr("Restart"), this);
    m_requireRestartButton->setObjectName("RequireRestartButton");
    m_requireRestartButton->setAutoExclusive(true);

    m_requireSuspendBUtton = new RoundItemButton(tr("Suspend"), this);
    m_requireSuspendBUtton->setObjectName("RequireSuspendButton");
    m_requireSuspendBUtton->setAutoExclusive(true);

    m_btnList = new QList<RoundItemButton*>;
    m_btnList->append(m_requireShutdownButton);
    m_btnList->append(m_requireRestartButton);
    m_btnList->append(m_requireSuspendBUtton);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(10);
    m_Layout->addStretch(0);
    m_Layout->addWidget(m_requireShutdownButton);
    m_Layout->addWidget(m_requireRestartButton);
    m_Layout->addWidget(m_requireSuspendBUtton);
    m_Layout->addStretch(0);
    setLayout(m_Layout);

    updateStyle(":/skin/requireshutdown.qss", this);
}

ShutdownWidget::~ShutdownWidget() {

}
