#ifndef BRIGHTNESSPROVIDER_H
#define BRIGHTNESSPROVIDER_H

#include "osdprovider.h"

#include <com_deepin_daemon_display.h>

#include "common.h"

class BrightnessProvider : public AbstractOSDProvider
{
    Q_OBJECT
public:
    explicit BrightnessProvider(QObject *parent = 0);

    QList<QPair<QString, QString>> data() const Q_DECL_OVERRIDE;
    Style style() const Q_DECL_OVERRIDE;

    void setBrightness(double brightness);

private slots:
    void brightnessChanged(const BrightnessMap &brightness);

private:
    double m_brightness;

    com::deepin::daemon::Display *m_displayInter;
};

#endif // BRIGHTNESSPROVIDER_H
