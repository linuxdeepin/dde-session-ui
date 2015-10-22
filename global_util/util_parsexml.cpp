#include "util_parsexml.h"

ParseXML::ParseXML(QString xmlFile, QString tag, QString node_key, QString node_value, QObject *parent)
    : QObject(parent)
{
    m_xmlFile = xmlFile;
    m_tag = tag;
    m_key = node_key;
    m_value = node_value;
    qDebug() << "parsing xmlFile:" << xmlFile;
    getTagNodeInfo();
}
QMap<QString, QString> ParseXML::getTagNodeInfo() {
    QFile XMLFile(m_xmlFile);
    QStringList map_key, map_value;

    if(XMLFile.open(QIODevice::ReadOnly | QIODevice::Text)) {

        qDebug() << "xmlFile opened!";
        QXmlStreamReader reader(&XMLFile);
        while(!reader.atEnd())
        {
            ///////////if the tag is exist or not ?
            if (reader.isStartElement() && reader.name() == m_tag) {
                QXmlStreamAttributes configItem_attributes = reader.attributes();
                Q_UNUSED(configItem_attributes);
            }

            /////////////////if the key is exist or not ?
            if (reader.name() ==  m_key) {
                map_key  << reader.readElementText();
//                qDebug() << "after attribute name:" << map_key;
            /////////////////if the value is exist or not ?
            } else if (reader.name() ==  m_value) {
                map_value << reader.readElementText();
            }
            reader.readNext();
        }
//        qDebug() << map_key;
        for (int i = 0; i < map_key.length(); i++) {
//            qDebug() << "map_key_value:" << map_key[i] << map_value[i];
            if (!map_key[i].isEmpty()) {
               nodeInfo.insert(map_key[i], map_value[i]);
//               qDebug() << "MAPNODEINFO:" << nodeInfo.value(map_key[i]);
            }
        }
    } else {
        qDebug()<<"Open file hello.xml failure";
    }
    return nodeInfo;
}
ParseXML::~ParseXML() {

}
