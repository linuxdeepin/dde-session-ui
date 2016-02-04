/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef XKBPARSER_H
#define XKBPARSER_H

#include <QtXml>
#include <QtXmlDepends>
#include <QObject>
#include <QDebug>
#include <QFile>

class XkbParser: public QObject
{
public:
    XkbParser(QObject* parent = 0);
    ~XkbParser();
    struct VariantItem {
        QString name;
        QString description;
    };
    struct LayoutItem {
        QString name;
        QString description;
        QList<VariantItem> variantItemList;
    };
    QList<LayoutItem> KeyboardLayoutList;
public slots:
    QStringList lookUpKeyboardList(QStringList keyboardList_key);
    QString lookUpKeyboardKey(QString keyboard_value);
private:
    const char* kBaseFile = "/usr/share/X11/xkb/rules/base.xml";

    bool parse();
};

#endif // XKBPARSER_H
