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

#include "warningdialog.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>

#include <QTranslator>
#include <QDBusConnection>
#include <QDBusInterface>

DWIDGET_USE_NAMESPACE

static const QString Service = "com.deepin.dde.WarningDialog";
static const QString Path = "/com/deepin/dde/WarningDialog";
static const QString Interface = "com.deepin.dde.WarningDialog";

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setApplicationName("dde-warning-dialog");
    a.setApplicationVersion("1.0");
    a.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    DGuiApplicationHelper::setSingleInstanceInterval(-1);
    if (!a.setSingleInstance(a.applicationName(), DApplication::UserScope)) {
        return -1;
    }

    WarningDialog w;
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService(Service);
    connection.registerObject(Path, &w);

    moveToCenter(&w);
    w.show();

    return a.exec();
}
