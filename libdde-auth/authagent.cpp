#include "authagent.h"
#include "deepinauthframework.h"

#include "agent_adaptor.h"

#include <QDBusConnection>
#include <QDateTime>
#include <QDebug>
#include <QDBusObjectPath>

AuthAgent::AuthAgent(Type type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
    QString typeName;

    switch (type) {
    case Keyboard:
        typeName = "keyboard";
        break;
    case Fprint:
        typeName = "fprint";
        break;
    default:
        break;
    }

    AgentAdaptor *adaptor = new AgentAdaptor(this);
    QDBusConnection *m_conn = new QDBusConnection(QDBusConnection::systemBus());
    const QString &dbuspath = QString("/com/deepin/daemon/Authority/Agent/%1%2").arg(QDateTime::currentMSecsSinceEpoch()).arg(type);
    qDebug() << m_conn->registerObject(dbuspath, adaptor, QDBusConnection::ExportAllSlots);

    QDBusObjectPath authDBusPath = QDBusObjectPath(dbuspath);

    m_authority = new Authority("com.deepin.daemon.Authority",
                                "/com/deepin/daemon/Authority",
                                QDBusConnection::systemBus(), this);

    QDBusObjectPath keyboard = m_authority->Start(typeName, "", authDBusPath).value();
    m_transaction = new Transaction("com.deepin.daemon.Authority", keyboard.path(), QDBusConnection::systemBus(), this);
}

AuthAgent::~AuthAgent()
{
    Cancel();
}

void AuthAgent::SetUser(const QString &username)
{
    m_transaction->SetUser(username);
}

void AuthAgent::Authenticate()
{
    m_transaction->Authenticate();
}

void AuthAgent::Cancel()
{
    m_transaction->End();
}

const QString AuthAgent::RequestEchoOff(const QString &msg)
{
    return parent()->RequestEchoOff(msg);
}

const QString AuthAgent::RequestEchoOn(const QString &msg)
{
    return parent()->RequestEchoOn(msg);
}

void AuthAgent::DisplayErrorMsg(const QString &msg)
{
    parent()->DisplayErrorMsg(m_type, msg);
}

void AuthAgent::DisplayTextInfo(const QString &msg)
{
    parent()->DisplayTextInfo(m_type, msg);
}

void AuthAgent::RespondResult(const QString &msg)
{
    parent()->RespondResult(m_type, msg);
}

DeepinAuthFramework *AuthAgent::parent() {
    return qobject_cast<DeepinAuthFramework*>(QObject::parent());
}
