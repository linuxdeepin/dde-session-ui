/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "mainwindow.h"
#include "pushbuttonlist.h"
#include <hotzone.h>
#include <QScreen>
#include <QGuiApplication>
#include <QMediaPlayer>
#include <QMediaPlaylist>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_dbusZoneInter = new ZoneInterface("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone", QDBusConnection::sessionBus(), this);

    // let the app start without system animation
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    // let background be transparent
    setAttribute(Qt::WA_TranslucentBackground, true);

    // catch the screen that mouse is in
    QList<QScreen *> screenList = QGuiApplication::screens();
    for (int i = 0; i < screenList.length(); i++) {
        QRect screen = screenList[i]->geometry();
        if (screen.contains(QCursor::pos())) {
            // set the size and position of this app. Enlarge 30px to height to avoid fade-zone of mouseEvent.
            this->setGeometry(screen.x(), screen.y() - MAIN_ITEM_TOP_MARGIN, screen.width(), screen.height() + MAIN_ITEM_TOP_MARGIN);
            break;
        }
    }

    // set the background
    QWidget *back = new QWidget(this);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0, 178));
    back->setPalette(palette);
    back->setAutoFillBackground(true);
    back->setGeometry(0, MAIN_ITEM_TOP_MARGIN, this->width(), this->height() - MAIN_ITEM_TOP_MARGIN);

    m_videoWidget = new DVideoWidget(this);

    // init corresponding QList for addButtons()
    m_ButtonNames << tr("Fast Screen Off") << tr("Control Center") << tr("All Windows") << tr("Launcher") << tr("Desktop") << tr("None");
    m_ActionStrs << FAST_SCREEN_OFF << CONTROL_CENTER_FROM_LEFT_STR << ALL_WINDOWS_STR << LAUNCHER_STR << SHOW_DESKTOP_STR << NONE_STR;
    m_ActionStrs2 << FAST_SCREEN_OFF << CONTROL_CENTER_FROM_RIGHT_STR << ALL_WINDOWS_STR << LAUNCHER_STR << SHOW_DESKTOP_STR << NONE_STR;

    QStringList topRightNames = QStringList() << tr("Close Window") << m_ButtonNames;
    QStringList topRightActionStr = QStringList() << CLOSE_MAX_WINDOW_STR << m_ActionStrs2;

    // load 4 corners
    HotZone *hotzone1 = new HotZone(this, false, false);
    hotzone1->addButtons(m_ButtonNames, m_ActionStrs);

    HotZone *hotzone2 = new HotZone(this, true, false);
    hotzone2->addButtons(topRightNames, topRightActionStr);

    HotZone *hotzone3 = new HotZone(this, false, true);
    hotzone3->addButtons(m_ButtonNames, m_ActionStrs);

    HotZone *hotzone4 = new HotZone(this, true, true);
    hotzone4->addButtons(m_ButtonNames, m_ActionStrs2);

    m_dbusZoneInter->EnableZoneDetected(false);

    QTimer::singleShot(1000, this, &MainWindow::onDemoVideo);
}

MainWindow::~MainWindow()
{
    m_dbusZoneInter->EnableZoneDetected(true);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton || e->button() == Qt::MiddleButton) {
        this->deleteLater();
        qApp->quit();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        this->deleteLater();
        qApp->quit();
    }
}

void MainWindow::onDemoVideo()
{
    QMediaPlayer *player = new QMediaPlayer;

    QMediaPlaylist *list = new QMediaPlaylist;

    list->addMedia(QUrl("qrc:/images/Prompt.mov"));
    list->setPlaybackMode(QMediaPlaylist::Loop);

    int x = (this->rect().right() - 450) / 2;
    int y = (this->rect().bottom() - 348) / 2;
    m_videoWidget->setGeometry(x, y, 450, 348);

    m_videoWidget->setSource(player);
    player->setPlaylist(list);
    player->play();
}
