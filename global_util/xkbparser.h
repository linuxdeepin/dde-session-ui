/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
