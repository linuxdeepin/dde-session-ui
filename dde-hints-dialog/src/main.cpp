/*
 * Copyright (C) 2020 ~ 2022 Uniontech Technology Co., Ltd.
 *
 * Author:     chenjun <chenjun@uniontech.com>
 *
 * Maintainer: chenjun <chenjun@uniontech.com>
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

#include "hintsdialog.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DGuiApplicationHelper>

#include <QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>

DWIDGET_USE_NAMESPACE

const int HINT_TITLE = 1;
const int HINT_CONTENT = 2;

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setApplicationName("dde-hints-dialog");
    a.setApplicationVersion("1.0");
    a.setQuitOnLastWindowClosed(true);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QStringList arguslist = a.arguments();
    if (arguslist.size() < 3) {
        qDebug() << "number of parameters must be greater than 1";
        return -1;
    }
    qDebug() << "Title:" << arguslist[HINT_TITLE] << " Content:" << arguslist[HINT_CONTENT];

    HintsDialog dialog;
    dialog.setHintTitle(arguslist[HINT_TITLE]);
    dialog.setHintContent(arguslist[HINT_CONTENT]);
    dialog.moveToCenter();
    dialog.show();

    return a.exec();
}
