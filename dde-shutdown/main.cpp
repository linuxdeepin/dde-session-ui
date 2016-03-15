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
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>

#include <DLog>

#include "app/shutdownframe.h"
#include "dbus/dbuscontrolcenter.h"
#include "backgroundlabel.h"

DUTIL_USE_NAMESPACE

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("dde-shutdown");
    qApp->setApplicationVersion("2015.1.0");
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid())
        DCCInter->HideImmediately();
    DCCInter->deleteLater();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    QCommandLineOption hide(QStringList() << "H" << "Hide", "hide buttons", "btns");
    QCommandLineOption disable(QStringList() << "D" << "Disable", "disable buttons", "btns");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(hide);
    parser.addOption(disable);
    parser.process(app);

    qDebug() << parser.values(hide);
    qDebug() << parser.values(disable);

    if(QDBusConnection::sessionBus().registerService(DBUS_NAME)){
        qDebug() << "DBUS_NAME:" << DBUS_NAME;

        ShutdownFrame w;
    //    w.hideBtns(parser.values(hide));
    //    w.disableBtns(parser.values(disable));
        w.show();

        return app.exec();
       }
       else {
           qWarning() << "shutdownFront is running...";
           return 0;
       }

    return app.exec();
}
