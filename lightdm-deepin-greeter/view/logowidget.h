/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef LOGOWIDGET
#define LOGOWIDGET

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class LogoWidget: public QFrame
{
    Q_OBJECT
public:
    LogoWidget(QWidget* parent=0);
    ~LogoWidget();
private:
    void initUI();
    QString getVersion();
    QLabel* m_logoLabel;
    QLabel* m_logoVersionLabel;
    QVBoxLayout* m_logoLeftSideLayout;
    QVBoxLayout* m_logoRightSideLayout;
    QHBoxLayout* m_logoLayout;
};
#endif // LOGOFRAME

