#include "wmstateprovider.h"

WMStateProvider::WMStateProvider(QObject *parent)
    : AbstractOSDProvider(parent)
{
    m_suitableParams << "SwitchWM3D" << "SwitchWM2D" << "SwitchWMError";
}

int WMStateProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant WMStateProvider::data(const QModelIndex &, int role) const
{
    const bool decoration = role == Qt::DecorationRole;
    if (m_param == "SwitchWM3D") {
        return decoration ? ":/icons/OSD_wm_3d" : tr("Window effect enabled");
    } else if (m_param == "SwitchWM2D") {
        return decoration ? ":/icons/OSD_wm_2d" : tr("Window effect disabled");
    }
    return decoration ? ":/icons/OSD_wm_failed" : tr("Failed to enable window effects");
}

void WMStateProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant pixPath = index.data(Qt::DecorationRole);
    QVariant textData = index.data();

    DrawHelper::DrawImage(painter, option, QPixmap(pixPath.toString()), true);
    DrawHelper::DrawText(painter, option, textData.toString());
}

QSize WMStateProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

bool WMStateProvider::match(const QString &param)
{
    m_param = param;
    return m_suitableParams.contains(param);
}
