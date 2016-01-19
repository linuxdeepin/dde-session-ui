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
