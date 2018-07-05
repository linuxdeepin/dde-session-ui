/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#include "mainwidget.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    if (QFile::exists(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + "/autostart/dde-first-run.desktop")) {
        QString currentVersion = GetSystemVersion().first;

        if (!currentVersion.isEmpty()) {
            QSettings welcomeSetting("deepin", "dde-welcome");
            QString version = welcomeSetting.value("Version").toString();
            if (version.isEmpty()) {
                welcomeSetting.setValue("Version", currentVersion);
            }
        }
        return 0;
    }

    if (CheckVersionChanged()) {
        DApplication::loadDXcbPlugin();
        DApplication app(argc, argv);
        app.setOrganizationName("deepin");
        app.setApplicationName("dde-welcome");

        if (!app.setSingleInstance(app.applicationName()))
            return -1;

        DLogManager::registerConsoleAppender();
        DLogManager::registerFileAppender();

        QTranslator translator;
        translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
        app.installTranslator(&translator);

        MainWidget w;
        w.show();

        return app.exec();
    }
    return 0;
}
