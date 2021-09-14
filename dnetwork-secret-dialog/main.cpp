/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
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

#include "networksecretdialog.h"
#if (defined QT_DEBUG) && (defined CHECK_ACCESSIBLENAME)
#include "../common/accessibilitycheckerex.h"
#endif

#include <DLog>
#include <DApplication>

#include <QCommandLineOption>
#include <QCommandLineParser>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dnetwork-secret-dialog");
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);
    app.loadTranslator();

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("filepath",
                                 "the file of json format data.\n"
                                 "data will be read from STDIN if filepath is not specified.",
                                 "[filepath]");
    parser.process(app);
    const QStringList &posArguments = parser.positionalArguments();

    QJsonDocument jsonDoc;
    QFile file;
    if (posArguments.isEmpty()) {
        qDebug() << "read json data from STDIN";
        if (!file.open(stdin, QFile::ReadOnly)) {
            qDebug() << "read from STDIN failed";
            return -2;
        }
    } else {
        file.setFileName(posArguments.first());
        if (!file.open(QFile::ReadOnly)) {
            qDebug() << "file:" << file.fileName()<< "open failed";
            return -1;
        }
    }

    jsonDoc = QJsonDocument::fromJson(file.readAll());
    if (jsonDoc.isEmpty()) {
        qDebug() << "invalid json data.";
        return -3;
    }

    file.close();
    NetworkSecretDialog dialog(jsonDoc);
#if (defined QT_DEBUG) && (defined CHECK_ACCESSIBLENAME)
    AccessibilityCheckerEx checker;
    checker.setOutputFormat(DAccessibilityChecker::FullFormat);
    checker.start();
#endif
    dialog.exec();

    app.exit(0);
}
