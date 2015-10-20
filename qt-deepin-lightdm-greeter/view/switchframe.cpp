#include <QDebug>
#include "switchframe.h"

SwitchFrame::SwitchFrame(QWidget* parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

SwitchFrame::~SwitchFrame()
{
}

void SwitchFrame::initConnect() {
    connect(m_switchUserButton, &QPushButton::clicked, this, &SwitchFrame::triggerSwitchUser);
    connect(m_powerButton, &QPushButton::clicked, this, &SwitchFrame::triggerPower);
    connect(m_switchSessionButton, &QPushButton::clicked, this, &SwitchFrame::triggerSwitchSession);
}

void SwitchFrame::initUI() {
    setFixedSize(300, 200);

    m_switchSessionButton = new QPushButton;
    m_switchSessionButton->setFixedSize(60, 60);
    m_switchSessionButton->setObjectName("SwitchDEnvironment");
    m_switchUserButton = new QPushButton;
    m_switchUserButton->setFixedSize(100, 100);
    m_switchUserButton->setObjectName("SwitchUser");
    m_powerButton = new QPushButton;
    m_powerButton->setFixedSize(100, 100);
    m_powerButton->setObjectName("PowerMenu");

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addWidget(m_switchSessionButton);
    m_Layout->addWidget(m_switchUserButton);
    m_Layout->addWidget(m_powerButton);
    m_Layout->addStretch();
    setLayout(m_Layout);
}
