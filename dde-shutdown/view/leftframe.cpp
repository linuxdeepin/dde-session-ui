#include <QDebug>
#include "leftframe.h"

LeftFrame::LeftFrame(int mode,QWidget *parent)
    : QFrame(parent)
{
    m_mode = mode;
    setFixedWidth(180);
    m_LogoWidget = new LogoWidget(this);
    m_LogoWidget->setFixedSize(180, 80);

    m_TimeWidget = new TimeWidget(this); 
    m_TimeWidget->setObjectName("TimeWidget");
    m_TimeWidget->setFixedSize(180, 140);

    setMode(mode);

    m_Layout = new QVBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addWidget(m_LogoWidget);
    m_Layout->addWidget(m_TimeWidget);

    setLayout(m_Layout);
}
LeftFrame::~LeftFrame() {
}
void LeftFrame::setMode(int mode) {
    m_mode = mode;
    if (m_mode == 0) {
        m_LogoWidget->show();
        m_TimeWidget->hide();
    } else if (m_mode == 1) {
        m_LogoWidget->hide();
        m_TimeWidget->show();

    } else if (m_mode == 2) {
        m_LogoWidget->hide();
        m_TimeWidget->hide();
    } else {
        m_LogoWidget->hide();
        m_TimeWidget->hide();
    }
}
