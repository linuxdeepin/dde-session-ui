#ifndef UTIL_PARSEXML
#define UTIL_PARSEXML
#include <QtCore>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QFile>

class ParseXML:public QObject
{
    Q_OBJECT
public:
    ParseXML(QString xmlFile, QString tag, QString node_key, QString node_value, QObject* parent = 0);
    ~ParseXML();
    QMap<QString, QString> getTagNodeInfo();
    QMap<QString, QString> nodeInfo;
    QString m_xmlFile;
    QString m_tag;
    QString m_key;
    QString m_value;
};
#endif // UTIL_PARSEXML

