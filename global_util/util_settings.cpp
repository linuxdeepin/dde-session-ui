#include <QDebug>
#include "util_settings.h"

UtilSettings::UtilSettings(QObject *parent)
    : QObject(parent)
{
}

UtilSettings::~UtilSettings()
{

}

QStringList UtilSettings::getKbdLayoutList(QString username) {
    QSettings settings(USERS_PATH, QSettings::IniFormat);
    //The KeyboardLayoutList in new file users.ini is like:
    // KeyboardList = us| ac|dd ...
    qDebug() << "usersPath:" << username << USERS_PATH;
    QStringList keyboardList = settings.value(username + "/KeyboardLayoutList").toString().split(" ");
    qDebug() << "ORIGIN keyboardList" << keyboardList << keyboardList.length();
    return keyboardList;
}

void UtilSettings::setCurrentKbdLayout(QString username, QString keybdLayout) {
    QSettings settings(USERS_PATH, QSettings::IniFormat);

    qDebug() << "settings.value:" << settings.value(username + "/KeyboardLayout").toString();
    settings.beginGroup(username);
    settings.setValue("KeyboardLayout", keybdLayout);
    settings.endGroup();
}
