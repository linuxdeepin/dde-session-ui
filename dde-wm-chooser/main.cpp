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

#include "wmframe.h"
#include "propertygroup.h"
#include "multiscreenmanager.h"

#include <DApplication>
#include <DLog>
#include <DSysInfo>
#ifdef QT_DEBUG
#include <DAccessibilityChecker>
#endif

#include <QDesktopWidget>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QTranslator>
#include <QScreen>
#include <QWindow>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

void selectNormalModel(QString qPath)
{
    QFileInfo info(qPath);
    QDir dir(info.path());
    if (!dir.exists())
       dir.mkpath(info.path());

    QFile file(qPath);

    if (!file.open(QFile::WriteOnly))
        exit(0);

    QJsonObject obj;
    obj.insert("allow_switch", true);
    obj.insert("last_wm", "deepin-metacity");

    QJsonDocument doc(obj);
    file.write(doc.toJson());
    file.close();
}

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setApplicationName("deepin-wm-chooser");

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    QCommandLineOption config(QStringList() << "c" << "config", "");
    config.setValueName("ConfigPath");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(config);
    parser.process(a);

    if (parser.isSet(config)) {
        const DSysInfo::DeepinType DeepinType = DSysInfo::deepinType();
        bool IsServerSystem = DSysInfo::DeepinServer == DeepinType;
        if (IsServerSystem) {
            selectNormalModel(parser.value(config));
            return 0;
        }

        PropertyGroup *pg = new PropertyGroup();

        pg->addProperty("contentVisible");

        auto createFrame = [&] (QScreen *screen) -> QWidget* {
            WMFrame *w = new WMFrame;
            w->setScreen(screen);
            pg->addObject(w);
            w->setConfigPath(parser.value(config));
            QObject::connect(w, &WMFrame::destroyed, pg, &PropertyGroup::removeObject);
            w->show();
            return w;
        };

        MultiScreenManager multi_screen_manager;
        multi_screen_manager.register_for_mutil_screen(createFrame);
    } else {
        return 0;
    }
#ifdef QT_DEBUG
    DAccessibilityChecker checker;
    checker.setOutputFormat(DAccessibilityChecker::FullFormat);
    checker.start();
#endif
    return a.exec();
}
