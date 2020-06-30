/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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
#include <QDebug>
#include <QTranslator>
#include "pincodedialog.h"


DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

const int PingCode = 1;
const int DevicePath = 2;
const int PingTime = 3;

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.loadTranslator();
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-bluetooth-dialog");
    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);


    QStringList arguslist = app.arguments();
    if (arguslist.size() < 4) {
        qDebug() << "number of parameters must be greater than 3";
        return -1;
    }
    qDebug() << "PingCode:" << arguslist[PingCode] << " Device Path:" << arguslist[DevicePath] << "Ping Time:" + arguslist[PingTime];

    dcc::bluetooth::PinCodeDialog dialog(arguslist[PingCode], arguslist[DevicePath], arguslist[PingTime], true);
    if (!dialog.isVisible()) {
        dialog.show();
    }
    return app.exec();
}

