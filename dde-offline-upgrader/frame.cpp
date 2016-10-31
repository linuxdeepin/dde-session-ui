#include "frame.h"
#include "deepinwave.h"

#include "dbus/dbusupdatejobmanager.h"
#include "dbus/dbusupdatejob.h"
#include "../global_util/dbus/dbuslogin1manager.h"

#include <QResizeEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QDBusPendingCallWatcher>

Frame::Frame(QWidget *parent)
    : QWidget(parent),
      m_wave(new DeepinWave(this)),
      m_progressTip(new QLabel(this)),
      m_warningTip(new QLabel(this)),
      m_dbusJobManagerInter(new DBusUpdateJobManager("com.deepin.lastore",
                                                     "/com/deepin/lastore",
                                                     QDBusConnection::systemBus(),
                                                     this))
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setCursor(Qt::BlankCursor);
    setStyleSheet("background:black");
    m_progressTip->setStyleSheet("color:white; font-size:14px");
    m_warningTip->setStyleSheet("color:white; font-size:11px");
    m_warningTip->setText(tr("Please don't power off or unplug your machine"));

    updateProgress(0);
}

Frame::~Frame()
{

}

void Frame::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QDesktopWidget * desktop = qApp->desktop();
    QRect primaryRect = desktop->availableGeometry(desktop->primaryScreen());
    m_wave->move(primaryRect.center() - m_wave->rect().center());

    m_progressTip->setFixedSize(primaryRect.width(), 20);
    m_warningTip->setFixedSize(primaryRect.width(), 20);

    m_progressTip->setAlignment(Qt::AlignHCenter);
    m_warningTip->setAlignment(Qt::AlignHCenter);

    m_progressTip->move(primaryRect.x(), m_wave->y() + m_wave->height() + 20);
    m_warningTip->move(primaryRect.x(), primaryRect.y() + primaryRect.height() - 70);
}

void Frame::showFullScreen()
{
    QDesktopWidget * desktop = qApp->desktop();
    setFixedSize(desktop->geometry().size());
    show();
    grabKeyboard();
    grabMouse();
}

void Frame::distUpgrade()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_dbusJobManagerInter->DistUpgrade(), this);
    connect(watcher, &QDBusPendingCallWatcher::finished, [this, watcher] {
        const QDBusPendingReply<QDBusObjectPath> &reply = *watcher;
        if (reply.isError()) {
            qDebug() << reply.error().message();
            qDebug() << "Quiting the program due to last error.";
            qApp->quit();
        }

        const QDBusObjectPath &path = reply.value();
        qDebug() << "distupgrade, getting job path " << path.path();

        qDebug() << "start upgrade job: " << path.path() << reply.error();
        DBusUpdateJob *job = new DBusUpdateJob("com.deepin.lastore", path.path(), QDBusConnection::systemBus(), this);
        connect(job, &DBusUpdateJob::ProgressChanged, [this, job] {
            const double progress = job->progress();
            qDebug() << "job progress updated: " << progress;

            updateProgress(job->progress());
        });
        connect(job, &DBusUpdateJob::StatusChanged, [this, job] {
            const QString status = job->status();
            qDebug() << "job status changed: " << status;

            if (status == "succeed" || status == "success" || status == "end" || status.isEmpty()) {
                // give lastore daemon some time to take care of its business.
                QTimer::singleShot(1000, this, SLOT(tryReboot()));
            }
            if (status == "failed") {
                // Failed to upgrade, quit the application.
                qApp->quit();
            }
        });

        watcher->deleteLater();
    });
}

void Frame::updateProgress(double progress)
{
    m_wave->setProgress(progress);

    QString percentage = QString::number(progress * 100, 'f', 0);
    m_progressTip->setText(tr("Installing updates %1%, please wait...").arg(percentage));
}

void Frame::tryReboot()
{
    static int TryRebootTimes = 1;

    DBusLogin1Manager * manager = new DBusLogin1Manager("org.freedesktop.login1",
                                                        "/org/freedesktop/login1",
                                                        QDBusConnection::systemBus());

    qDebug() << "try to reboot the machine for the " << TryRebootTimes << " time";
    TryRebootTimes++;
    manager->Reboot(false);

    connect(manager, &DBusLogin1Manager::BlockInhibitedChanged, [manager] {
        if (manager->blockInhibited().isEmpty()) {
            manager->Reboot(false);
        }
    });

    // Try again.
    QTimer::singleShot(1000, this, SLOT(tryReboot()));
}
