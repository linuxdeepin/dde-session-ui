#include "delegate.h"

#include <QPainter>
#include <QDebug>

#include "common.h"
#include "osdprovider.h"

Delegate::Delegate(QWidget *parent)
    : QStyledItemDelegate(parent),
      m_provider(nullptr)
{

}

void Delegate::setProvider(AbstractOSDProvider *provider)
{
    m_provider = provider;
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(painter->renderHints() | QPainter::Antialiasing);

    if (m_provider) {
        m_provider->paint(painter, option, index);
    }
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (m_provider)
        return m_provider->sizeHint(option, index);
    return QStyledItemDelegate::sizeHint(option, index);
}
