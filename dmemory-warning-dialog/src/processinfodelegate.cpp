#include "processinfodelegate.h"
#include "processinfomodel.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>

ProcessInfoDelegate::ProcessInfoDelegate(QObject *parent)
    : QItemDelegate(parent)
{

}

void ProcessInfoDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != COLUMN_ICON)
        return QItemDelegate::paint(painter, option, index);

    const QPixmap pix = index.data(ProcessInfoModel::IconRole).value<QPixmap>();
    painter->drawPixmap(option.rect.center() - pix.rect().center() / pix.devicePixelRatio(), pix);
}
