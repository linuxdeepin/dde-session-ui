#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QTextCodec>
#include <QDebug>
#include <QSettings>

#include "logowidget.h"
LogoWidget::LogoWidget(QWidget* parent)
    : QFrame(parent)
{
    initUI();
}

void LogoWidget::initUI() {
    setFixedSize(240, 40);

    m_logoLabel = new QLabel();
    m_logoLabel->setObjectName("Logo");
    m_logoLabel->setFixedSize(150, 38);

    m_logoVersionLabel = new QLabel;
    m_logoVersionLabel->setObjectName("LogoVersion");

    this->setObjectName("LogoWidget");

    m_logoRightSideLayout = new QVBoxLayout;
    m_logoRightSideLayout->setMargin(0);
    m_logoRightSideLayout->setSpacing(0);
    m_logoRightSideLayout->addWidget(m_logoVersionLabel);
    m_logoRightSideLayout->addStretch();

    m_logoLayout = new QHBoxLayout;
    m_logoLayout->setMargin(0);
    m_logoLayout->setSpacing(0);
    m_logoLayout->addWidget(m_logoLabel);
    m_logoLayout->addLayout(m_logoRightSideLayout);
    m_logoLayout->addStretch();
    setLayout(m_logoLayout);

    QString systemVersion = getVersion();
    m_logoVersionLabel->setText(systemVersion);
}

QString LogoWidget::getVersion() {
    QSettings settings("/etc/deepin-version", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("utf8"));
    QString item = "Release";
    ///////////system version
    QString version = settings.value(item + "/Version").toString();
    //////////system type
    QString localKey =QString("%1/Type[%2]").arg(item).arg(QLocale::system().name());
    QString finalKey =QString("%1/Type").arg(item);

    QString type = settings.value(localKey, settings.value(finalKey)).toString();
    //////////system release version
    QString milestone = settings.value("Addition/Milestone").toString();

    qDebug() << "Deepin Version:" << version << type;

    QString versionNumber = version + " " + type + " " + milestone;
    QFont versionFont; QFontMetrics fm(versionFont);
    int width=fm.width(versionNumber);
    setFixedSize(150+width, 40);
    return versionNumber;
}

LogoWidget::~LogoWidget()
{
}
