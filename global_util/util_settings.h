#ifndef UTIL_SETTINGS
#define UTIL_SETTINGS

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QMap>

class UtilSettings:public QObject {
    Q_OBJECT
public:
    UtilSettings(QObject* parent = 0);
    ~UtilSettings();

    const QString USERS_PATH = "/var/lib/greeter/users.ini";
    QStringList keyboardLayoutNameList;
    int currentListItemIndex = 0;
public slots:
    QString getCurrentKbdLayout(QString username);
    QStringList getKbdLayoutList(QString username);

    void setCurrentKbdLayout(QString username, QString keybdLayout);
};


#endif // UTIL_SETTINGS

