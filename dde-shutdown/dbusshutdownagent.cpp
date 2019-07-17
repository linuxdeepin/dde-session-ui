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
    // action未成功执行时，可能是被其它应用程序阻塞，此时应当确保主窗口显示，将阻塞程序的列表显示出来
    if (!m_frames.first()->powerAction(Actions::Suspend)) {
        for (ShutdownFrame *frame : m_frames) {
            frame->show();
        }
    }
}

void DBusShutdownAgent::Hibernate()
{
    // action未成功执行时，可能是被其它应用程序阻塞，此时应当确保主窗口显示，将阻塞程序的列表显示出来
    if (!m_frames.first()->powerAction(Actions::Hibernate)) {
        for (ShutdownFrame *frame : m_frames) {
            frame->show();
        }
    }
}

void DBusShutdownAgent::SwitchUser()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->setConfirm(true);
        frame->powerAction(Actions::SwitchUser);
    }
}

void DBusShutdownAgent::Show()
{
    for (ShutdownFrame *frame : m_frames) {
        frame->show();
    }
}
