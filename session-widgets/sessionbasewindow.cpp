#include "sessionbasewindow.h"

#include <QDebug>
#include <QScrollArea>
#include <QResizeEvent>
#include <QApplication>

SessionBaseWindow::SessionBaseWindow(QWidget *parent)
    : QFrame(parent)
    , m_centerWidget(nullptr)
    , m_leftBottomWidget(nullptr)
    , m_rightBottomWidget(nullptr)
{
    initUI();
}

void SessionBaseWindow::setLeftBottomWidget(QWidget * const widget)
{
    if (m_leftBottomWidget != nullptr) {
        m_leftBottomLayout->removeWidget(m_leftBottomWidget);
    }

    m_leftBottomLayout->addWidget(widget, 0, Qt::AlignBottom);
    m_leftBottomWidget = widget;
}

void SessionBaseWindow::setRightBottomWidget(QWidget * const widget)
{
    if (m_rightBottomWidget != nullptr) {
        m_rightBottomLayout->removeWidget(m_rightBottomWidget);
    }

    m_rightBottomLayout->addWidget(widget, 0, Qt::AlignBottom);
    m_rightBottomWidget = widget;
}

void SessionBaseWindow::setCenterContent(QWidget * const widget)
{
    if (m_centerWidget != nullptr) {
        m_centerLayout->removeWidget(m_centerWidget);
        m_centerWidget->hide();
    }

    m_centerLayout->addWidget(widget);
    m_centerWidget = widget;

    widget->show();
}

void SessionBaseWindow::initUI()
{
    m_mainLayou = new QVBoxLayout;

    m_centerLayout = new QHBoxLayout;
    m_leftBottomLayout = new QHBoxLayout;
    m_rightBottomLayout = new QHBoxLayout;

    m_centerLayout->setMargin(0);
    m_centerLayout->setSpacing(0);

    m_leftBottomLayout->setMargin(0);
    m_leftBottomLayout->setSpacing(0);

    m_rightBottomLayout->setMargin(0);
    m_rightBottomLayout->setSpacing(0);

    QFrame *bottomWidget = new QFrame;
    m_scrollArea = new QScrollArea;
    QWidget *centerWidget = new QWidget;
    centerWidget->setLayout(m_centerLayout);

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFocusPolicy(Qt::NoFocus);
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet("background: transparent;");
    m_scrollArea->setWidget(centerWidget);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setMargin(0);
    bottomLayout->setSpacing(0);

    bottomLayout->addLayout(m_leftBottomLayout);
    bottomLayout->addStretch();
    bottomLayout->addLayout(m_rightBottomLayout);

    bottomWidget->setLayout(bottomLayout);
    bottomWidget->setFixedHeight(132);

    m_mainLayou->setContentsMargins(0, 33, 0, 33);
    m_mainLayou->setSpacing(0);

    m_mainLayou->addWidget(m_scrollArea);
    m_mainLayou->addWidget(bottomWidget, 0, Qt::AlignBottom);

    setLayout(m_mainLayou);
}

