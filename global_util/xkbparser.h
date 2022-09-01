// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    explicit XkbParser(QObject* parent = nullptr);
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
