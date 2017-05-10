#include "container.h"

#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>

#include <DPlatformWindowHandle>

Container::Container(QWidget *parent)
    : DBlurEffectWidget(parent),
      m_wmHelper(DWindowManagerHelper::instance()),
      m_supportComposite(m_wmHelper->hasComposite())
{
    setWindowFlags(Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);

    m_layout = new QHBoxLayout;
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    setLayout(m_layout);

    DPlatformWindowHandle handle(this);
    handle.setBorderColor(QColor(0, 0, 0, 0.04 * 255));
    handle.setWindowRadius(getWindowRadius());
    handle.setShadowColor(Qt::transparent);
    handle.setTranslucentBackground(true);

    setBlendMode(DBlurEffectWidget::BehindWindowBlend);
    setBlurRectXRadius(getWindowRadius());
    setBlurRectYRadius(getWindowRadius());
    setMaskColor(DBlurEffectWidget::LightColor);

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged,
            this, &Container::windowManagerChanged);
}

void Container::setContent(QWidget *content)
{
    m_layout->addWidget(content);
}

void Container::moveToCenter()
{
    QDesktopWidget *desktop = QApplication::desktop();
    const int primary = desktop->primaryScreen();
    const QRect primaryRect = desktop->screenGeometry(primary);

    move(primaryRect.center() - rect().center());
}

void Container::windowManagerChanged()
{
    m_supportComposite = m_wmHelper->hasComposite();

    updateWindowRadius();
}

void Container::updateWindowRadius()
{
    const int value = getWindowRadius();

    DPlatformWindowHandle handle(this);
    handle.setWindowRadius(value);

    setBlurRectXRadius(value);
    setBlurRectYRadius(value);
}

int Container::getWindowRadius()
{
    return m_supportComposite ? 10 : 0;
}

