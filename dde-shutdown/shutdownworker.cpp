#include "shutdownworker.h"
#include "userinfo.h"
#include "sessionbasemodel.h"

using namespace Auth;

ShutdownWorker::ShutdownWorker(SessionBaseModel * const model, QObject *parent)
    : AuthInterface(model, parent)
    , m_hotZoneInter(new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone", QDBusConnection::sessionBus(), this))
{
    if (valueByQSettings<bool>("", "loginPromptAvatar", true)) {
        initDBus();
        initData();
    }
}

void ShutdownWorker::switchToUser(std::shared_ptr<User> user)
{
    Q_UNUSED(user);
}

void ShutdownWorker::authUser(const QString &password)
{
    Q_UNUSED(password);
}

void ShutdownWorker::enableZoneDetected(bool enable)
{
    m_hotZoneInter->EnableZoneDetected(enable);
}
