#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QDebug>
#include <QSettings>

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

    m_logoVersionLabel = new QLabel;
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

    QString systemVersion = getVersion();
    m_logoVersionLabel->setText(systemVersion);
}

QString LogoWidget::getVersion() {
    QSettings settings("/etc/deepin-version", QSettings::IniFormat);
    QString item = "Addition";
    QString version = settings.value(item + "/Milestone").toString();
    qDebug() << "Deepin Version:" << version;
    return version;
}

LogoWidget::~LogoWidget()
{
}
