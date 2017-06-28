/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>

#include "../boxframe/boxframe.h"
#include "loginmanager.h"

class LoginFrame : public BoxFrame
{
    Q_OBJECT

public:
    explicit LoginFrame(QWidget* parent = 0);
    ~LoginFrame();

public slots:
    void updateScreenPosition(QRect rect);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;

private:
    QList<QLabel*> otherScreenBackgroundList;
    LoginManager* m_loginManager;
};

#endif // LOGINFRAME_H
