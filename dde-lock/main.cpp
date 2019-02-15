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

#include <DLog>

#include "lockframe.h"
#include "dbus/dbuslockfrontservice.h"
#include "dbus/dbuslockagent.h"
#include "multiscreenmanager.h"

#include "lockcontent.h"
#include "lockworker.h"
#include "sessionbasemodel.h"
#include "propertygroup.h"

#include <QLabel>
#include <QScreen>
#include <QWindow>
#include <dapplication.h>
#include <QDBusInterface>
#include <QDesktopWidget>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-lock");
    app.setApplicationVersion("2015.1.0");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    app.installTranslator(&translator);

    QCommandLineParser cmdParser;
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();

    QCommandLineOption backend(QStringList() << "d" << "daemon", "start to daemon mode");
    cmdParser.addOption(backend);
    QCommandLineOption switchUser(QStringList() << "s" << "switch", "show user switch");
    cmdParser.addOption(switchUser);
    cmdParser.process(app);

    bool runDaemon = cmdParser.isSet(backend);
    bool showUserList = cmdParser.isSet(switchUser);

    SessionBaseModel *model = new SessionBaseModel(SessionBaseModel::AuthType::LockType);
    LockWorker *worker = new LockWorker(model); //
    PropertyGroup *property_group = new PropertyGroup(worker);

    property_group->addProperty("contentVisible");

    auto createFrame = [&] (QScreen *screen) -> QWidget* {
        LockFrame *lockFrame = new LockFrame(model);
        lockFrame->setScreen(screen);
        property_group->addObject(lockFrame);
        QObject::connect(lockFrame, &LockFrame::requestSwitchToUser, worker, &LockWorker::switchToUser);
        QObject::connect(lockFrame, &LockFrame::requestAuthUser, worker, &LockWorker::authUser);
        QObject::connect(model, &SessionBaseModel::visibleChanged, lockFrame, &LockFrame::setVisible);
        QObject::connect(model, &SessionBaseModel::showUserList, lockFrame, &LockFrame::showUserList);
        QObject::connect(lockFrame, &LockFrame::requestSetLayout, worker, &LockWorker::setLayout);
        QObject::connect(lockFrame, &LockFrame::requestEnableHotzone, worker, &LockWorker::enableZoneDetected, Qt::UniqueConnection);
        QObject::connect(lockFrame, &LockFrame::destroyed, property_group, &PropertyGroup::removeObject);
        lockFrame->setVisible(model->isShow());
        return lockFrame;
    };

    MultiScreenManager multi_screen_manager;
    multi_screen_manager.register_for_mutil_screen(createFrame);

    DBusLockAgent agent;
    agent.setModel(model);
    DBusLockFrontService service(&agent);
    Q_UNUSED(service);

    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.registerService(DBUS_NAME) ||
            !conn.registerObject("/com/deepin/dde/lockFront", &agent) ||
            !app.setSingleInstance(QString("dde-lock"), DApplication::UserScope)) {
        qDebug() << "register dbus failed"<< "maybe lockFront is running..." << conn.lastError();

        if (!runDaemon) {
            const char *interface = "com.deepin.dde.lockFront";
            QDBusInterface ifc(DBUS_NAME, DBUS_PATH, interface, QDBusConnection::sessionBus(), NULL);
            if (showUserList) {
                ifc.asyncCall("ShowUserList");
            } else {
                ifc.asyncCall("Show");
            }
        }
    } else {
        if (!runDaemon) {
            if (showUserList) {
                emit model->showUserList();
            } else {
                model->setIsShow(true);
            }
        }
        app.exec();
    }

    return 0;
}
