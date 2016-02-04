/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QtCore/QObject>

#include "rounditembutton.h"
#include "util_signalmanager.h"

RoundItemButton::RoundItemButton(const QString &text, QWidget* parent)
    : QAbstractButton(parent),
      m_itemIcon(new QLabel(this)),
      m_itemText(new QLabel(this))
{
    m_itemText->setText(text);
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    m_opacityEffect->setOpacity(1.0);
    setGraphicsEffect(m_opacityEffect);

    initUI();
    initConnect();
}

RoundItemButton::~RoundItemButton()
{
}

void RoundItemButton::setDisabled(bool disabled)
{
    if (!disabled)
        updateState(Normal);
    else
        updateState(Disabled);

    QAbstractButton::setDisabled(disabled);

    // update qss
    setStyleSheet(styleSheet());

    // update opacity
    m_opacityEffect->setOpacity(disabled ? 0.5 : 1.0);
}

void RoundItemButton::setChecked(bool checked)
{
    if (checked)
        updateState(Checked);
    else
        updateState(Normal);
}

void RoundItemButton::initConnect()
{
    connect(this, &RoundItemButton::stateChanged, this, &RoundItemButton::setState, Qt::DirectConnection);
    connect(this, &RoundItemButton::stateChanged, this, &RoundItemButton::updateIcon);
    connect(this, &RoundItemButton::stateChanged, this, static_cast<void (RoundItemButton::*)()>(&RoundItemButton::update));
    connect(this, &RoundItemButton::iconChanged, this, &RoundItemButton::updateIcon);
    connect(this, &RoundItemButton::toggled, this, &RoundItemButton::setChecked);
    connect(signalManager, &SignalManager::setButtonHover, [this] (const QString &text) {
        if (m_itemText->text() != text && !isChecked() && !isDisabled()) {
            updateState(Normal);
        }
    });
}

void RoundItemButton::initUI() {
    m_itemIcon->setFocusPolicy(Qt::NoFocus);
    m_itemIcon->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_itemText->setWordWrap(true);
    m_itemText->setStyleSheet("color: rgba(255, 255, 255, 255);"
                              "font-size:16px; margin:0 5px;");
    m_itemText->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    m_itemText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_itemIcon);
    mainLayout->setAlignment(m_itemIcon, Qt::AlignHCenter);
    mainLayout->addWidget(m_itemText);

    setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(mainLayout);
    setFixedSize(QSize(140, 140));
    setCheckable(true);

    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect(m_itemText);
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
//    m_itemText->setGraphicsEffect(nameShadow);
}

void RoundItemButton::enterEvent(QEvent* event)
{
    Q_UNUSED(event)

    if (m_state == Disabled)
        return;

    if (m_state == Normal)
        updateState(Hover);

    emit signalManager->setButtonHover(m_itemText->text());
}

void RoundItemButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)

    if (m_state == Disabled)
        return;

    if (m_state == Checked)
        return;

    updateState(Normal);
}

void RoundItemButton::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    updateState(Pressed);
}

void RoundItemButton::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);

    if (m_state == Checked)
        updateState(Hover);
    else
        updateState(Pressed);

    if (m_state != Disabled)
        emit clicked();
}

void RoundItemButton::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    if (m_state != Checked)
        return;

    QPainter painter(this);
    painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
    painter.setBrush(QColor(0, 0 , 0, 76));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRoundedRect(QRect(2, 2, width() - 4, height() - 4), 10, 10, Qt::RelativeSize);
}

void RoundItemButton::updateIcon()
{
    QPixmap pixmap;
    switch (m_state)
    {
    case Disabled:  /* show normal pic */
    case Normal:    pixmap.load(m_normalIcon);      break;
    case Default:
    case Hover:     pixmap.load(m_hoverIcon);       break;
    case Checked:   pixmap.load(m_normalIcon);      break;
    case Pressed:   pixmap.load(m_pressedIcon);     break;
    default:;
    }

    if (!pixmap.isNull()) {
        m_itemIcon->setPixmap(pixmap);
    }
}

void RoundItemButton::updateState(const RoundItemButton::State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }

    QAbstractButton::setChecked(m_state == Checked);
    return updateIcon();
}
