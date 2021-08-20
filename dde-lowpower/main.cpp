/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "window.h"

#ifdef QT_DEBUG
#include <DAccessibilityChecker>
#endif

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QTranslator>
#include <QTimer>
#include <QDebug>

#ifdef QT_DEBUG
DWIDGET_USE_NAMESPACE
#endif

#define DBUS_SERV "com.deepin.dde.lowpower"
#define DBUS_PATH "/com/deepin/dde/lowpower"
#define DBUS_IFCE "com.deepin.dde.LowPower"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineOption raiseOption("raise", "raise the main window.");
    QCommandLineOption quitOption("quit", "quit all dde-lowpower instances.");

    // command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(raiseOption);
    parser.addOption(quitOption);
    parser.process(a);

    Window w;
    LowPowerAdaptor adaptor(&w);
    QDBusInterface iface(DBUS_SERV,
                         DBUS_PATH,
                         DBUS_IFCE,
                         QDBusConnection::sessionBus());

    bool result = QDBusConnection::sessionBus().registerService(DBUS_SERV);
    if (result) {
        if (parser.isSet(quitOption)) {
            return 0;
        } else {
            w.show();
            w.grabMouse();
            if (!w.SleepLock()) {
               w.grabKeyboard();
            }

            QDBusConnection::sessionBus().registerObject(DBUS_PATH, &w);
        }
    } else {
        if (parser.isSet(raiseOption)) {
            iface.call("Raise");
        } else if (parser.isSet(quitOption)) {
            iface.call("Quit");
        }

        QTimer::singleShot(500, &a, SLOT(quit()));
    }

#ifdef QT_DEBUG
    DAccessibilityChecker checker;
    checker.setOutputFormat(DAccessibilityChecker::FullFormat);
    checker.start();
#endif

    return a.exec();
}
