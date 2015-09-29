#include "powermenuframe.h"

PowerMenuFrame::PowerMenuFrame(int mode, QWidget *parent)
    : QFrame(parent)
{
    this->setFixedWidth(180);
    m_mode = mode;
//    m_userSwitchButton = new QPushButton;
//    m_userSwitchButton->setFixedSize(60, 60);
//    m_userSwitchButton->setObjectName("UserSwitchButton");
//    m_powerMenuButton = new QPushButton;
//    m_powerMenuButton->setFixedSize(60, 60);
//    m_powerMenuButton->setObjectName("PowerMenu");

    m_ContentLayout = new QHBoxLayout;
    m_ContentLayout->setMargin(0);
    m_ContentLayout->setSpacing(10);
//    m_ContentLayout->addWidget(m_userSwitchButton);
//    m_ContentLayout->addWidget(m_powerMenuButton);
    m_ContentLayout->setSpacing(10);

    m_Layout = new QVBoxLayout;
    m_Layout->addStretch();
    m_Layout->addLayout(m_ContentLayout);

}
PowerMenuFrame::~PowerMenuFrame()
{}
void PowerMenuFrame::setMode(int mode) {
    m_mode = mode;
//    if (m_mode == 0) {
//        m_powerMenuButton->show();
//        m_userSwitchButton->show();
//    } else if (m_mode == 1) {
//        m_powerMenuButton->show();
//        m_userSwitchButton->show();
//    } else if (m_mode == 2) {
//        m_powerMenuButton->hide();
//        m_userSwitchButton->hide();
//    } else {
//        m_powerMenuButton->hide();
//        m_userSwitchButton->hide();
//    }
}
