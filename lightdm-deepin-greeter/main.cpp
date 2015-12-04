#include <QApplication>
#include <QtCore/QTranslator>
#include "app/loginframe.h"
#include <QLabel>
#include <QProcess>

#include "logmanager.h"

void setAllMonitorsExtend()
{
    QProcess *checkMons = new QProcess;
    checkMons->start("xrandr");
    checkMons->waitForFinished(1000);

    QString primaryMonitor;
    QStringList otherMonitors;

    const QString result = checkMons->readAllStandardOutput();
    const QStringList &infoList = result.split('\n');
    for (const QString &info : infoList)
    {
        qDebug() << "info: " << info;

        const QStringList details = info.split(' ');

        if (details.count() < 3 || details.at(1) != "connected")
            continue;

        qDebug() << "found monitor: " << details.first();

        if (details.at(2) == "primary")
            primaryMonitor = details.first();
        else
            otherMonitors.append(details.first());
    }

    // set other monitors
    QString lastMonitor = primaryMonitor;
    if (lastMonitor.isEmpty())
        lastMonitor = otherMonitors.first();

    for (const QString &m : otherMonitors)
    {
        if (m == lastMonitor)
            continue;

        QProcess *setting = new QProcess;
        QString cmd = QString("xrandr --output %1 --right-of %2 --auto").arg(m).arg(lastMonitor);

        qDebug() << "exec: " << cmd;

        setting->start(cmd);
        bool result = setting->waitForFinished(1000);
        qDebug() << "finished: " << result;
        setting->deleteLater();
    }

    checkMons->deleteLater();
}

int main(int argc, char* argv[])
{   
    setAllMonitorsExtend();

    QApplication a(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("lightdm-deepin-greeter");
    qApp->setApplicationVersion("2015.1.0");
    LogManager::instance()->debug_log_console_on();

    /* the logmanager is used to output debug log */
    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    const QString &backgroundUrl = ":/theme/background/default_background.jpg";
    LoginFrame  w(backgroundUrl);
    w.show();
    w.grabKeyboard();

    return a.exec();
}
