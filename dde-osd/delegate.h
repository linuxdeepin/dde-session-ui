#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

#include "common.h"

class AbstractOSDProvider;
class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QWidget *parent = 0);

    void setProvider(AbstractOSDProvider *provider);

private:
    AbstractOSDProvider *m_provider;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // DELEGATE_H
