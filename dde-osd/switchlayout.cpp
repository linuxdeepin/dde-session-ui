/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchlayout.h"

static const int MinLayoutWindowWidth = 200;

SwitchLayout::SwitchLayout(QWidget *parent) : QWidget(parent)
{
    initGlobalVar(parent);
}

void SwitchLayout::initGlobalVar(QWidget *parent)
{
    m_ParentItem = parent;

    m_LayoutInterface = new LayoutDbus("com.deepin.daemon.InputDevices",
                                       "/com/deepin/daemon/InputDevice/Keyboard",
                                       QDBusConnection::sessionBus(), this);

    m_ListWidget = new QListWidget(parent);

    m_KeyboardList = m_LayoutInterface->userLayoutList();

    connect(m_LayoutInterface, &LayoutDbus::UserLayoutListChanged, [this] {
        m_KeyboardList = m_LayoutInterface->userLayoutList();
        calculateKeyboardSize();
        if (m_ListWidget && m_HBoxLayout) {
            fillListWidget();
        }
    });
}

void SwitchLayout::deleteOsd()
{
    m_ParentItem->hide();
}

void SwitchLayout::hideLayout()
{
    m_ListWidget->setVisible(false);
}

void SwitchLayout::showLayout()
{
    m_ListWidget->setVisible(true);
}

int SwitchLayout::layoutCount()
{
    return m_KeyboardList.length();
}

void SwitchLayout::setKeyboard()
{
    m_LayoutInterface->setCurrentLayout(m_KeyboardList[m_CurrentIndexOfKeyBoard]);
}

void SwitchLayout::loadSwitchLayout()
{
    if (m_KeyboardList.length() > 1) {
        // give out the value of m_MaxTextWidth and m_KeyboradLayoutHeight, to help resize this app
        calculateKeyboardSize();

        resizeParent();

        initKeyboard();
    } else {
        qDebug() << "hide parent";
        // if user's keyboard layout(s) just contain(s) 1 kind, quit the app immediately
        hide();
        deleteOsd();
    }
}

bool SwitchLayout::isPanelVailed()
{
    m_KeyboardList = m_LayoutInterface->userLayoutList();
    return m_KeyboardList.length() > 1;
}

void SwitchLayout::calculateKeyboardSize()
{
    m_MaxTextWidth = 0;
    // set font-size to 14px, so that we can calculate the max width of all keyboardlayout texts
    QLabel *text = new QLabel;
    m_f = text->font();
    m_f.setPixelSize(14);
    text->setFont(m_f);

    int length = m_KeyboardList.length();
    // give the value of m_KeyboradLayoutHeight
    m_KeyboradLayoutHeight = (length > 5 ? KEYBOARD_ITEM_HEIGHT * 5 : KEYBOARD_ITEM_HEIGHT * length);

    // give the value of m_MaxTextWidth
    QFontMetrics metrics(text->font());
    for (int i = 0; i < length; i++) {
        text->setText(m_LayoutInterface->GetLayoutDesc(m_KeyboardList[i]));
        int textWidth = metrics.boundingRect(text->text()).width();
        m_MaxTextWidth = qMax(MinLayoutWindowWidth, qMax(textWidth, m_MaxTextWidth));
    }
}

void SwitchLayout::resizeParent()
{
    m_ParentItem->resize(m_MaxTextWidth + LAYOUT_MARGIN * 4, m_KeyboradLayoutHeight + LAYOUT_MARGIN * 2);
}

// Switch to next layout and don't show the UI.
void SwitchLayout::directSwitchNextLayout()
{
    const QString currentLayout = m_LayoutInterface->currentLayout();
    const int nextIndex = (m_KeyboardList.indexOf(currentLayout) + 1 ) % m_KeyboardList.length();
    m_LayoutInterface->setCurrentLayout(m_KeyboardList.at(nextIndex));

    // set this state property so that the normal behavior of this OSD won't go wrong.
    // sigh :(
    m_CurrentIndexOfKeyBoard = nextIndex;
    if (isVisible()) {
        reHiglightKeyboard();
    }
}

void SwitchLayout::initKeyboard()
{
    initListWidget();

    fillListWidget();

    addAnimation();
}

void SwitchLayout::initListWidget()
{
    m_ListWidget->setVisible(true);

    // variable "size" is the size of QListWidgetItem* item and QWidget* customItem
    size = QSize(m_MaxTextWidth + LAYOUT_MARGIN * 2, KEYBOARD_ITEM_HEIGHT);

    // hLayout is used to wrap m_ListWidget
    m_HBoxLayout = new QHBoxLayout(m_ParentItem);
    // make sure m_ListWidget's margins be 10px
    m_HBoxLayout->setContentsMargins(10, 10, 10, 10);

    // set m_ListWidget's parameters
    m_ListWidget->setStyleSheet("background:transparent");
    m_ListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setFrameStyle(QFrame::NoFrame);
    m_ListWidget->resize(m_MaxTextWidth + LAYOUT_MARGIN * 2, m_KeyboradLayoutHeight);

}

void SwitchLayout::fillListWidget()
{
    m_KeyboradTextList.clear();
    m_ListWidget->clear();

    for (int i = 0, length = m_KeyboardList.length(); i < length; i++) {
        QListWidgetItem *item = new QListWidgetItem;
        // setFlags(Qt::NoItemFlags) can remove the default highlight
        item->setFlags(Qt::NoItemFlags);
        item->setSizeHint(size);

        // customItem and text within it
        QWidget *customItem = new QWidget;
        customItem->setFixedSize(size);
        QLabel *text = new QLabel(customItem);
        text->setFont(m_f);
        m_KeyboradTextList << text;
        // make sure left-margin and right-margin be 10px
        text->setContentsMargins(10, 0, 10, 0);
        text->setFixedSize(size);
        text->setText(m_LayoutInterface->GetLayoutDesc(m_KeyboardList[i]));

        // highlight the chosen customItem and text withint it, when app starts
        if (m_LayoutInterface->currentLayout() == m_KeyboardList[i]) {
            m_CurrentIndexOfKeyBoard = i;
            text->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        } else {
            text->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
        }

        m_ListWidget->addItem(item);
        m_ListWidget->setItemWidget(item, customItem);
    }

    m_HBoxLayout->addWidget(m_ListWidget);
}

void SwitchLayout::addAnimation()
{
    // make sure that the highlighted item can be displayed in view after app startss
    m_ListWidget->scrollToItem(m_ListWidget->item(m_CurrentIndexOfKeyBoard));
    // the following codes are about animation
    contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
    m_Animation = new QVariantAnimation(m_ListWidget);
    m_Animation->setDuration(70);

    // when currentrow changes, check if new_contentY is different from contentY. If different,m_animation should start
    connect(m_ListWidget, &QListWidget::currentRowChanged,
    this, [this] {
        int new_contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
        if (new_contentY != contentY  && m_KeyboardList.length() > 5)
        {
            m_Animation->setStartValue(QVariant::fromValue(contentY));
            m_Animation->setEndValue(QVariant::fromValue(new_contentY));

            contentY = new_contentY;

            m_Animation->start();
        }
    });

    connect(m_Animation, &QVariantAnimation::valueChanged, this, [this](const QVariant & value) {
        int contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
        for (int i = 0; i < m_KeyboardList.length(); i++) {
            QWidget *w = m_ListWidget->itemWidget(m_ListWidget->item(i));
            w->move(0, w->y() - contentY + value.toInt());
        }
    });
}

void SwitchLayout::highlightNextLayout()
{
    reAlignCurrentIndex();

    reHiglightKeyboard();
}

void SwitchLayout::reAlignCurrentIndex()
{
    if (m_CurrentIndexOfKeyBoard == m_KeyboardList.length() - 1) {
        m_CurrentIndexOfKeyBoard = 0;

        // make sure the listwidgetitem is positioned as initial after one-cycle selection
        if (m_KeyboardList.length() > 5) {
            for (int i = 0, length = m_KeyboardList.length(); i < length; i++) {
                QWidget *w = m_ListWidget->itemWidget(m_ListWidget->item(i));
                w->move(0, KEYBOARD_ITEM_HEIGHT * (i + 5 - length));
            }
        }
    } else {
        m_CurrentIndexOfKeyBoard++;
    }

    // apply changes
    setKeyboard();
}

void SwitchLayout::reHiglightKeyboard()
{
    if (m_KeyboradTextList.isEmpty()) return;

    if (m_CurrentIndexOfKeyBoard == 0) {
        // if m_CurrentIndexOfKeyBoard == 0,highlight the first one, and normalize the last one
        m_KeyboradTextList[0]->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        m_KeyboradTextList[m_KeyboradTextList.length() - 1]->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
    } else {
        // highlight the current one, normalize the old one
        m_KeyboradTextList[m_CurrentIndexOfKeyBoard]->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        m_KeyboradTextList[m_CurrentIndexOfKeyBoard - 1]->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
    }
    m_ListWidget->setCurrentRow(m_CurrentIndexOfKeyBoard);
}
