#include "authdbusadapter.h"
#include "authagent.h"

const QString AuthDBusAdaptor::RequestEchoOff(const QString &msg)
{
    return parent()->RequestEchoOff(msg);
}

const QString AuthDBusAdaptor::RequestEchoOn(const QString &msg)
{
    return parent()->RequestEchoOn(msg);
}

void AuthDBusAdaptor::DisplayErrorMsg(const QString &msg)
{
    parent()->DisplayErrorMsg(msg);
}

void AuthDBusAdaptor::DisplayTextInfo(const QString &msg)
{
    parent()->DisplayTextInfo(msg);
}

void AuthDBusAdaptor::RespondResult(const QString &msg)
{
    parent()->RespondResult(msg);
}
