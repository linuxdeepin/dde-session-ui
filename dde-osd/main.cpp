/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QTranslator>

#include <DApplication>
#include <QFile>
#include <QDBusConnection>
#include <QProcess>

#include "manager.h"
#include "kblayoutindicator.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setApplicationName("dde-osd");
    a.setApplicationVersion("1.0");

    a.setTheme("light");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QFile file(":/themes/light.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    QStringList args = a.arguments();
    QString action;
    if (args.length() > 1) {
        action = args.at(1);
    }

    if (!QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")) {
        if (!action.isEmpty()) {
            QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.osd / com.deepin.dde.osd.ShowOSD string:" + action);
        }

        qWarning() << "failed to register dbus service";
        return -1;
    }

    Manager m;
    QDBusConnection::sessionBus().registerObject("/", "com.deepin.dde.osd", &m, QDBusConnection::ExportAllSlots);

    if (!action.isEmpty()) {
        m.ShowOSD(action);
    }

    KBLayoutIndicator indicator;
    indicator.show();

    return a.exec();
}
