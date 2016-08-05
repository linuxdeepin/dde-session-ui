/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QApplication>
#include <QtCore/QTranslator>
#include "app/loginframe.h"
#include <QLabel>
#include <QProcess>
#include <QThread>

#include <DLog>

DUTIL_USE_NAMESPACE

bool setAllMonitorsExtend()
{
    QProcess *checkMons = new QProcess;
    checkMons->start("xrandr");
    checkMons->waitForFinished(1000);

    QString primaryMonitor;
    QStringList otherMonitors;

    const QString result = checkMons->readAllStandardOutput();
    const QStringList &infoList = result.split('\n');
    bool foundMonitor = false;
    for (const QString &info : infoList)
    {
        const QStringList details = info.split(' ');

        if (details.count() < 3 || details.at(1) != "connected")
            continue;

        qDebug() << "info: " << info;
        qDebug() << "found monitor: " << details.first();
        foundMonitor = true;
        if (details.at(2) == "primary")
            primaryMonitor = details.first();
        else
            otherMonitors.append(details.first());
    }

    if (!foundMonitor) {
        qCritical() << "can not find any monitor" << "retray in 15 second...";
        return foundMonitor;
    }

    // set other monitors
    QString lastMonitor = primaryMonitor;
    if (lastMonitor.isEmpty())
        lastMonitor = otherMonitors.first();

    // call enable xrandr first
    QProcess enableMonitor;
    enableMonitor.start("xrandr --auto");
    bool ret = enableMonitor.waitForFinished(-1);
    qDebug()<< "enable monitor" <<ret<<enableMonitor.readAll();

    for (const QString &m : otherMonitors)
    {
        if (m == lastMonitor)
            continue;

        QProcess *setting = new QProcess;
        QString cmd = QString("xrandr --output %1 --right-of %2 --auto").arg(m).arg(lastMonitor);

        qDebug() << "exec: " << cmd;

        setting->start(cmd);
        bool result = setting->waitForFinished(1000);
        qDebug() << "finished: " << result;
        setting->deleteLater();
    }

    checkMons->deleteLater();
    return foundMonitor;
}

// This workaround is work when you don not plug in any monitor and
// power on computer. Then Qt can not draw out any thing.
// You can not quit application because lightdm will quit with greeter quit.
// And you can not recive and signal of monitor plugin from kernel.
// So please dectect time by time, :<
void waitMonitorReady() {
    while(!setAllMonitorsExtend()) {
        QThread::sleep(10);
    }
}

int main(int argc, char* argv[])
{
    waitMonitorReady();

    QApplication a(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("lightdm-deepin-greeter");
    qApp->setApplicationVersion("2015.1.0");

    DLogManager::registerConsoleAppender();

    /* the logmanager is used to output debug log */
    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    LoginFrame w;
    w.show();
    w.grabKeyboard();

    return a.exec();
}
