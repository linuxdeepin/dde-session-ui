#include "container.h"

#include <DPlatformWindowHandle>

Container::Container(QWidget *parent)
    : DBlurEffectWidget(parent)
{
    setWindowFlags(Qt::SplashScreen);

    DPlatformWindowHandle handle(this);
    handle.setBorderColor(QColor(0, 0, 0, 0.04 * 255));
    handle.setWindowRadius(10);
    handle.setShadowColor(Qt::transparent);

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setBlurRectXRadius(10);
    setBlurRectYRadius(10);
    setMaskColor(QColor(245, 245, 245));
}
