/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
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

    if (!QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")
            || !a.setSingleInstance(QString("dde-osd"), DApplication::UserScope)) {
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
