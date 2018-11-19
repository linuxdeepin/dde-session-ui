#include "dbusshutdownagent.h"

DBusShutdownAgent::DBusShutdownAgent(QObject *parent) : QObject(parent)
{

}

void DBusShutdownAgent::addFrame(ShutdownFrame *frame)
{
    m_frames << frame;
}

void DBusShutdownAgent::Ping()
{

}

void DBusShutdownAgent::Shutdown()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::Shutdown);
        frame->show();
    }
}

void DBusShutdownAgent::Restart()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::Restart);
        frame->show();
    }
}

void DBusShutdownAgent::Logout()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::Logout);
        frame->show();
    }
}

void DBusShutdownAgent::Suspend()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::Suspend);
        frame->show();
    }
}

void DBusShutdownAgent::SwitchUser()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::SwitchUser);
        frame->show();
    }
}

void DBusShutdownAgent::Show()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->show();
    }
}
