/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "util_file.h"

#include <QSettings>
#include <QTextCodec>

UtilFile::UtilFile(QObject *parent)
    : QObject(parent) {

}

UtilFile::~UtilFile() {
}

int UtilFile::GetAuthLimitation()
{
    QSettings settings("/etc/deepin-version", QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("utf8"));
    if (settings.value("Release/Type").toString() == "Server") {
        return 3;
    }

    return INT_MAX;
}

int UtilFile::getExpandState() {
    //expand UserWidget
    QFile file(DDESESSIONCC::LOCK_SHUTDOWN_FLAG);
    if (file.exists()) {
        qDebug() << "exist file open:" << file.open(QIODevice::ReadOnly);
        QTextStream out(&file);
        int stateIndex = out.readAll().trimmed().toInt();
        file.close();
        return stateIndex;
    } else {
        return 0;
    }
}

void UtilFile::setExpandState(int stateIndex) {
    QFile file(DDESESSIONCC::LOCK_SHUTDOWN_FLAG);
    qDebug() << "file open:" << file.open(QIODevice::ReadWrite|QIODevice::Truncate);
    file.setPermissions(DDESESSIONCC::LOCK_SHUTDOWN_FLAG, QFile::ReadOwner|QFile::WriteOwner
                        |QFile::ReadGroup|QFile::WriteGroup|QFile::ReadOther|QFile::WriteOther
                        |QFile::ReadUser|QFile::WriteUser);

    QTextStream in(&file);
    in << stateIndex;
    file.close();
}
