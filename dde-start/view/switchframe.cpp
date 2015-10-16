#include <QDebug>
#include "switchframe.h"

SwitchFrame::SwitchFrame(QWidget *parent)
    : QFrame(parent)
{
    this->setFixedSize(300, 200);
    m_switchSystemButton = new QPushButton;
    m_switchSystemButton->setFixedSize(60, 60);
    m_switchSystemButton->setObjectName("SwitchDEnvironment");
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
    m_Layout->addWidget(m_switchSystemButton);
    m_Layout->addWidget(m_switchUserButton);
    m_Layout->addWidget(m_powerButton);
    m_Layout->addStretch();
    setLayout(m_Layout);

    initConnect();
}

void SwitchFrame::testing(QString switchId) {

    qDebug() << "SwitchId:" << switchId;

}

void SwitchFrame::initConnect() {
    connect(m_switchUserButton, SIGNAL(clicked()), this, SLOT(switchUser()));
    connect(m_powerButton, SIGNAL(clicked()), this, SLOT(switchPowerMenu()));
    connect(m_switchSystemButton, SIGNAL(clicked()), this, SLOT(switchDesktopEnvironment()));
}

void SwitchFrame::switchDesktopEnvironment() {
    emit switchItem(m_switchSystemButton->objectName());
}

void SwitchFrame::switchPowerMenu() {
    emit switchItem(m_powerButton->objectName());
}

void SwitchFrame::switchUser() {
    emit switchItem(m_switchUserButton->objectName());
}

SwitchFrame::~SwitchFrame()
{

}
