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

static double get_scale_ratio() {
    Display *display = XOpenDisplay(nullptr);

    XRRScreenResources *resources = XRRGetScreenResourcesCurrent(display, DefaultRootWindow(display));
    double scaleRatio = 0.0;

    if (!resources) {
        resources = XRRGetScreenResources(display, DefaultRootWindow(display));
        qWarning() << "get XRRGetScreenResourcesCurrent failed, use XRRGetScreenResources.";
    }

    if (resources) {
        for (int i = 0; i < resources->noutput; i++) {
            XRROutputInfo* outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
            if (outputInfo->crtc == 0 || outputInfo->mm_width == 0) continue;

            XRRCrtcInfo *crtInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
            if (crtInfo == nullptr) continue;

            scaleRatio = static_cast<double>(crtInfo->width) / static_cast<double>(outputInfo->mm_width) / (1366.0 / 310.0);

            if (scaleRatio > 1 + 2.0 / 3.0) {
                scaleRatio = 2;
            }
            else if (scaleRatio > 1 + 1.0 / 3.0) {
                scaleRatio = 1.5;
            }
            else {
                scaleRatio = 1;
            }
        }
    }
    else {
        qWarning() << "get scale radio failed, please check X11 Extension.";
    }

    return scaleRatio;
}

static void set_auto_QT_SCALE_FACTOR() {
    const double ratio = get_scale_ratio();
    if (ratio > 0.0) {
        setenv("QT_SCALE_FACTOR", QByteArray::number(ratio).constData(), 1);
    }

    if (!qEnvironmentVariableIsSet("QT_SCALE_FACTOR")) {
        setenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1", 1);
    }
}

int main(int argc, char *argv[])
{
    if (QString(getenv("XDG_CURRENT_DESKTOP")) != QStringLiteral("Deepin")) {
        qDebug() << "I only run the Deepin Desktop!";
        return -1;
    }

    // 加载dpi设置防止应用启动后字体大小不正常
    if (!QFile::exists("/etc/lightdm/deepin/qt-theme.ini")) {
        set_auto_QT_SCALE_FACTOR();
    }
    else {
        DApplication::customQtThemeConfigPath("/etc/lightdm/");
    }
    if (!qgetenv("WAYLAND_DISPLAY").isEmpty()) {
        qputenv("QT_WAYLAND_SHELL_INTEGRATION", "kwayland-shell");
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
