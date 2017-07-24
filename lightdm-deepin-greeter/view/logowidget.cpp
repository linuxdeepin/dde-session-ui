/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QTextCodec>
#include <QPalette>
#include <QDebug>
#include <QSettings>

#include "logowidget.h"

const QPixmap systemLogo()
{
    const QSettings settings("/etc/deepin-installer.conf", QSettings::IniFormat);
    const QString logo_path = settings.value("system_info_vendor_logo").toString();
    const QPixmap oem_logo(logo_path);

    if (oem_logo.isNull())
        return QPixmap(":img/logo_text.png");
    else
        return oem_logo;
}

LogoWidget::LogoWidget(QWidget* parent)
    : QFrame(parent)
{
    initUI();
}

void LogoWidget::initUI() {
    setFixedSize(240, 40);

    m_logoLabel = new QLabel();
    m_logoLabel->setPixmap(systemLogo());
    m_logoLabel->setObjectName("Logo");
    m_logoLabel->setFixedSize(150, 38);

    m_logoVersionLabel = new QLabel;
    m_logoVersionLabel->setObjectName("LogoVersion");
#ifdef SHENWEI_PLATFORM
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    m_logoVersionLabel->setPalette(pe);
#endif
//    m_logoVersionLabel->setFixedWidth(30);
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
