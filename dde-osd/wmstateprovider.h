#ifndef WMSTATEPROVIDER_H
#define WMSTATEPROVIDER_H

#include "osdprovider.h"

class WMStateProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit WMStateProvider(QObject *parent = 0);

    // model
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    // delegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool match(const QString &param) Q_DECL_OVERRIDE;

private:
    QString m_param;
};

#endif // WMSTATEPROVIDER_H
