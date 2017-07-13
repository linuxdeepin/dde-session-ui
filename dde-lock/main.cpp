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

int main(int argc, char *argv[])
{
    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-lock");
    app.setApplicationVersion("2015.1.0");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    if (!app.setSingleInstance(QString("dde-lock_%1").arg(getuid())))
        return -1;

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid()) {
        DCCInter->HideImmediately();
    }
    DCCInter->deleteLater();

    QCommandLineParser cmdParser;
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();

    QCommandLineOption backend(QStringList() << "d" << "daemon", "starting mode");
    cmdParser.addOption(backend);
    QCommandLineOption switchUser(QStringList() << "s" << "switch", "show user switch");
    cmdParser.addOption(switchUser);
    cmdParser.process(app);

    bool runDaemon = cmdParser.isSet(backend);
    bool showUserList = cmdParser.isSet(switchUser);

    LockFrame lockFrame;
    DBusLockFrontService service(&lockFrame);
    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(DBUS_NAME) ||
            !conn.registerObject("/com/deepin/dde/lockFront", &lockFrame)) {
        qDebug() << "register dbus failed"<< "maybe lockFront is running..." << conn.lastError();

        if (!runDaemon) {
            const char *interface = "com.deepin.dde.lockFront";
            QDBusInterface ifc(DBUS_NAME, DBUS_PATH, interface, QDBusConnection::sessionBus(), NULL);
            if (showUserList) {
                ifc.asyncCall("ShowUserList");
            } else {
                ifc.asyncCall("Show");
            }
        }
    } else {
        if (!runDaemon) {
            if (showUserList) {
                lockFrame.showUserList();
            } else {
                lockFrame.show();
            }
        } else {
            lockFrame.hide();
        }
        app.exec();
    }

    return 0;
}

