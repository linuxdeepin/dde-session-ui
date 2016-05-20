/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "switchmonitor.h"

extern QString getThemeIconPath(QString iconName);
extern void showThemeImage(QString iconName, QSvgWidget *svgLoader, QLabel *notSvgLoader);

SwitchMonitor::SwitchMonitor(QWidget *parent) : QWidget(parent)
{
    initGlobalVars(parent);
}

void SwitchMonitor::initGlobalVars(QWidget *parent)
{
    m_DisplayInterface = new DisplayDbus("com.deepin.daemon.Display",
                                         "/com/deepin/daemon/Display",
                                         QDBusConnection::sessionBus(), this);

    m_ParentItem = parent;

    m_MonitersWrapper = new QWidget(parent);

    m_HBoxLayout = new QHBoxLayout(m_MonitersWrapper);

    // init SwitchMonitors's displaymode
    if (m_DisplayInterface->displayMode() == 0) {
        displaymode = Custom;
    } else if (m_DisplayInterface->displayMode() == 1) {
        displaymode = Duplicate;
    } else if (m_DisplayInterface->displayMode() == 2) {
        displaymode = Expanded;
    } else if (m_DisplayInterface->displayMode() == 3) {
        displaymode = OneScreen;
    }
}

double SwitchMonitor::getBrightness()
{

    const QString primaryScreen = m_DisplayInterface->primary();

    if (primaryScreen.contains("=")) {
        return m_DisplayInterface->brightness()[primaryScreen.split('=').first()];
    } else {
        return m_DisplayInterface->brightness()[primaryScreen];
    }
}

void SwitchMonitor::setMonitorMode()
{
    if (m_CurrentIndexOfMonitorItem == 0) {
        // switch to duplicate mode
        m_DisplayInterface->SwitchMode(1, "");
    } else if (m_CurrentIndexOfMonitorItem == 1) {
        // switch to expanded mode
        m_DisplayInterface->SwitchMode(2, "");
    } else {
        // switch to one certain screen mode
        m_DisplayInterface->SwitchMode(3, m_ScreenList[m_CurrentIndexOfMonitorItem - 2]);
    }
}

void SwitchMonitor::hideMonitors()
{
    m_MonitersWrapper->setVisible(false);
}

void SwitchMonitor::showMonitors()
{
    m_MonitersWrapper->setVisible(true);
}

void SwitchMonitor::loadSwitchMonitors()
{
    getAllMonitors();

    if (m_ScreenList.length() > 1) {
        resizeParent();

        initMonitorItems();

        initScreenMode();
    } else {
        // if just 1 screen , quit the app immediately
        hide();
        deleteOsd();
    }
}

void SwitchMonitor::deleteOsd()
{
    m_ParentItem->hide();
}

bool SwitchMonitor::isPanelVailed()
{
    getAllMonitors();
    return m_ScreenList.length() > 1;
}

void SwitchMonitor::getAllMonitors()
{
    m_ScreenList = m_DisplayInterface->ListOutputNames();
}

void SwitchMonitor::resizeParent()
{
    m_ParentItem->resize(BASE_SIZE * (m_ScreenList.length() + 2), BASE_SIZE);
}

void SwitchMonitor::initMonitorItems()
{
    resizeMonitor();

    initDuplicateMode();

    initExtendMode();

    initOneScreenMode();
}

void SwitchMonitor::resizeMonitor()
{
    m_MonitersWrapper->setVisible(true);
    m_MonitersWrapper->resize(m_ParentItem->size());
}

void SwitchMonitor::initDuplicateMode()
{
    // for duplicate mode
    QWidget *duplicateScreenItem = new QWidget(m_MonitersWrapper);
    QVBoxLayout *vLayoutOfDuplicateScreen = new QVBoxLayout(duplicateScreenItem);
    // image label for duplicate mode
    m_DuplicateScreenImageSvg = new QSvgWidget(duplicateScreenItem);
    m_DuplicateScreenImageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_DuplicateScreenImageLabel = new QLabel(duplicateScreenItem);
    m_DuplicateScreenImageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
    // text label for duplicate mode
    m_DuplicateScreenText = new QLabel(duplicateScreenItem);
    m_DuplicateScreenText->setText(tr("Duplicate"));
    m_DuplicateScreenText->setAlignment(Qt::AlignCenter);
    m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    // add above 2 widgets
    if (getThemeIconPath("project_screen-duplicate-symbolic").endsWith(".svg")) {
        vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenImageSvg, 0, Qt::AlignHCenter);
    } else {
        vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenImageLabel, 0, Qt::AlignHCenter);
    }
    vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenText, 0, Qt::AlignHCenter);

    m_HBoxLayout->addWidget(duplicateScreenItem);
}

void SwitchMonitor::initExtendMode()
{
    // for expanded mode
    QWidget *expandedScreenItem = new QWidget(m_MonitersWrapper);
    QVBoxLayout *vLayoutOfExpandedScreen = new QVBoxLayout(expandedScreenItem);
    // image label for expanded mode
    m_ExpandedScreenImageSvg = new QSvgWidget(expandedScreenItem);
    m_ExpandedScreenImageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_ExpandedScreenImageLabel = new QLabel(expandedScreenItem);
    m_ExpandedScreenImageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    showThemeImage(getThemeIconPath("project_screen-extend-symbolic"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
    // text label for expanded mode
    m_ExpandedScreenText = new QLabel(expandedScreenItem);
    m_ExpandedScreenText->setText(tr("Extend"));
    m_ExpandedScreenText->setAlignment(Qt::AlignCenter);
    m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    // add above 2 widgets
    if (getThemeIconPath("project_screen-extend-symbolic").endsWith(".svg")) {
        vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenImageSvg, 0, Qt::AlignHCenter);
    } else {
        vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenImageLabel, 0, Qt::AlignHCenter);
    }
    vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenText, 0, Qt::AlignHCenter);

    m_HBoxLayout->addWidget(expandedScreenItem);
}

void SwitchMonitor::initOneScreenMode()
{
    // for one-screen mode
    for (int i = 0, length = m_ScreenList.length(); i < length; i++) {
        // one-screen mode item
        QWidget *item = new QWidget(m_MonitersWrapper);
        QVBoxLayout *vLayout = new QVBoxLayout(item);
        // image label for one-screen mode
        QSvgWidget *imageSvg = new QSvgWidget(item);
        imageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
        QLabel *imageLabel = new QLabel(item);
        imageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), imageSvg, imageLabel);
        // text label for one-screen mode
        QLabel *textLabel = new QLabel(item);
        textLabel->setText(m_ScreenList[i]);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
        // store imagelabel and textlabel into lists, so that we can change their style later
        m_ImageSvgList << imageSvg;
        m_ImageLabelList << imageLabel;
        m_TextLabelList << textLabel;
        // add above 2 widgets
        if (getThemeIconPath("project_screen-onlyone-symbolic").endsWith(".svg")) {
            vLayout->addWidget(imageSvg, 0, Qt::AlignHCenter);
        } else {
            vLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);
        }
        vLayout->addWidget(textLabel, 0, Qt::AlignHCenter);

        m_HBoxLayout->addWidget(item);
    }
}

void SwitchMonitor::initScreenMode()
{
    // for each mode, we would do the same operations to change it's style when app starts
    switch (displaymode) {
    case Custom:
        //        m_CurrentIndexOfMonitorItem = -1;
        break;
    case Duplicate:
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic-focus"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        m_CurrentIndexOfMonitorItem = 0;
        break;
    case Expanded:
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic-focus"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        m_CurrentIndexOfMonitorItem = 1;
        break;
    case OneScreen:
        QString primaryScreenName = m_DisplayInterface->primary();
        for (int i = 0, length = m_ScreenList.length(); i < length; i++) {
            if (m_ScreenList[i] == primaryScreenName) {
                showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[i], m_ImageLabelList[i]);
                m_TextLabelList[i]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
                // m_CurrentIndexOfMonitorItem is always 2 bigger than m_ScreenList's i
                m_CurrentIndexOfMonitorItem = i + 2;
            }
        }
        break;
    }
}

void SwitchMonitor::highlightNextMonitor()
{
    reAlignCurrentIndex();

    reHighlightMonitor();
}

void SwitchMonitor::reAlignCurrentIndex()
{
    if (m_CurrentIndexOfMonitorItem < (m_ScreenList.length() + 1)) {
        ++m_CurrentIndexOfMonitorItem;
    } else {
        m_CurrentIndexOfMonitorItem = 0;
    }
}

void SwitchMonitor::reHighlightMonitor()
{
    if (m_CurrentIndexOfMonitorItem == 0) {
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic-focus"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), m_ImageSvgList[m_ScreenList.length() - 1], m_ImageLabelList[m_ScreenList.length() - 1]);
        m_TextLabelList[m_ScreenList.length() - 1]->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else if (m_CurrentIndexOfMonitorItem == 1) {
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic-focus"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else if (m_CurrentIndexOfMonitorItem == 2) {
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[0], m_ImageLabelList[0]);
        m_TextLabelList[0]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else {
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[m_CurrentIndexOfMonitorItem - 2], m_ImageLabelList[m_CurrentIndexOfMonitorItem - 2]);
        m_TextLabelList[m_CurrentIndexOfMonitorItem - 2]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), m_ImageSvgList[m_CurrentIndexOfMonitorItem - 3], m_ImageLabelList[m_CurrentIndexOfMonitorItem - 3]);
        m_TextLabelList[m_CurrentIndexOfMonitorItem - 3]->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    }
}
