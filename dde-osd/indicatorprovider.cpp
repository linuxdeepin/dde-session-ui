#include "indicatorprovider.h"

IndicatorProvider::IndicatorProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_suitableParams << "CapsLockOn" << "CapsLockOff" << "NumLockOn" << "NumLockOff";
    m_suitableParams << "TouchpadOn" << "TouchpadOff";
}

int IndicatorProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant IndicatorProvider::data(const QModelIndex &, int) const
{
    if (m_param == "CapsLockOn") {
        return ":/icons/OSD_caps_lock_on";
    } else if (m_param == "CapsLockOff") {
        return ":/icons/OSD_caps_lock_off";
    } else if (m_param == "NumLockOn") {
        return ":/icons/OSD_num_lock_on";
    } else if (m_param == "NumLockOff") {
        return ":/icons/OSD_num_lock_off";
    } else if (m_param == "TouchpadOn") {
        return ":/icons/OSD_trackpad_on";
    } else if (m_param == "TouchpadOff") {
        return ":/icons/OSD_trackpad_off";
    }

    return "";
}

void IndicatorProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant pixData = index.data(Qt::DecorationRole);
    DrawHelper::DrawImage(painter, option, QPixmap(pixData.toString()));
}

QSize IndicatorProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool IndicatorProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParams.contains(param);
}
