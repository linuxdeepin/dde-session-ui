#include "dbusshutdownagent.h"

DBusShutdownAgent::DBusShutdownAgent(QObject *parent) : QObject(parent)
{

}

void DBusShutdownAgent::addFrame(ShutdownFrame *frame)
{
    m_frames << frame;
}

void DBusShutdownAgent::removeFrame(ShutdownFrame *frame)
{
    m_frames.removeOne(frame);
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
    m_frames.first()->powerAction(Actions::Suspend);
}

void DBusShutdownAgent::Hibernate()
{
    m_frames.first()->powerAction(Actions::Hibernate);
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
