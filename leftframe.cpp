#include "leftframe.h"

LeftFrame::LeftFrame(int mode,QWidget *parent)
    : QFrame(parent)
{
    m_LogoWidget = new LogoWidget(this);
    m_LogoWidget->setFixedSize(180,80);
    setObjectName("LeftFrame");
    m_TimeWidget = new TimeWidget(this);
    m_TimeWidget->setFixedSize(180, 120);
    m_Layout = new QVBoxLayout;
    m_Layout->setContentsMargins(0,700,0, 0);
    m_Layout->addWidget(m_LogoWidget);
    m_Layout->addWidget(m_TimeWidget);


    setViewMode(mode);
    setLayout(m_Layout);
}
LeftFrame::~LeftFrame() {
}
void LeftFrame::setViewMode(int mode) {
    if (mode == 0) {
        m_LogoWidget->show();
        m_TimeWidget->hide();
        setBottomWidget(m_LogoWidget);
    } else if (mode == 1) {
        m_LogoWidget->hide();
        m_TimeWidget->show();
        setBottomWidget(m_TimeWidget);
    } else {
        m_LogoWidget->hide();
        m_TimeWidget->hide();
    }
}
void LeftFrame::setBottomWidget(QWidget *bottomWidget) {
    QLayoutItem *child;
    while ((child = m_Layout->takeAt(0)) != 0) {
        m_Layout->removeItem(child);
    }
    m_Layout->addWidget(bottomWidget);
}
