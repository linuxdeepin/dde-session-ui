#ifndef SHUTDOWNWORKER_H
#define SHUTDOWNWORKER_H

#include "authinterface.h"
#include "dbus/dbushotzone.h"
#include <QObject>

class ShutdownWorker : public Auth::AuthInterface
{
    Q_OBJECT
public:
    explicit ShutdownWorker(SessionBaseModel *const model, QObject *parent = nullptr);

    void switchToUser(std::shared_ptr<User> user);
    void authUser(const QString &password);

public slots:
    void enableZoneDetected(bool enable);

private:
    DBusHotzone *m_hotZoneInter;
};

#endif // SHUTDOWNWORKER_H
