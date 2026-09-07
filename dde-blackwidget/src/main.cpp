// SPDX-FileCopyrightText: 2015 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.h"

#include <DLog>
#include <DGuiApplicationHelper>

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDebug>

const QString DBUS_SERV = "org.deepin.dde.BlackScreen1";
const QString DBUS_PATH = "/org/deepin/dde/BlackScreen1";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!Dtk::Gui::DGuiApplicationHelper::setSingleInstance(QStringLiteral("dde-blackwidget"))) {
        qInfo() << "dde-blackwidget already running, quit.";
        return 0;
    }
#ifdef QT_DEBUG
    DCORE_USE_NAMESPACE::Dtk::Core::DLogManager::registerConsoleAppender();
#endif
    DCORE_USE_NAMESPACE::Dtk::Core::DLogManager::registerJournalAppender();

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.process(a);
    const bool useDBus = !parser.positionalArguments().contains(QStringLiteral("nodbus"));

    Window w;
    qInfo() << " Black screen use dbus : " << useDBus;

    if (!useDBus) {
        qInfo() << "Direct to launch black widget.";
        w.setLogoVisible(true);
        w.raiseWindow();
        int ret = a.exec();
        w.cleanupBeforeExit();
        return ret;
    }

    QDBusConnection sessionDBus = QDBusConnection::sessionBus();
    bool result = sessionDBus.interface()->registerService(DBUS_SERV,
                                                           QDBusConnectionInterface::ReplaceExistingService,
                                                           QDBusConnectionInterface::AllowReplacement);
    qInfo() << "Session register service : " << result;

    BlackWidgetAdaptor adaptor(&w);

    if (result) {
        qInfo() << "Session register object : " << sessionDBus.registerObject(DBUS_PATH, &w);
        QObject::connect(sessionDBus.interface(), &QDBusConnectionInterface::serviceUnregistered, &w, &Window::onNameLost);
    } else {
        qInfo() << "End black widget.";
        return -1;
    }

    int ret = a.exec();
    adaptor.quitDBusService();
    w.cleanupBeforeExit();
    return ret;
}
