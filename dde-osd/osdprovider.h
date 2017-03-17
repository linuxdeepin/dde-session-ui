#ifndef OSDPROVIDER_H
#define OSDPROVIDER_H

#include <QObject>
#include <QSize>

#include "common.h"

class AbstractOSDProvider : public QObject
{
    Q_OBJECT
public:
    explicit AbstractOSDProvider(QObject *parent = 0);

    // match tell if we should handle the param.
    virtual bool match(const QString &param) const;

    // check whether the conditions are all met so we can show
    // our content.
    virtual bool checkConditions() const;

    // provide hint about how many space we will take.
    virtual QSize contentSize() const;

    // provide data.
    virtual QList<QPair<QString, QString>> data() const;

    // provide style.
    virtual Style style() const;

signals:
    void dataChanged();

protected:
    QStringList m_suitableParams;
};

#endif // OSDPROVIDER_H
