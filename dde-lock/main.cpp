/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "lockframe.h"
#include "logmanager.h"
#include "dbus/dbuscontrolcenter.h"

#include <QLabel>
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("dde-lock");
    qApp->setApplicationVersion("2015.1.0");
    LogManager::instance()->debug_log_console_on();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    // hide dde-control-center
    DBusControlCenter *DCCInter = new DBusControlCenter;
    if (DCCInter->isValid())
        DCCInter->HideImmediately();
    DCCInter->deleteLater();

    if(QDBusConnection::sessionBus().registerService(DBUS_NAME)){
        qDebug() << "DBUS_NAME:" << DBUS_NAME;
        const QString &backgroundUrl = ":/theme/background/default_background.jpg";

        LockFrame lf(backgroundUrl);
        lf.show();
        lf.grabKeyboard();

        return app.exec();
       }
       else {
           qWarning() << "lockFront is running...";
           return 0;
       }

}
