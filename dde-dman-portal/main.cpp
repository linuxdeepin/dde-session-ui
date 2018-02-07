/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#include <QCoreApplication>

#include <QDebug>
#include <QDBusConnection>
#include <QDBusError>

#include "dmanproxy.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    DManProxy dmp;

    auto conn = QDBusConnection::sessionBus();

    if (!conn.registerService(SERVICE_NAME)) {
        qDebug() << "registerService failed:" << conn.lastError();
        return EXIT_FAILURE;
    }
    if (!conn.registerObject(SERVICE_PATH, &dmp, QDBusConnection::ExportAllSlots)) {
        qDebug() << "registerObject failed:" << conn.lastError();
        return EXIT_FAILURE;
    }

    return app.exec();
}
