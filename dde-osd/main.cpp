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

#ifndef UNIT_TEST

#include "notification/bubblemanager.h"
#include "notification/notifications_dbus_adaptor.h"
#include "manager.h"
#include "kblayoutindicator.h"
#include "accessible.h"
#include "notification/persistence.h"
#include "notification/notifysettings.h"

#include <DApplication>
#include <DDBusSender>
#include <DLog>
#include <DGuiApplicationHelper>

#include <QTranslator>
#include <QFile>
#include <QDBusConnection>
#include <QProcess>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    if (QString(getenv("XDG_CURRENT_DESKTOP")) != QStringLiteral("Deepin")) {
        qDebug() << "I only run the Deepin Desktop!";
        return -1;
    }

    DGuiApplicationHelper::setUseInactiveColorGroup(false);
    DGuiApplicationHelper::setColorCompositingEnabled(true);
    DApplication a(argc, argv);
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-osd");
    a.setApplicationVersion("1.0");

#if DTK_VERSION >= DTK_VERSION_CHECK(2, 0, 9, 0)
    a.setOOMScoreAdj(500);
#endif

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    QAccessible::installFactory(accessibleFactory);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QStringList args = a.arguments();
    QString action;
    if (args.length() > 1) {
        action = args.at(1);
    }

    // run osd
    Manager m;
    QDBusConnection connection = QDBusConnection::sessionBus();

    connection.interface()->registerService("com.deepin.dde.osd",
                                            QDBusConnectionInterface::ReplaceExistingService,
                                            QDBusConnectionInterface::AllowReplacement);

    connection.registerObject("/", "com.deepin.dde.osd", &m, QDBusConnection::ExportAllSlots);

    QObject::connect(connection.interface(), &QDBusConnectionInterface::serviceUnregistered, qApp, &QApplication::quit);

    if (!action.isEmpty()) {
        m.ShowOSD(action);
    }

    // run notification
    Persistence persistence;
    NotifySettings setting;
    BubbleManager manager(&persistence, &setting);

    DDENotifyDBus ddenotify(&manager);
    NotificationsDBusAdaptor adapter(&manager);
    return a.exec();
}
#else

#include "unittest.h"

QTEST_MAIN(UnitTest)

#endif
