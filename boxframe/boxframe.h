/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef BOXFRAME_H
#define BOXFRAME_H

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QEvent>

#include <QFrame>
#include <QLabel>
#include <QFileSystemWatcher>
#include <QTimer>

class BoxFrame : public QFrame
{
    Q_OBJECT

public:
    explicit BoxFrame(QWidget* parent = 0);
    explicit BoxFrame(const QString url, QWidget *parent = 0);
    ~BoxFrame();

public slots:
    void setBackground();
    void setBackground(const QString &url, bool force = false);

signals:
    void screenChanged(QRect rect);

protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    QString m_lastUrl;
    QPixmap m_lastPixmap;
    QRect m_lastScreenGeometry;
    QRect m_lastPrimaryScreenGeometry;
    QFileSystemWatcher m_blurredImageWatcher;

    QTimer *m_screenSizeMonitor;
};

#endif // BOXFRAME_H
