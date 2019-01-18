#ifndef DBUSSHUTDOWNAGENT_H
#define DBUSSHUTDOWNAGENT_H

#include <QObject>

#include "app/shutdownframe.h"
class DBusShutdownAgent : public QObject
{
    Q_OBJECT
public:
    explicit DBusShutdownAgent(QObject *parent = nullptr);

    void addFrame(ShutdownFrame *frame);
    void removeFrame(ShutdownFrame *frame);

    void Ping();
    void Shutdown();
    void Restart();
    void Logout();
    void Suspend();
    void Hibernate();
    void SwitchUser();
    void Show();

private:
    QList<ShutdownFrame*> m_frames;
};

#endif // DBUSSHUTDOWNAGENT_H
