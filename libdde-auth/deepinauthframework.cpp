#include "deepinauthframework.h"
#include "interface/deepinauthinterface.h"
#include "userinfo.h"

#include <QTimer>
#include <QVariant>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

static std::shared_ptr<User> USER;
static QString PASSWORD;

DeepinAuthFramework::DeepinAuthFramework(DeepinAuthInterface *inter, QObject *parent)
    : QObject(parent)
    , m_interface(inter)
{
}

DeepinAuthFramework::~DeepinAuthFramework()
{
}

bool DeepinAuthFramework::isAuthenticate() const
{
    return !m_keyboard.isNull() || !m_fprint.isNull();
}

void DeepinAuthFramework::SetUser(std::shared_ptr<User> user)
{
    USER = user;
}

void DeepinAuthFramework::Authenticate()
{
    m_keyboard = new AuthAgent(AuthAgent::Keyboard, this);
    m_fprint = new AuthAgent(AuthAgent::Fprint, this);

    m_keyboard->SetUser(USER->name());
    m_fprint->SetUser(USER->name());

    if (USER->isNoPasswdGrp() || (!USER->isNoPasswdGrp() && !PASSWORD.isEmpty())) {
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
