#include "brightnessprovider.h"

BrightnessProvider::BrightnessProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_brightness(0),
      m_displayInter(new com::deepin::daemon::Display("com.deepin.daemon.Display",
                                                      "/com/deepin/daemon/Display",
                                                      QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "BrightnessUp" << "BrightnessDown";

    m_displayInter->setSync(false);
    connect(m_displayInter, &__Display::BrightnessChanged,
            this, &BrightnessProvider::brightnessChanged);
    brightnessChanged(m_displayInter->brightness());
}

Style BrightnessProvider::style() const
{
    return StyleImageProg;
}

int BrightnessProvider::rowCount(const QModelIndex &) const
{
    return 1;
}

QVariant BrightnessProvider::data(const QModelIndex &, int role) const
{
    if (role == Qt::DecorationRole) {
        return ":/icons/OSD_light.png";
    }
    return m_brightness;
}

void BrightnessProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant progressData = index.data(Qt::DisplayRole);

    DrawImage(painter, option, QPixmap(imageData.toString()), true);
    DrawProgressBar(painter, option, progressData.toDouble());
}

QSize BrightnessProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

void BrightnessProvider::setBrightness(double brightness)
{
    if (brightness == m_brightness) return;

    m_brightness = brightness;
    emit dataChanged();
}

void BrightnessProvider::brightnessChanged(const BrightnessMap &brightness)
{
    if (brightness.values().length() == 0) return;
    setBrightness(brightness.values().at(0));
}
