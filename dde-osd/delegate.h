#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>

#include "common.h"

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QWidget *parent = 0);

    void setStyle(const Style &style);

private:
    Style m_style;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void drawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QPixmap pix) const;
    void drawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, float progress) const;
    void drawText(QPainter *painter, const QStyleOptionViewItem &option, QPixmap pix) const;
};

#endif // DELEGATE_H
