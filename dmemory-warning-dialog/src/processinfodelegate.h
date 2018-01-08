#ifndef PROCESSINFODELEGATE_H
#define PROCESSINFODELEGATE_H

#include <QItemDelegate>

class ProcessInfoDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ProcessInfoDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PROCESSINFODELEGATE_H
