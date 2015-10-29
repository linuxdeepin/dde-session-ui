#include "util_settings.h"
#include "util_parsexml.h"

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

    for (int i = 0; i < keyboardList.length(); i++) {
        QStringList itemKeyboardList = keyboardList[i].split("|");

        if (itemKeyboardList.count() < 2)
            continue;

        m_keyboardLayoutNameList << itemKeyboardList[0];
        m_keyboardLayoutShortDescriptionList << itemKeyboardList[1];
//        m_keyboardLayoutMap.insert(itemKeyboardList[0], itemKeyboardList[1]);
    }

    return keyboardList;
}
//If you want to get the KbdLayoutMainDescriptionMap, you need to run getKbdLayoutMap() first;

QStringList UtilSettings::getKbdLayoutMainDescriptionList() {

    ParseXML* m_parseMainDescriptionXml = new ParseXML("/usr/share/X11/xkb/rules/base.xml", "configItem", "name", "description", this);
    QMap<QString, QString> tmpMainMap = m_parseMainDescriptionXml->getTagNodeInfo();
//    QMap<QString, QString>::iterator i ;
//    qDebug() << "QQQtmpMainMap:" << m_keyboardLayoutMap;
//    for(i = m_keyboardLayoutMap.begin(); i != m_keyboardLayoutMap.end(); i++) {
//        m_kbdLayoutMainDescriptionMap.insert(i.key(), tmpMainMap.value(i.key()));
//    }
    for (int i = 0; i < m_keyboardLayoutNameList.length(); i++) {
        m_keyboardLayoutMainDescriptionList << tmpMainMap.value(m_keyboardLayoutNameList[i]);
    }

    qDebug() << "YYYYYYYYYYkeyboardLayout:" << m_keyboardLayoutMainDescriptionList;
    return  m_keyboardLayoutMainDescriptionList;
}

//QMap<QString, QString> UtilSettings::getKbdLayoutShortDescriptionMap() {
//    ParseXML* m_parseShortDescriptionXml = new ParseXML("/usr/share/X11/xkb/rules/base.xml", "configItem", "name", "shortDescription", this);
//    QMap<QString, QString> tmpMainMap = m_parseShortDescriptionXml->getTagNodeInfo();
//    QMap<QString, QString>::iterator i ;

//    for(i = m_keyboardLayoutMap.begin(); i != m_keyboardLayoutMap.end(); i++) {
//        if (!i.value().isEmpty()) {
//            m_kbdLayoutShortDescriptionMap.insert(i.value(), tmpMainMap.value(i.value()));
//        }

//    }

//    qDebug() << "keyboardLayout:" << m_kbdLayoutShortDescriptionMap;
//    return m_kbdLayoutShortDescriptionMap;
//}

void UtilSettings::setCurrentKeybdLayoutValue(QString username, QString value) {
    QSettings settings(USERS_PATH, QSettings::IniFormat);
    settings.beginGroup(username);
    settings.setValue("KeyboardLayout", value);
    settings.endGroup();
}
