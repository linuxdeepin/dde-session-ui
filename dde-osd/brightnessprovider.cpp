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

QList<QPair<QString, QString>> BrightnessProvider::data() const
{
    QList<QPair<QString,QString>> ret;
    ret << Pair(":/icons/OSD_light.png", QString::number(m_brightness));
    return ret;
}

Style BrightnessProvider::style() const
{
    return StyleImageProg;
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
