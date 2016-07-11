/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef UTIL_FILE
#define UTIL_FILE
#include <QFile>
#include <QObject>
#include <QDebug>

#include "constants.h"

class UtilFile:public QObject {
    Q_OBJECT
public:
    UtilFile(QObject* parent=0);
    ~UtilFile();

    static int GetAuthLimitation();

    int getExpandState();
    void setExpandState(int stateIndex);
};
#endif // UTIL_FILE

