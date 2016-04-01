/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <DLog>

#include "lockframe.h"
#include "dbus/dbuslockfrontservice.h"
#include "dbus/dbuslockfront.h"
#include "dbus/dbuscontrolcenter.h"

#include <unistd.h>
#include <QLabel>
#include <dapplication.h>
#include <QDBusInterface>

DUTIL_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char* argv[])
{
    DApplication app(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("dde-lock");
    qApp->setApplicationVersion("2015.1.0");
    DLogManager::registerConsoleAppender();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid())
        DCCInter->HideImmediately();
    DCCInter->deleteLater();
    const bool quit = !app.setSingleInstance(QString("dde-lock_%1").arg(getuid()));

    QCommandLineParser cmdParser;
    cmdParser.addHelpOption();

    cmdParser.addVersionOption();


    QCommandLineOption backend(QStringList() << "d" << "daemon", "starting mode");
    cmdParser.addOption(backend);
    cmdParser.process(app);

    QString cmdOption;
    if (cmdParser.optionNames().length()>0) {
         cmdOption = cmdParser.optionNames().at(0);
    }

    LockFrame lockFrame;
    DBusLockFrontService service(&lockFrame);
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService("com.deepin.dde.lockFront") ||
        !conn.registerObject("/com/deepin/dde/lockFront", &lockFrame))
    {
        qDebug() << "register failed";
        if (!cmdParser.isSet(backend))
        {
            const char* interface = "com.deepin.dde.lockFront";
            QDBusInterface ifc(DBUS_NAME, DBUS_PATH, interface, QDBusConnection::sessionBus(), NULL);
            ifc.asyncCall("Show");

	    return 0;
        }
    }

    if (cmdOption == "d" || cmdOption == "daemon")
    {
        lockFrame.hide();
    } else {
        lockFrame.show();
    }
    app.exec();

}
