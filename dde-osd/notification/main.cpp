/*
 * Copyright (C) 2014 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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

#include "bubblemanager.h"
#include "notifications_dbus_adaptor.h"

#include <DLog>
#include <DApplication>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

#define APP_NAME "deepin-notifications"

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);
    app.setTheme("light");
    app.setOrganizationName("deepin");
    app.setApplicationName(APP_NAME);

    if (app.setSingleInstance(APP_NAME, DApplication::UserScope)) {
        DLogManager::registerConsoleAppender();
        DLogManager::registerFileAppender();

        BubbleManager manager;

        DDENotifyDBus ddenotify(&manager);
        NotificationsDBusAdaptor adapter(&manager);

        Q_UNUSED(ddenotify)
        Q_UNUSED(adapter)

        return app.exec();
    } else {
        qWarning() << "An instance has already existed!";
    }

    return 0;
}
