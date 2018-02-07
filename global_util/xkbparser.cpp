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

#include <locale.h>
#include <libintl.h>

#include <QDomDocument>
#include <QDomElement>

#include "xkbparser.h"

XkbParser::XkbParser(QObject *parent)
    : QObject(parent)
{
    if (!parse()) {
        qDebug() << "Parse xml failed!";
    }
}

QStringList XkbParser::lookUpKeyboardList(QStringList keyboardList_key) {
    QStringList result;
    setlocale(LC_ALL, "");
    const char xkbDomain[] = "xkeyboard-config";

    for (int k = 0; k < keyboardList_key.length(); k++) {
        QString tmpKey, head_key, tail_key;
        tmpKey = keyboardList_key[k];
        QStringList tmpKeyList = tmpKey.split(";");
        if (tmpKeyList.length() == 2) {
            head_key = tmpKeyList[0];
            tail_key = tmpKeyList[1];

            for (int i = 0; i < KeyboardLayoutList.length(); i++) {
                if (KeyboardLayoutList[i].name == head_key) {
                    if (tail_key.isEmpty()) {
                        result << dgettext(xkbDomain, KeyboardLayoutList[i].description.toLatin1());
                        break;
                    } else {
                        for (int j = 0; j < KeyboardLayoutList[i].variantItemList.length(); j++) {
                            if (KeyboardLayoutList[i].variantItemList[j].name == tail_key) {
                                result << dgettext(xkbDomain, KeyboardLayoutList[i].variantItemList[j].description.toLatin1());


                            }
                        }
                        break;
                    }
                }
            }


        } else {
            continue;
        }
    }

    return result;
}
QString XkbParser::lookUpKeyboardKey(QString keyboard_value) {
    QString keyboard_key = "";
    for (int i = 0; i < KeyboardLayoutList.length(); i++) {
        if (KeyboardLayoutList[i].description == keyboard_value) {
            keyboard_key = QString("%1|").arg(KeyboardLayoutList[i].name);
            qDebug() << "keyboard_key:" << keyboard_key;
            return keyboard_key;
        } else {
            for (int j = 0; j < KeyboardLayoutList[i].variantItemList.length(); j++) {
                if (KeyboardLayoutList[i].variantItemList[j].description == keyboard_value) {
                    keyboard_key = QString("%1|%2").arg(KeyboardLayoutList[i].name).arg(KeyboardLayoutList[i].variantItemList[j].name);
                    return keyboard_key;
                }
            }
        }
    }
    return keyboard_key;
}

bool XkbParser::parse() {

    QFile baseFile(kBaseFile);

    QDomDocument document;

    if (baseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        document.setContent(&baseFile);
    } else {
        qDebug() << "Failed to open base.xml";
        return false;
    }

    QDomElement rootElement = document.documentElement();

    if (rootElement.isNull()) {
        qDebug() << "root element is null.";
        return false;
    }

    QDomElement layoutListElement = rootElement.firstChildElement("layoutList");

    QDomNodeList layoutNodes = layoutListElement.elementsByTagName("layout");

    if (layoutNodes.isEmpty()) {
        qDebug() << "layout list is empty.";
        return false;
    }

    QDomNode layoutNode;
    for (int layoutIndex = 0; layoutIndex < layoutNodes.size(); ++layoutIndex) {
        VariantItem tmpVariantItem;

        LayoutItem tmpLayoutItem;

        layoutNode = layoutNodes.at(layoutIndex);
//        qDebug() << "layout element: " << layoutNode.nodeName();

        QDomElement configElement = layoutNode.firstChildElement("configItem");
        QDomElement nameElement = configElement.firstChildElement("name");
        //get the Layout element: name
        tmpLayoutItem.name = nameElement.text();
//        qDebug() << "layout name: " << nameElement.text();


        QDomElement descriptionElement = configElement.firstChildElement("description");
//        qDebug() << "layout description: " << descriptionElement.text();
        //get the Layout element: description
        tmpLayoutItem.description = descriptionElement.text();

        QDomElement variantListElement = layoutNode.firstChildElement("variantList");
        if (variantListElement.isNull()) {
//            qDebug() << "has no variant list.";
            continue;
        }
        QDomNodeList variantNodes = variantListElement.elementsByTagName("variant");
//        qDebug() << "size of variant list: " << variantNodes.size();
        QDomNode variantNode;
        for (int variantIndex = 0; variantIndex < variantNodes.size(); ++variantIndex) {
            variantNode = variantNodes.at(variantIndex);
            if (variantNode.isNull()) {
                qDebug() << "variant node is null.";
                continue;
            }
//            qDebug() << "tag name of variant node: " << variantNode.nodeName();
            QDomElement variantConfigElement = variantNode.firstChildElement("configItem");
            if (variantConfigElement.isNull()) {
                qDebug() << "variant config element is null.";
                continue;
            }
            QDomElement variantNameElement = variantConfigElement.firstChildElement("name");
//            qDebug() << "variant name: " << variantNameElement.text();
            tmpVariantItem.name = variantNameElement.text();
            QDomElement variantDescriptionElement = variantConfigElement.firstChildElement("description");
            tmpVariantItem.description = variantDescriptionElement.text();
//            qDebug() << "variant description: " << variantDescriptionElement.text();

            tmpLayoutItem.variantItemList.append(tmpVariantItem);
        }
        KeyboardLayoutList.append(tmpLayoutItem);
    }
    return true;
}

XkbParser::~XkbParser() {

}
