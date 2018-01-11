#include "processinfomanager.h"

#include <QTimer>
#include <QDebug>
#include <QWindow>
#include <QFile>
#include <QApplication>
#include <QRegularExpression>
#include <QDBusPendingCallWatcher>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

const QStringList BlackListDesktopSuffix = { "google-chrome.desktop",
                                             "deepin-activator.desktop",
                                             "im-launch.desktop",
                                             "polkit-dde-authentication-agent-1.desktop",
                                           };

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
    , m_startManagerInter(new StartManagerInter("com.deepin.SessionManager", "/com/deepin/StartManager", QDBusConnection::sessionBus(), this))
    , m_chromeTabsInter(new ChromeTabsInter("com.deepin.chromeExtension.TabsLimit", "/com/deepin/chromeExtension/TabsLimit", QDBusConnection::sessionBus(), this))
{
    m_refreshTimer->setSingleShot(false);
    m_refreshTimer->setInterval(1000);

    QFile sessionId("/proc/self/sessionid");
    if (sessionId.open(QIODevice::ReadOnly))
        m_sessionId = sessionId.readAll();
    else
        m_sessionId = "2";

    connect(m_refreshTimer, &QTimer::timeout, this, &ProcessInfoManager::scanProcessInfos);
    connect(m_refreshTimer, &QTimer::timeout, this, &ProcessInfoManager::scanChromeTabs);
}

void ProcessInfoManager::scanChromeTabs()
{
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(m_chromeTabsInter->GetTabInfoList(), this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ProcessInfoManager::scanChromeTabsCB);
}

void ProcessInfoManager::scanProcessInfos()
{
    const auto &apps = m_startManagerInter->GetApps().value();

    processInfoList.clear();

    // append basic apps
    for (auto it(apps.cbegin()); it != apps.cend(); ++it)
        appendCGroupPath(QString("/%1@dde/uiapps/%2").arg(m_sessionId).arg(it.key()), it.value());

    mergeLists();

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

void ProcessInfoManager::mergeLists()
{
    for (const auto tab : tabsInfoList)
        processInfoList << tab;
}

void ProcessInfoManager::scanChromeTabsCB(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<ChromeTabList> reply = *watcher;

    tabsInfoList.clear();

    // append chrome tabs
    const ChromeTabList &tabs = reply.value();
    for (const auto &tab : tabs)
        appendChromeTab(tab);

    watcher->deleteLater();
}

void ProcessInfoManager::appendCGroupPath(const QString &path, const QString &desktop)
{
    if (desktop == "sh")
        return;
    for (const auto &suffix : BlackListDesktopSuffix)
        if (desktop.endsWith(suffix))
            return;

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

void ProcessInfoManager::appendChromeTab(const ChromeTabInfo &tabInfo)
{
    ProcessInfo pInfo;
    pInfo.total_mem_bytes = tabInfo.memory;
    pInfo.app_name = tabInfo.title;
    pInfo.id = tabInfo.id;
    pInfo.desktop = "/google-chrome.desktop";

    tabsInfoList << std::move(pInfo);
}
