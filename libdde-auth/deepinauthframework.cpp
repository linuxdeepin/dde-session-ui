#include "deepinauthframework.h"
#include "interface/deepinauthinterface.h"

#include <QTimer>
#include <QVariant>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

static QString USERNAME;
static QString PASSWORD;

static bool checkUserIsNoPWGrp(const QString &username)
{
    // Caution: 32 here is unreliable, and there may be more
    // than this number of groups that the user joins.

    int ngroups = 32;
    gid_t groups[32];
    struct passwd pw;
    struct group gr;

    /* Fetch passwd structure (contains first group ID for user) */

    pw = *getpwnam(username.toUtf8().data());

    /* Retrieve group list */

    if (getgrouplist(username.toUtf8().data(), pw.pw_gid, groups, &ngroups) == -1) {
        fprintf(stderr, "getgrouplist() returned -1; ngroups = %d\n",
                ngroups);
        return false;
    }

    /* Display list of retrieved groups, along with group names */

    for (int i = 0; i < ngroups; i++) {
        gr = *getgrgid(groups[i]);
        if (QString(gr.gr_name) == QString("nopasswdlogin")) {
            return true;
        }
    }

    return false;
}

DeepinAuthFramework::DeepinAuthFramework(DeepinAuthInterface *inter, QObject *parent)
    : QObject(parent)
    , m_interface(inter)
{
}

DeepinAuthFramework::~DeepinAuthFramework()
{
}

void DeepinAuthFramework::SetUser(const QString &username)
{
    USERNAME = username;
}

void DeepinAuthFramework::Authenticate()
{
    m_keyboard = new AuthAgent(AuthAgent::Keyboard, this);
    m_fprint = new AuthAgent(AuthAgent::Fprint, this);

    m_keyboard->SetUser(USERNAME);
    m_fprint->SetUser(USERNAME);

    if (checkUserIsNoPWGrp(USERNAME) || !PASSWORD.isEmpty()) {
        QTimer::singleShot(100, m_keyboard, &AuthAgent::Authenticate);
    }

    // It takes time to auth again after cancel!
    QTimer::singleShot(500, m_fprint, &AuthAgent::Authenticate);
}

void DeepinAuthFramework::Clear()
{
    if (!m_keyboard.isNull()) {
        m_keyboard->deleteLater();
    }

    if (!m_fprint.isNull()) {
        m_fprint->deleteLater();
    }

    PASSWORD.clear();
}

void DeepinAuthFramework::setPassword(const QString &password)
{
    PASSWORD = password;
}

const QString DeepinAuthFramework::RequestEchoOff(const QString &msg)
{
    Q_UNUSED(msg);

    return PASSWORD;
}

const QString DeepinAuthFramework::RequestEchoOn(const QString &msg)
{
    return msg;
}

void DeepinAuthFramework::DisplayErrorMsg(AuthAgent::Type type, const QString &msg)
{
    Q_UNUSED(type);

    m_interface->onDisplayErrorMsg(msg);
}

void DeepinAuthFramework::DisplayTextInfo(AuthAgent::Type type, const QString &msg)
{
    Q_UNUSED(type);

    m_interface->onDisplayTextInfo(msg);
}

void DeepinAuthFramework::RespondResult(AuthAgent::Type type, const QString &msg)
{
    if (type == AuthAgent::Fprint && msg.isEmpty()) return;

    m_interface->onPasswordResult(msg);
}
