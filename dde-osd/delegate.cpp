#include "delegate.h"

#include <QPainter>
#include <QDebug>

#include "common.h"

Delegate::Delegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

void Delegate::setStyle(const Style &style)
{
    qDebug() << style;
    m_style = style;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant textData = index.data(Qt::DisplayRole);
    QVariant imageData = index.data(Qt::DecorationRole);

    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    QPixmap pix( imageData.toString() );
    drawPixmap(painter, option, pix);

    float progress( textData.toString().toFloat() );
    drawProgressBar(painter, option, progress);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

void Delegate::drawPixmap(QPainter *painter, const QStyleOptionViewItem &option, QPixmap pix) const
{
    if (m_style == StyleVList) return;

    const QRect rect( option.rect );
    const int yOffset = m_style == StyleImage ? 40 : 30;
    painter->drawPixmap(rect.x() + (rect.width() - pix.width()) / 2, rect.y() + yOffset, pix);
}

void Delegate::drawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, float progress) const
{
    if (m_style != StyleImageProg) return;

    const QRect rect( option.rect );
    const int progressBarWidth = 80;
    const int progressBarHeight = 4;
    const float radius = progressBarHeight / 2.f;

    painter->setPen(Qt::NoPen);

    QRect dest(rect.x() + (rect.width() - progressBarWidth) / 2, rect.y() + 110, progressBarWidth, progressBarHeight);
    painter->setBrush(QColor(0, 0, 0, 25));
    painter->drawRoundedRect(dest, radius, radius);

    dest.setWidth(dest.width() * progress);
    painter->setBrush(Qt::black);
    painter->drawRoundedRect(dest, radius, radius);
}

void Delegate::drawText(QPainter *painter, const QStyleOptionViewItem &option, QPixmap pix) const
{

}
