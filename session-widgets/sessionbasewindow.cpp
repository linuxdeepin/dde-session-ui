#include "sessionbasewindow.h"

SessionBaseWindow::SessionBaseWindow(QWidget *parent)
    : QFrame(parent)
    , m_centerContent(nullptr)
    , m_leftBottomWidget(nullptr)
    , m_rightBottomWidget(nullptr)
{
    initUI();
}

void SessionBaseWindow::setLeftBottomWidget(QWidget * const widget)
{
    if (m_leftBottomWidget != nullptr) {
        m_leftBottomLayout->removeWidget(m_leftBottomWidget);
        m_leftBottomWidget->deleteLater();
    }

    m_leftBottomLayout->addWidget(widget);
    m_leftBottomWidget = widget;
}

void SessionBaseWindow::setRightBottomWidget(QWidget * const widget)
{
    if (m_rightBottomWidget != nullptr) {
        m_rightBottomLayout->removeWidget(m_rightBottomWidget);
        m_rightBottomWidget->deleteLater();
    }

    m_rightBottomLayout->addWidget(widget);
    m_rightBottomWidget = widget;
}

void SessionBaseWindow::setCenterContent(QWidget * const widget)
{
    if (m_centerContent != nullptr) {
        m_centerLayout->removeWidget(m_centerContent);
        m_centerContent->deleteLater();
    }

    m_centerLayout->addWidget(widget);
    m_centerContent = widget;
}

void SessionBaseWindow::initUI()
{
    m_mainLayou = new QVBoxLayout;

    m_leftBottomLayout = new QHBoxLayout;
    m_rightBottomLayout = new QHBoxLayout;
    m_centerLayout = new QHBoxLayout;

    QFrame *topWidget = new QFrame;
    QWidget *bottomWidget = new QWidget;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setSpacing(0);

    bottomLayout->addSpacing(50);
    bottomLayout->addLayout(m_leftBottomLayout);
    bottomLayout->addStretch();
    bottomLayout->addLayout(m_rightBottomLayout);
    bottomLayout->addSpacing(50);

    bottomWidget->setLayout(bottomLayout);
    bottomWidget->setFixedHeight(300);

    m_mainLayou->setMargin(0);
    m_mainLayou->setSpacing(0);

    m_mainLayou->addWidget(topWidget, 0, Qt::AlignTop);
    m_mainLayou->addLayout(m_centerLayout);
    m_mainLayou->addWidget(bottomWidget, 0, Qt::AlignBottom);

    setLayout(m_mainLayou);
}

