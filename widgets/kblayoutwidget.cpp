/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include "kblayoutwidget.h"

LayoutButton::LayoutButton(QString text, QWidget *parent)
    : QPushButton(parent)
{
    setObjectName("LayoutButton");
    setCheckable(true);

    m_text = text;
    m_iconLabel = new QLabel(this);
    QPixmap pix(":img/tick_hover.png");
    m_iconLabel->setPixmap(pix);
    m_iconLabel->setMinimumSize(pix.size());
    m_textLabel = new QLabel;
    m_textLabel->setObjectName("LayoutTextLabel");
    m_textLabel->setText(text);
    m_iconLabel->hide();

    QHBoxLayout* m_Layout = new QHBoxLayout;
    m_Layout->setContentsMargins(20, 0, 0, 0);
    m_Layout->setSpacing(0);

    m_Layout->addSpacing(5);
    m_Layout->addWidget(m_textLabel);
    m_Layout->addStretch();
    setLayout(m_Layout);

    m_iconLabel->move(this->x() + 5, this->y() + 4);


    updateStyle(":/skin/layoutbutton.qss", this);
}
void LayoutButton::enterEvent(QEvent *event) {
    emit mouseEnter(m_text);
    Q_UNUSED(event);
//    qDebug() << "enterEvent;";
    updateStyleSelf();
}

void LayoutButton::leaveEvent(QEvent *event) {
    emit mouseLeave(m_text);
    Q_UNUSED(event);
//    qDebug() << "leaveEvent;";
     updateStyleSelf();
}

void LayoutButton::updateStyleSelf() {
    style()->unpolish(this);
    style()->polish(this);
    this->update();
}

void LayoutButton::setButtonChecked(bool checked) {
    if (checked) {
        this->setChecked(true);
        m_textLabel->setProperty("Checked", true);
        m_iconLabel->show();
        updateStyle(":/skin/layoutbutton.qss", this);
    } else {
        this->setChecked(false);
        m_textLabel->setProperty("Checked", false);
        updateStyle(":/skin/layoutbutton.qss", this);
        m_iconLabel->hide();
    }

    updateStyleSelf();

}

void LayoutButton::OnlyMeChecked(bool checked) {
    setButtonChecked(checked);
    qDebug() << "onLyMeChecked" << m_text;
    emit onlyOneChecked(m_text);
}

LayoutButton::~LayoutButton()
{
}

KbLayoutWidget::KbLayoutWidget(QStringList buttons, QWidget *parent)
    : QListWidget(parent)
{
    if (!buttons.isEmpty()) {
        m_buttons.clear();
        m_buttons = buttons;
    }

    initUI();
    updateButtonList(buttons);
    updateUI();

}

void KbLayoutWidget::initUI() {
    setObjectName("KeyboardLayoutFrame");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);
    setResizeMode(Adjust);

    for (int i = 0; i < m_buttons.length(); i++) {
        addButton(m_buttons[i]);
    }
    this->setFixedHeight(DDESESSIONCC::LAYOUTBUTTON_HEIGHT * std::min(3, m_buttons.length()));

    updateStyle(":/skin/keybdlayoutwidget.qss", this);
}

void KbLayoutWidget::setButtonsChecked(QString text) {
    qDebug() << "XsetButtonsChecked:" << text;
    for (int i = 0; i < m_layoutButtons.length(); i++) {
        if (m_layoutButtons.at(i)->m_text != text) {
            m_layoutButtons.at(i)->setButtonChecked(false);
        } else {
            m_layoutButtons.at(i)->setButtonChecked(true);
        }
    }

    emit setButtonClicked(text);
}

void KbLayoutWidget::setListItemChecked(int itemIndex) {
    qDebug() << "m_layoutbuttons.length:" << m_layoutButtons.length() << itemIndex;
    if (itemIndex <  m_layoutButtons.length()) {
        QString itemText = m_layoutButtons.at(itemIndex)->m_text;
        qDebug() << "itemIndex itemText:" << itemIndex << itemText;
        setButtonsChecked(itemText);
    }
}

void KbLayoutWidget::updateButtonList(const QStringList &buttons)
{
    if (buttons == m_buttons)
        return;

    m_buttons = buttons;

    for (LayoutButton* button : m_layoutButtons)
        button->deleteLater();

    m_layoutButtons.clear();

    for (auto it(m_layoutItemList.begin()); it != m_layoutItemList.end(); ++it) {
        removeItemWidget(it.key());

        if (it.key())
            delete it.key();

        if (it.value())
            delete it.value();
    }

    m_layoutItemList.clear();

    for (int i = 0; i < m_buttons.length(); i++) {
        addButton(m_buttons[i]);
    }
    this->setFixedHeight(DDESESSIONCC::LAYOUTBUTTON_HEIGHT * std::min(3, m_buttons.length()));

    updateStyle(":/skin/keybdlayoutwidget.qss", this);
}

void KbLayoutWidget::addButton(const QString &button)
{
    LayoutButton* itemButton = new LayoutButton(button);
    m_layoutButtons.append(itemButton);
    itemButton->setFixedSize(widget_width, DDESESSIONCC::LAYOUTBUTTON_HEIGHT);

    QFrame *borderFrame = new QFrame;
    borderFrame->setObjectName("LayoutBorderFrame");
    QVBoxLayout* borderLayout = new QVBoxLayout;
    borderLayout->setContentsMargins(0, 0, 0, 0);
    borderLayout->setSpacing(0);
    borderLayout->addWidget(itemButton);
    borderFrame->setLayout(borderLayout);
    borderFrame->setFixedSize(widget_width, DDESESSIONCC::LAYOUTBUTTON_HEIGHT);

    QListWidgetItem* item = new QListWidgetItem(this);
    item->sizeHint();
    this->addItem(item);
    setItemWidget(item,  borderFrame);
    m_layoutItemList.insert(item, borderFrame);
    this->setGridSize(QSize(widget_width, DDESESSIONCC::LAYOUTBUTTON_HEIGHT));
    this->setFixedWidth(widget_width);
    updateStyle(":/skin/layoutbutton.qss", this);

    connect(itemButton, &LayoutButton::clicked, itemButton, &LayoutButton::OnlyMeChecked, Qt::UniqueConnection);
    connect(itemButton, &LayoutButton::onlyOneChecked, this, &KbLayoutWidget::setButtonsChecked, Qt::UniqueConnection);
    connect(itemButton, &LayoutButton::onlyOneChecked, this, &KbLayoutWidget::setButtonClicked, Qt::UniqueConnection);
}

void KbLayoutWidget::updateUI() {

}
KbLayoutWidget::~KbLayoutWidget(){
}
