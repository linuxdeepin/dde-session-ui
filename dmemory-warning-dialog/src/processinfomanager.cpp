#include "processinfomanager.h"

#include <QTimer>
#include <QDebug>
#include <QWindow>
#include <QFile>
#include <QApplication>
#include <QRegularExpression>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

typedef QMap<quint32, QString> AppsMap;

QString genericAppName(const QString &desktop)
{
    do {
        if (desktop.isEmpty())
            break;

        QFile f(desktop);
        if (!f.open(QIODevice::ReadOnly))
            break;

        const QString output = f.readAll();
        const QString locale = QLocale().name();

        QRegularExpression regex(QString("Name\\[%1\\]=(.+)$").arg(locale));
        regex.setPatternOptions(QRegularExpression::MultilineOption);
        auto match = regex.match(output);
        if (match.isValid() && match.hasMatch())
            return match.captured(1);

        regex = QRegularExpression(QString("Name=(.+)$"));
        regex.setPatternOptions(QRegularExpression::MultilineOption);
        match = regex.match(output);
        if (match.isValid() && match.hasMatch())
            return match.captured(1);

    } while (false);

    return desktop;
}

ProcessInfoManager::ProcessInfoManager(QObject *parent)
    : QObject(parent)

    , m_refreshTimer(new QTimer(this))
{
    m_refreshTimer->setSingleShot(false);
    m_refreshTimer->setInterval(1000);

    QFile sessionId("/proc/self/sessionid");
    if (sessionId.open(QIODevice::ReadOnly))
        m_sessionId = sessionId.readAll();
    else
        m_sessionId = "2";

    connect(m_refreshTimer, &QTimer::timeout, this, &ProcessInfoManager::scanProcessInfos);

    qRegisterMetaType<AppsMap>("AppsMap");
    qDBusRegisterMetaType<AppsMap>();
}

void ProcessInfoManager::scanProcessInfos()
{
    // using dbus to find windows
    QDBusInterface appsInter("com.deepin.SessionManager", "/com/deepin/StartManager",  "com.deepin.StartManager");
    AppsMap reply = QDBusReply<AppsMap>(appsInter.call("GetApps")).value();

    processInfoList.clear();
    for (auto it(reply.cbegin()); it != reply.cend(); ++it)
        appendCGroupPath(QString("/%1@dde/uiapps/%2").arg(m_sessionId).arg(it.key()), it.value());

    // sort by mem
    std::sort(processInfoList.begin(), processInfoList.end(),
              [](const ProcessInfo &a, const ProcessInfo &b) {
        return a.total_mem_bytes > b.total_mem_bytes;
    });

    emit processInfoListChanged();
}

void ProcessInfoManager::startRefreshData()
{
    m_refreshTimer->start();
    QTimer::singleShot(1, this, &ProcessInfoManager::scanProcessInfos);
}

void ProcessInfoManager::appendCGroupPath(const QString &path, const QString &desktop)
{
    const QString basePath = "/sys/fs/cgroup/memory" + path;

    const auto idx = std::find_if(processInfoList.begin(), processInfoList.end(),
                               [&](const ProcessInfo &info) {
        return info.cgroup_path == basePath;
    });
    if (idx != processInfoList.end())
        return;


    QFile mem_usage(basePath + "/memory.usage_in_bytes");
    if (!mem_usage.open(QIODevice::ReadOnly))
        return;
    const QString &mem_num = mem_usage.readAll();

    QFile procs(basePath + "/cgroup.procs");
    if (!procs.open(QIODevice::ReadOnly))
        return;
    const auto &pidList = procs.readAll().split('\n');
    const QString &mainProcId = pidList.first();

    QFile procCmdline("/proc/" + mainProcId + "/cmdline");
    if (!procCmdline.open(QIODevice::ReadOnly))
        return;

    ProcessInfo pInfo;
    pInfo.total_mem_bytes = mem_num.trimmed().toUInt();
    pInfo.cgroup_path = basePath;
    pInfo.app_name = genericAppName(desktop);
    pInfo.desktop = desktop;
    for (const auto &id : pidList)
        if (!id.isEmpty())
            pInfo.pid_list << id;

    processInfoList << std::move(pInfo);
}
