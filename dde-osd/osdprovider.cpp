#include "osdprovider.h"

AbstractOSDProvider::AbstractOSDProvider(QObject *parent)
    : QObject(parent)
{

}

bool AbstractOSDProvider::match(const QString &param)
{
    return m_suitableParams.contains(param);
}

bool AbstractOSDProvider::checkConditions() const
{
    return true;
}

QSize AbstractOSDProvider::contentSize() const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

QMargins AbstractOSDProvider::contentMargins() const
{
    return QMargins();
}

QListView::Flow AbstractOSDProvider::flow() const
{
    return QListView::TopToBottom;
}

void AbstractOSDProvider::highlightCurrent()
{

}

void AbstractOSDProvider::highlightNext()
{

}

void AbstractOSDProvider::sync()
{

}
