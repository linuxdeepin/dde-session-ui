#include <QApplication>
#include <QDebug>

#include "frame.h"
#include "dialog.h"
#include "dbus/dbusupdatejobmanager.h"

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
    QApplication a(argc, argv);

    /* No need to do this, lastore-session-helper will do this before launch this program.
    if (!DownloadedPackagesAvailable()) {
        qDebug() << "No downloaded packages available.";
        return 0;
    }
    */

    Dialog d;
    d.show();

    QObject::connect(&d, &Dialog::buttonClicked, [](int index, const QString &){
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

    QObject::connect(&d, &Dialog::aboutToClose, []{
        // remind later;
        UpgradeRemindMeLater();
    });

    return a.exec();
}
