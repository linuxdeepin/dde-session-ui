/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "util_settings.h"

UtilSettings::UtilSettings(QObject *parent)
    : QObject(parent)
{
}

UtilSettings::~UtilSettings()
{
}

QString UtilSettings::getCurrentKbdLayout(QString username) {
    QSettings settings(USERS_PATH, QSettings::IniFormat);
    //The KeyboardLayout in new file USERS_PATH is like:
    // KeyboardList = us|
    qDebug() << "usersPath:" << username << USERS_PATH;
    QStringList keybdLayoutList = settings.value(username + "/KeyboardLayout").toString().split("|");
    QString currentKeybdLayout = "";
    if (keybdLayoutList.length() != 0) {
        currentKeybdLayout = keybdLayoutList.at(0);
    }
    //QString currentKeybdLayout = keyboardList[0].split("|")[0];
    qDebug() << "current keyboardLayout:" << currentKeybdLayout;
    return currentKeybdLayout;
}

QStringList UtilSettings::getKbdLayoutList(QString username) {
    Q_ASSERT(false); // this function is deprecreated
    QSettings settings(USERS_PATH, QSettings::IniFormat);
    //The KeyboardLayoutList in new file USERS_PATH is like:
    // KeyboardList = us| ac|dd ...
    qDebug() << "usersPath:" << username << USERS_PATH;
    QStringList keyboardList = settings.value(username + "/KeyboardLayout").toString().split(" ");
    qDebug() << "ORIGIN keyboardList" << keyboardList << keyboardList.length();
    for (int k(0); k < keyboardList.length(); k++) {
        if (keyboardList[k].split("|")[0] == getCurrentKbdLayout(username)) {
            currentListItemIndex = k;
        }
    }
    return keyboardList;
}

void UtilSettings::setCurrentKbdLayout(QString username, QString keybdLayout) {
    Q_ASSERT(false); // this function is deprecreated
    QSettings settings(USERS_PATH, QSettings::IniFormat);

    qDebug() << "settings.value:" << settings.value(username + "/KeyboardLayout").toString();
    settings.beginGroup(username);
    settings.setValue("KeyboardLayout", keybdLayout);
    settings.endGroup();
}
