/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef ContentFrame
#define ContentFrame
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore>
#include "contentwidget.h"

class MainFrame : public QFrame
{
    Q_OBJECT
public:
    MainFrame(QWidget* parent=0);
    ~MainFrame();

    ShutDownFrame* m_shutdownFrame;
    QHBoxLayout* m_Layout;
    QVBoxLayout* m_contentLayout;
    QVBoxLayout* m_contentRightLayout;
signals:
    void OutKeyLeft();
    void OutKeyRight();
    void pressEnterAction();

public slots:
    void setConfrim(const bool confirm);
    void powerAction(const Actions action);
    void changeView(bool a);
    void DirectLeft();
    void DirectRight();
    void EnterAction();
    void hideBtns(const QStringList &btnsName);
    void disableBtns(const QStringList &btnsName);

private:
    void initUI();
    void initConnect();

};

#endif // ContentFrame
