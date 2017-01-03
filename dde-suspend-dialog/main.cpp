/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "suspenddialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Manager manager;
    QObject::connect(&manager, &Manager::finished, [] (const int code) {
        if (code == 0) {
            qApp->exit(1);
        } else {
            qApp->exit(0);
        }
    });

    return a.exec();
}
