#include <DApplication>
#include <DLog>
#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "frame.h"
#include "dialog.h"
#include "dbus/dbusupdatejobmanager.h"

DWIDGET_USE_NAMESPACE

DUTIL_USE_NAMESPACE

static bool DownloadedPackagesAvailable() {
    DBusUpdateJobManager manager("com.deepin.lastore",
                                 "/com/deepin/lastore",
                                 QDBusConnection::systemBus());
    QDBusObjectPath path;
    bool result = manager.CheckDownloadUpgradablePackagesJob(path);

    return result;
}

static void ScheduleUpgrade(int secs) {
    QDBusInterface scheduler("com.deepin.LastoreSessionHelper",
                             "/com/deepin/LastoreSessionHelper",
                             "com.deepin.LastoreSessionHelper",
                             QDBusConnection::sessionBus());
    scheduler.call("TimeAfter", secs);
}

static bool Upgrading = false;
static void UpgradeRemindMeLater() {
    if (Upgrading) return;
    ScheduleUpgrade(2 * 60 * 60);
    qApp->quit();
}

static void UpgradeNotNow() {
    if (Upgrading) return;
    ScheduleUpgrade(24 * 60 * 60);
    qApp->quit();
}

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-offline-upgrader");
    a.setApplicationVersion("0.9");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption noAskOption(QStringList() << "s" << "silent",
                                   "don't show the option dialog, upgrade directly.");
    parser.addOption(noAskOption);

    parser.process(a);

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    if (!a.setSingleInstance(a.applicationName())) {
        qDebug() << "Another instance is running.";
        return 0;
    }

    /* No need to do this, lastore-session-helper will do this before launch this program.
    if (!DownloadedPackagesAvailable()) {
        qDebug() << "No downloaded packages available.";
        return 0;
    }
    */

    if (parser.isSet(noAskOption)) {
        Frame * f = new Frame;
        f->showFullScreen();
        f->distUpgrade();
    } else {
        Dialog * d = new Dialog;
        d->show();

        QObject::connect(d, &Dialog::buttonClicked, [](int index, const QString &){
            Frame * f = new Frame;
            switch (index) {
            case 0:
                // remind later
                UpgradeRemindMeLater();
                break;
            case 1:
                Upgrading = true;
                f->showFullScreen();
                f->distUpgrade();
                break;
            case 2:
                // not now.
                UpgradeNotNow();
                break;
            default:
                break;
            }
        });

        QObject::connect(d, &Dialog::aboutToClose, []{
            // remind later;
            UpgradeRemindMeLater();
        });
    }

    return a.exec();
}
