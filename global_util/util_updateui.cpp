/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "util_updateui.h"

void updateStyle(QString styleFiles, QWidget* widget){
    QFile qssFile(QString("%1").arg(styleFiles));

    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()) {
        qss = QLatin1String(qssFile.readAll());
        widget->setStyleSheet(qss);
        qssFile.close();
    }
}

