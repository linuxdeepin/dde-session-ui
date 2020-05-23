#ifndef NOTIFYSETTINGS_H
#define NOTIFYSETTINGS_H

#include "launcherinter.h"

#include <QObject>

class QGSettings;
class QTimer;

using LauncherInter = com::deepin::dde::daemon::Launcher;

class NotifySettings : public QObject
{
    Q_OBJECT
public:
    explicit NotifySettings(QObject *parent = nullptr);

signals:
    void settingChanged(QString key);

public slots:

public:
    void initAllSettings();

    void setAppSetting(QString settings);
    QString getAppSetings(QString appName);

    void setSystemSetting(QString settings);
    QString getSystemSetings();

    void setAllSetting(QString settings);
    QString getAllSetings();

    void appAdded(QString appName);
    void appRemoved(QString appName);
private:
    void setSetings(QString settings);
    QString getSetings(QString key);

private:
    QGSettings *m_settings = nullptr;
    LauncherInter *m_launcherInter = nullptr;
    QTimer *m_initTimer;
};

#endif // NOTIFYSETTINGS_H
