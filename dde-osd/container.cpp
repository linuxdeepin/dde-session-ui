#include "container.h"

#include <QHBoxLayout>

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

    m_layout = new QHBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    setLayout(m_layout);
}

void Container::setContent(QWidget *content)
{
    m_layout->addWidget(content);
}

