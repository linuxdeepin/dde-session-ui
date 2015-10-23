#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QDebug>

#include "logowidget.h"
LogoWidget::LogoWidget(QWidget* parent)
    : QFrame(parent)
{
    initUI();
}

void LogoWidget::initUI() {
    setFixedSize(180, 40);
    m_logoLabel = new QLabel();
    m_logoLabel->setObjectName("Logo");
    m_logoLabel->setFixedSize(150, 38);

    m_logoVersionLabel = new QLabel("Alpha");
    m_logoVersionLabel->setObjectName("LogoVersion");
//    m_logoVersionLabel->setFixedWidth(30);

    this->setObjectName("LogoWidget");
    m_logoLeftSideLayout = new QVBoxLayout;
    m_logoLeftSideLayout->setMargin(0);
    m_logoLeftSideLayout->setSpacing(0);
    m_logoLeftSideLayout->addStretch();
    m_logoLeftSideLayout->addWidget(m_logoLabel);
    m_logoLeftSideLayout->addStretch();

    m_logoRightSideLayout = new QVBoxLayout;
    m_logoRightSideLayout->setMargin(0);
    m_logoRightSideLayout->setSpacing(0);
    m_logoRightSideLayout->addWidget(m_logoVersionLabel);
    m_logoRightSideLayout->addStretch();

    m_logoLayout = new QHBoxLayout;
    m_logoLayout->setMargin(0);
    m_logoLayout->setSpacing(0);
    m_logoLayout->addLayout(m_logoLeftSideLayout);
    m_logoLayout->addStretch();
    m_logoLayout->addLayout(m_logoRightSideLayout);
    setLayout(m_logoLayout);
}

LogoWidget::~LogoWidget()
{
}
