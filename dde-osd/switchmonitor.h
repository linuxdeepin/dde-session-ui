/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SWITCHMONITOR_H
#define SWITCHMONITOR_H

#include "dbus/displaydbus.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSvgWidget>

class SwitchMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchMonitor(QWidget *parent = 0);

    bool isPanelVailed();

    void setMonitorMode();
    void loadSwitchMonitors();
    void highlightNextMonitor();
    void reHighlightMonitor();

    void hideMonitors();
    void showMonitors();
    void resizeParent();
    double getBrightness();

private:
    void initGlobalVars(QWidget *parent);
    void getAllMonitors();
    void deleteOsd();
    void initMonitorItems();
    void resizeMonitor();
    void initDuplicateMode();
    void initExtendMode();
    void initOneScreenMode();
    void initScreenMode();
    void reAlignCurrentIndex();

    DisplayDbus* m_DisplayInterface;
    QWidget* m_ParentItem;
    QWidget *m_MonitersWrapper;
    QHBoxLayout* m_HBoxLayout;
    QSvgWidget *m_DuplicateScreenImageSvg;
    QLabel* m_DuplicateScreenImageLabel;
    QLabel *m_DuplicateScreenText;
    QSvgWidget *m_ExpandedScreenImageSvg;
    QLabel* m_ExpandedScreenImageLabel;
    QLabel *m_ExpandedScreenText;

    enum DisPlayMode {
        Custom,
        Duplicate,
        Expanded,
        OneScreen
    } displaymode;

    QStringList m_ScreenList;
    QList <QSvgWidget *> m_ImageSvgList;
    QList <QLabel *> m_ImageLabelList;
    QList <QLabel *> m_TextLabelList;
    int m_CurrentIndexOfMonitorItem;

    const int BASE_SIZE = 140;
    const int IMAGE_SIZE = 56;
    const QString MONITOR_TEXT_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px}";
    const QString MONITOR_TEXT_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px}";
};

#endif // SWITCHMONITOR_H
