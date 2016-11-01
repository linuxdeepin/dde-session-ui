/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchframe.h"

#include <QEvent>
#include <QDebug>
#include <QFile>
#include <QGraphicsDropShadowEffect>

DWIDGET_USE_NAMESPACE

ItemButton::ItemButton(QWidget* parent)
    : QPushButton(parent),
      m_itemIcon(new QLabel(this))
{
    initUI();
    initConnect();
}

ItemButton::~ItemButton()
{
}

void ItemButton::setDisabled(bool disabled)
{
    if (!disabled)
        updateState(Normal);
    else
        updateState(Disabled);

    QAbstractButton::setDisabled(disabled);

    // update qss
//    setStyleSheet(styleSheet());

    // update opacity
}

void ItemButton::setChecked(bool checked)
{
    if (checked)
        updateState(Checked);
    else
        updateState(Normal);
}

void ItemButton::initConnect()
{
    connect(this, &ItemButton::stateChanged, this, &ItemButton::setState, Qt::DirectConnection);
    connect(this, &ItemButton::stateChanged, this, &ItemButton::updateIcon);
    connect(this, &ItemButton::stateChanged, this, static_cast<void (ItemButton::*)()>(&ItemButton::update));
    connect(this, &ItemButton::iconChanged, this, &ItemButton::updateIcon);
    connect(this, &ItemButton::toggled, this, &ItemButton::setChecked);
}

void ItemButton::initUI() {
    m_itemIcon->setFocusPolicy(Qt::NoFocus);
    this->setStyleSheet("QPushButton { border: none; }");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addSpacing(0);
    mainLayout->addWidget(m_itemIcon);
    mainLayout->setAlignment(m_itemIcon, Qt::AlignCenter);

    setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(mainLayout);
}

void ItemButton::enterEvent(QEvent* event)
{
    Q_UNUSED(event)

    if (m_state == Disabled)
        return;

    if (m_state == Normal)
        updateState(Hover);
}

void ItemButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)

    if (m_state == Disabled)
        return;

    if (m_state == Checked)
        return;

    updateState(Normal);
}

void ItemButton::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);

    updateState(Pressed);
}

void ItemButton::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);

    if (m_state == Checked)
        updateState(Hover);
    else
        updateState(Pressed);

    if (m_state != Disabled)
        emit clicked();
}

void ItemButton::updateIcon()
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
        qDebug() << "item Icon:" << m_normalIcon;
        m_itemIcon->setPixmap(pixmap);
        update();
    }
}

void ItemButton::updateState(const ItemButton::State state)
{
    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }

    QAbstractButton::setChecked(m_state == Checked);
    return updateIcon();
}

SwitchFrame::SwitchFrame(QWidget* parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

SwitchFrame::~SwitchFrame()
{
}

void SwitchFrame::chooseToSession(const QString &session)
{
    qDebug() << "choosed session: " << session;
    if (session.isEmpty())
        return;

    m_tipsLabel->setText(session);

    const QString sessionId = session.toLower();
    const QString normalIcon = QString(":/img/%1_indicator_normal.png").arg(sessionId);
    const QString hoverIcon = QString(":/img/%1_indicator_hover.png").arg(sessionId);
    const QString checkedIcon = QString(":/img/%1_indicator_press.png").arg(sessionId);

    if (QFile(normalIcon).exists() && QFile(hoverIcon).exists() && QFile(checkedIcon).exists()) {
#ifndef SHENWEI_PLATFORM
        m_switchSessionButton->setNormalPic(normalIcon);
        m_switchSessionButton->setHoverPic(hoverIcon);
        m_switchSessionButton->setPressPic(checkedIcon);
#else
        m_switchSessionButton->setProperty("normalIcon", normalIcon);
        m_switchSessionButton->setProperty("hoverIcon", hoverIcon);
        m_switchSessionButton->setProperty("checkedIcon", checkedIcon);
#endif
    } else {
#ifndef SHENWEI_PLATFORM
        m_switchSessionButton->setNormalPic(":/img/unknow_indicator_normal.png");
        m_switchSessionButton->setHoverPic(":/img/unknow_indicator_hover.png");
        m_switchSessionButton->setPressPic(":/img/unknow_indicator_press.png");
#else
        m_switchSessionButton->setProperty("normalIcon", ":/img/unknow_indicator_normal.png");
        m_switchSessionButton->setProperty("hoverIcon", ":/img/unknow_indicator_hover.png");
        m_switchSessionButton->setProperty("checkedIcon", ":/img/unknow_indicator_press.png");
#endif
    }
}

void SwitchFrame::hideTips()
{
#ifndef SHENWEI_PLATFORM
    m_tipsAni->setEndValue(QPoint(m_tipsWidget->width(), 0));
    m_tipsAni->setStartValue(QPoint());
    m_tipsAni->start();
#else
    m_tipsLabel->move(m_tipsWidget->width(), 0);
#endif
}
void SwitchFrame::showTips()
{
#ifndef SHENWEI_PLATFORM
    m_tipsAni->setStartValue(QPoint(m_tipsWidget->width(), 0));
    m_tipsAni->setEndValue(QPoint());
    m_tipsAni->start();
#else
    m_tipsLabel->move(0, 0);
#endif
}

bool SwitchFrame::eventFilter(QObject *o, QEvent *e)
{
#ifndef SHENWEI_PLATFORM
    if (o == m_switchSessionButton) {
        if (e->type() == QEvent::Enter)
            showTips();
        else if (e->type() == QEvent::Leave)
            hideTips();
    }
#else
    Q_UNUSED(o);
    Q_UNUSED(e);
#endif
    return false;
}

void SwitchFrame::initConnect() {
#ifndef SHENWEI_PLATFORM
    connect(m_switchUserButton, &DImageButton::clicked, this, &SwitchFrame::triggerSwitchUser);
    connect(m_powerButton, &DImageButton::clicked, this, &SwitchFrame::triggerPower);
    connect(m_switchSessionButton, &DImageButton::clicked, this, &SwitchFrame::triggerSwitchSession);
#else
    connect(m_switchUserButton, &ItemButton::clicked, this, &SwitchFrame::triggerSwitchUser);
    connect(m_powerButton, &ItemButton::clicked, this, &SwitchFrame::triggerPower);
    connect(m_switchSessionButton, &ItemButton::clicked, this, &SwitchFrame::triggerSwitchSession);
    connect(m_switchSessionButton, &ItemButton::stateChanged, [this]{
        if (m_switchSessionButton->state() == ItemButton::State::Hover) {
            showTips();
        } else {
            hideTips();
        }
    });
#endif
}

void SwitchFrame::initUI() {
    setFixedSize(350, 100);
#ifndef SHENWEI_PLATFORM
    m_switchSessionButton = new DImageButton;
    m_switchSessionButton->setFixedSize(50, 100);
    m_switchSessionButton->installEventFilter(this);

    m_switchUserButton = new DImageButton;
    m_switchUserButton->setNormalPic(":/img/switchframe/userswitch_normal.png");
    m_switchUserButton->setHoverPic(":/img/switchframe/userswitch_hover.png");
    m_switchUserButton->setPressPic(":/img/switchframe/userswitch_press.png");
    m_switchUserButton->setFixedSize(50, 100);
    m_switchUserButton->setObjectName("SwitchUser");
    m_powerButton = new DImageButton;
    m_powerButton->setNormalPic(":/img/switchframe/shutdown_normal.png");
    m_powerButton->setHoverPic(":/img/switchframe/shutdown_hover.png");
    m_powerButton->setPressPic(":/img/switchframe/shutdown_press.png");
    m_powerButton->setFixedSize(50, 100);
    m_powerButton->setObjectName("PowerMenu");
#else
    m_switchSessionButton = new ItemButton(this);
    m_switchSessionButton->setProperty("normalIcon", ":/img/unknow_indicator_normal.png");
    m_switchSessionButton->setProperty("hoverIcon", ":/img/unknow_indicator_hover.png");
    m_switchSessionButton->setProperty("checkedIcon", ":/img/unknow_indicator_press.png");

    m_switchSessionButton->setFixedSize(40, 40);

    m_switchSessionButton->updateState(ItemButton::Normal);

    m_switchUserButton = new ItemButton(this);
    m_switchUserButton->setProperty("normalIcon", ":/img/switchframe/userswitch_normal.png");
    m_switchUserButton->setProperty("hoverIcon", ":/img/switchframe/userswitch_hover.png");
    m_switchUserButton->setProperty("checkedIcon", ":/img/switchframe/userswitch_press.png");
    m_switchUserButton->setFixedSize(40, 40);

    m_powerButton = new ItemButton(this);
    m_powerButton->setProperty("normalIcon", ":/img/switchframe/shutdown_normal.png");
    m_powerButton->setProperty("hoverIcon", ":/img/switchframe/shutdown_hover.png");
    m_powerButton->setProperty("checkedIcon", ":/img/switchframe/shutdown_press.png");
    m_powerButton->setFixedSize(40, 40);

#endif
    m_tipsWidget = new QWidget;
    m_tipsWidget->setFixedSize(80, 20);

    m_tipsLabel = new QLabel(m_tipsWidget);
    m_tipsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
#ifndef SHENWEI_PLATFORM
    m_tipsLabel->setStyleSheet("color:white;"
//                               "background-color:green;"
                               "font-size:12px;");
#else
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    m_tipsLabel->setPalette(pe);
#endif
    m_tipsLabel->setFixedSize(m_tipsWidget->size());
    m_tipsLabel->move(m_tipsWidget->width(), 0);
#ifndef SHENWEI_PLATFORM
    m_tipsAni = new QPropertyAnimation(m_tipsLabel, "pos", this);
#endif
    QGraphicsDropShadowEffect *tipsShadow = new QGraphicsDropShadowEffect(m_tipsLabel);
    tipsShadow->setColor(Qt::white);
    tipsShadow->setBlurRadius(4);
    tipsShadow->setOffset(0, 0);
    m_tipsLabel->setGraphicsEffect(tipsShadow);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(10);
    m_Layout->addStretch();

    m_Layout->addWidget(m_tipsWidget);
    m_Layout->addWidget(m_switchSessionButton);
#ifndef SHENWEI_PLATFORM
    m_Layout->setAlignment(m_switchSessionButton, Qt::AlignBottom);
#endif
    m_Layout->addWidget(m_switchUserButton);
#ifndef SHENWEI_PLATFORM
    m_Layout->setAlignment(m_switchUserButton, Qt::AlignBottom);
#endif
    m_Layout->addWidget(m_powerButton);
#ifndef SHENWEI_PLATFORM
    m_Layout->setAlignment(m_powerButton, Qt::AlignBottom);
#endif
//    m_Layout->addStretch();
    setLayout(m_Layout);
//    setStyleSheet("background-color:red;");


}
