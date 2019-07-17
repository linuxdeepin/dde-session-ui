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

#include <DApplication>
#include <QtCore/QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QDBusAbstractInterface>
#include <DDBusSender>

#include <DLog>

#include "app/shutdownframe.h"
#include "sessionbasemodel.h"
#include "shutdownworker.h"
#include "propertygroup.h"
#include "multiscreenmanager.h"

#include "dbusshutdownagent.h"

const QString DBUS_PATH = "/com/deepin/dde/shutdownFront";
const QString DBUS_NAME = "com.deepin.dde.shutdownFront";

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-shutdown");
    app.setTheme("light");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    // NOTE: it's better to be -h/--show, but some apps may have used the binary to show
    // the shutdown frame directly, we need to keep the default behavier to that.
    QCommandLineOption daemon(QStringList() << "d" << "daemon",
                              "start the program, but do not show the window.");
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(daemon);
    parser.process(app);

    QDBusConnection session = QDBusConnection::sessionBus();
    if(!session.registerService(DBUS_NAME) ||
            !app.setSingleInstance(QString("dde-shutdown"), DApplication::UserScope)){
        qWarning() << "dde-shutdown is running...";

        if (!parser.isSet(daemon)) {
            DDBusSender()
                    .service(DBUS_NAME)
                    .interface(DBUS_NAME)
                    .path(DBUS_PATH)
                    .method("Show")
                    .call();
        }

        return 0;
    } else {
        qDebug() << "dbus registration success.";

        SessionBaseModel *model = new SessionBaseModel(SessionBaseModel::AuthType::UnknowAuthType);
        ShutdownWorker *worker = new ShutdownWorker(model); //

        DBusShutdownAgent *dbusAgent = new DBusShutdownAgent;
        PropertyGroup *property_group = new PropertyGroup(worker);

        property_group->addProperty("contentVisible");

        auto createFrame = [&] (QScreen *screen) -> QWidget* {
            ShutdownFrame *frame = new ShutdownFrame(model);
            dbusAgent->addFrame(frame);
            frame->setScreen(screen);
            property_group->addObject(frame);
            QObject::connect(model, &SessionBaseModel::visibleChanged, frame, &ShutdownFrame::setVisible);
            QObject::connect(frame, &ShutdownFrame::requestEnableHotzone, worker, &ShutdownWorker::enableZoneDetected);
            QObject::connect(frame, &ShutdownFrame::destroyed, property_group, &PropertyGroup::removeObject);
            QObject::connect(frame, &ShutdownFrame::destroyed, frame, [=] {
                dbusAgent->removeFrame(frame);
            });
            frame->setVisible(model->isShow());
            return frame;
        };

        MultiScreenManager multi_screen_manager;
        multi_screen_manager.register_for_mutil_screen(createFrame);

        QObject::connect(model, &SessionBaseModel::visibleChanged, &multi_screen_manager, &MultiScreenManager::startRaiseContentFrame);

        if (!parser.isSet(daemon)) {
            model->setIsShow(true);
        }

        ShutdownFrontDBus adaptor(dbusAgent); Q_UNUSED(adaptor);
        QDBusConnection::sessionBus().registerObject(DBUS_PATH, dbusAgent);

        return app.exec();
    }

    qWarning() << "have unknow error!";
    return -1;
}
