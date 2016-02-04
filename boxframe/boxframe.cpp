/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDebug>
#include <QPushButton>
#include <QScreen>
#include <QTimer>

#include "boxframe.h"
#include "backgroundlabel.h"

BoxFrame::BoxFrame(QWidget *parent)
    : QFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry" << geometry();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint);

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        qDebug() << "boxframe rect:" << i<< rect;

        BackgroundLabel* m_background = new BackgroundLabel(true, this);
        m_background->setFixedSize(rect.size());

        qDebug() << "setFixSizeBackgroundSize:" << m_background->size();
        m_background->move(rect.x(), rect.y());
        qDebug() << "Background backgroundGemoetry" << m_background->geometry();

    }

    // Always receives mouseMoveEvent.
    this->setMouseTracking(true);
}

BoxFrame::BoxFrame(const QString url, QWidget *parent)
    : QFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry" << geometry();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint);


    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        qDebug() << "boxframe rect:" << i<< rect;

        QLabel* m_background = new QLabel(this);
        m_background->setStyleSheet(QString("border-image:url(%1)").arg(url));
        m_background->setFixedSize(rect.size());
        qDebug() << "setFixSizeBackgroundSize:" << m_background->size();
        m_background->move(rect.x(), rect.y());
        qDebug() << "QLable backgroundGemoetry" << m_background->geometry();

    }

    // Always receives mouseMoveEvent.
    this->setMouseTracking(true);
}

void BoxFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

void BoxFrame::resizeEvent(QResizeEvent *e) {
    QSize boxFrameSize = e->size();
    this->setFixedSize(boxFrameSize);
}

BoxFrame::~BoxFrame()
{
}
