#ifndef UTIL_SETTINGS
#define UTIL_SETTINGS

#include <QObject>
#include <QMap>
class UtilSettings:public QObject {
    Q_OBJECT
public:
    UtilSettings(QObject* parent = 0);
    ~UtilSettings();
    const QString USERS_PATH = "/var/lib/greeter/users.ini";

//    QMap<QString, QString> m_keyboardLayoutMap;
    QStringList m_keyboardLayoutNameList;
    QStringList m_keyboardLayoutShortDescriptionList;
    QStringList m_keyboardLayoutMainDescriptionList;
public slots:
    QStringList getKbdLayoutList(QString username);
    QStringList getKbdLayoutMainDescriptionList();
//    QMap<QString, QString> getKbdLayoutShortDescriptionMap();
    void setCurrentKeybdLayoutValue(QString username, QString value);

private:
    QMap<QString, QString> m_kbdLayoutMainDescriptionMap;
    QMap<QString, QString> m_kbdLayoutShortDescriptionMap;
};


#endif // UTIL_SETTINGS

