/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <dimagebutton.h>
#include <dmpriscontrol.h>
#include "util_file.h"

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);
    inline void setUserSwitchEnable(bool enable) {m_userswitch->setVisible(enable);}

signals:
    void shutdownClicked();
    void switchUser();

private:
    DTK_WIDGET_NAMESPACE::DImageButton* m_userswitch;
    DTK_WIDGET_NAMESPACE::DImageButton *m_shutdown;

    UtilFile* m_utilFile;
    Dtk::Widget::DMPRISControl *m_mprisWidget;
};

#endif // CONTROLWIDGET_H
