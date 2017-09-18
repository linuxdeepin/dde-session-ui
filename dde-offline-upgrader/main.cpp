/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#include <DApplication>
#include <DLog>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "frame.h"
#include "dialog.h"
#include "dbus/dbusupdatejobmanager.h"

DWIDGET_USE_NAMESPACE

DCORE_USE_NAMESPACE

static bool DownloadedPackagesAvailable() {
    QDBusInterface scheduler("com.deepin.LastoreSessionHelper",
                             "/com/deepin/LastoreSessionHelper",
                             "com.deepin.LastoreSessionHelper",
                             QDBusConnection::sessionBus());
    QDBusMessage msg = scheduler.call("CheckPrepareDistUpgradeJob");
    QVariantList args = msg.arguments();
    if (args.length() > 1) {
        return args.at(0).toBool();
    }

    return false;
}

static void ScheduleUpgrade(uint secs) {
    QDBusInterface scheduler("com.deepin.LastoreSessionHelper",
                             "/com/deepin/LastoreSessionHelper",
                             "com.deepin.LastoreSessionHelper",
                             QDBusConnection::sessionBus());
    scheduler.call("LaterUpgrade", secs);
}

static bool Upgrading = false;
static void UpgradeRemindMeLater() {
    if (Upgrading) return;
    ScheduleUpgrade(2 * 60 * 60);
    qApp->quit();
}

static void UpgradeNotNow() {
    if (Upgrading) return;
    ScheduleUpgrade(24 * 60 * 60);
    qApp->quit();
}

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-offline-upgrader");
    a.setApplicationVersion("0.9");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption noAskOption(QStringList() << "s" << "silent",
                                   "don't show the option dialog, upgrade directly.");
    parser.addOption(noAskOption);

    parser.process(a);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    if (!QDBusConnection::sessionBus().registerService("com.deepin.dde.OfflineUpgrader")) {
        qDebug() << "Another process has taken our service name.";

        if (!a.setSingleInstance(a.applicationName())) {
            qDebug() << "Another instance is running, raise it's window.";
            QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.OfflineUpgrader "
                                    "/ "
                                    "com.deepin.dde.OfflineUpgrader.RaiseWindow");
        }

        return 0;
    }

    // Double check if there're upgradable packages available before the
    // GUI show.
    if (!DownloadedPackagesAvailable()) {
        qDebug() << "No downloaded packages available.";
        return 0;
    }

    if (parser.isSet(noAskOption)) {
        Frame * f = new Frame;
        f->showFullScreen();
        f->distUpgrade();
    } else {
        Dialog * d = new Dialog;
        d->show();

        QDBusConnection::sessionBus().registerObject("/", d);

        bool buttonClicked = false;
        QObject::connect(d, &Dialog::buttonClicked, [&buttonClicked](int index, const QString &){
            buttonClicked = true;

            // Triple check if there're upgradable packages available before the actual
            // actions taken.
            if (!DownloadedPackagesAvailable()) {
                qDebug() << "No downloaded packages available.";
                return 0;
            }

            Frame * f = new Frame;
            switch (index) {
            case 0:
                // remind later
                UpgradeRemindMeLater();
                break;
            case 1:
                // not now.
                UpgradeNotNow();
                break;
            case 2:
                Upgrading = true;
                f->showFullScreen();
                f->distUpgrade();
                break;
            default:
                break;
            }
        });

        QObject::connect(d, &Dialog::aboutToClose, [&buttonClicked]{
            if (!buttonClicked) {
                // remind later;
                UpgradeRemindMeLater();
            }
        });
    }

    return a.exec();
}
