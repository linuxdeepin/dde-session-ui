/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

