#include "osdprovider.h"

AbstractOSDProvider::AbstractOSDProvider(QObject *parent)
    : QObject(parent)
{

}

bool AbstractOSDProvider::match(const QString &param) const
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

Style AbstractOSDProvider::style() const
{
    return StyleImage;
}

void AbstractOSDProvider::highlightCurrent()
{

}

void AbstractOSDProvider::highlightNext()
{

}
