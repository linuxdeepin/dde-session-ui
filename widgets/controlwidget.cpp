#include "controlwidget.h"

#include <QHBoxLayout>
#include <dimagebutton.h>
#include <QEvent>
#include <QWheelEvent>

DWIDGET_USE_NAMESPACE

ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
}

void ControlWidget::initUI()
{
    m_mediaWidget = nullptr;

    m_mainLayout = new QHBoxLayout;

    m_switchUserBtn = new DImageButton;
    m_switchUserBtn->setNormalPic(":/img/bottom_actions/userswitch_normal.png");
    m_switchUserBtn->setHoverPic(":/img/bottom_actions/userswitch_hover.png");
    m_switchUserBtn->setPressPic(":/img/bottom_actions/userswitch_press.png");

    m_powerBtn = new DImageButton;
    m_powerBtn->setNormalPic(":/img/bottom_actions/shutdown_normal.png");
    m_powerBtn->setHoverPic(":/img/bottom_actions/shutdown_hover.png");
    m_powerBtn->setPressPic(":/img/bottom_actions/shutdown_press.png");

    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(26);
    m_mainLayout->addWidget(m_switchUserBtn);
    m_mainLayout->setAlignment(m_switchUserBtn, Qt::AlignBottom);
    m_mainLayout->addWidget(m_powerBtn);
    m_mainLayout->setAlignment(m_powerBtn, Qt::AlignBottom);
    m_mainLayout->addSpacing(28);

    setLayout(m_mainLayout);
    setFixedSize(500, 150);
}

void ControlWidget::initConnect()
{
    connect(m_switchUserBtn, &DImageButton::clicked, this, &ControlWidget::requestSwitchUser);
    connect(m_powerBtn, &DImageButton::clicked, this, &ControlWidget::requestShutdown);
}

void ControlWidget::setMPRISEnable(const bool state)
{
    if (m_mediaWidget) {
        m_mediaWidget->setVisible(state);
    } else {
        m_mediaWidget = new MediaWidget;
        m_mediaWidget->initMediaPlayer();
        m_mainLayout->insertWidget(0, m_mediaWidget);
        m_mainLayout->insertStretch(0);
    }
}

void ControlWidget::setUserSwitchEnable(const bool visible)
{
    m_switchUserBtn->setVisible(visible);
}
