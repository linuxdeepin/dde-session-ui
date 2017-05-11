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
#include <QCryptographicHash>
#include <QDesktopWidget>
#include <QScreen>

#include "boxframe.h"

static const QString DefaultBackground = "/usr/share/backgrounds/default_background.jpg";
static const QString BlurredImageDir = "/var/cache/image-blur/";

static QString GetBlurredImagePath(QString path) {
    QString ext = path.split(".").last();
    QString md5 = QString(QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex());
    return BlurredImageDir + QString("%1.%2").arg(md5).arg(ext);
}

BoxFrame::BoxFrame(QWidget *parent)
    : QFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    setObjectName("BoxFrame");

    qDebug() << "this geometry" << geometry();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint);

    // Always receives mouseMoveEvent.
    this->setMouseTracking(true);

    m_blurredImageWatcher.addPath(BlurredImageDir);
    connect(&m_blurredImageWatcher, &QFileSystemWatcher::directoryChanged, [this](const QString &){
        // NOTE: the direcotryChanged signal is triggered when the blurred background
        // is about being written to the disk. It's not completed yet.
        QTimer::singleShot(500, this, SLOT(setBackground()));
    });

    m_screenSizeMonitor = new QTimer(this);
    m_screenSizeMonitor->setInterval(1000);
    m_screenSizeMonitor->setSingleShot(false);
    m_lastScreenGeometry = qApp->desktop()->rect();
    connect(m_screenSizeMonitor, &QTimer::timeout, [this] {
        if (m_lastScreenGeometry == qApp->desktop()->rect() &&
            m_lastPrimaryScreenGeometry == qApp->primaryScreen()->geometry())
            return;

        m_lastScreenGeometry = qApp->desktop()->rect();
        m_lastPrimaryScreenGeometry = qApp->primaryScreen()->geometry();
        setFixedSize(m_lastScreenGeometry.size());
        setBackground(m_lastUrl, true);

        emit screenChanged(m_lastPrimaryScreenGeometry);
    });
    m_screenSizeMonitor->start();
}

BoxFrame::BoxFrame(const QString url, QWidget *parent)
    : BoxFrame(parent)
{
    this->setBackground(url);
}

void BoxFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

void BoxFrame::resizeEvent(QResizeEvent *e) {
//    QSize boxFrameSize = e->size();
//    this->setFixedSize(boxFrameSize);
}

BoxFrame::~BoxFrame()
{
}

// Message for maintainers: DON'T use QSS to set the background !

// This function takes ~2ms by setting QSS to set backgrounds, but causes show() of
// ShutdownFrame takes ~260ms to complete. On the other hand, this function takes
// ~130ms by setting pixmap, yet takes only ~12ms to complete the show() of ShutdownFrame.
// It'll be more obvious on dual screens environment.
void BoxFrame::setBackground()
{
	setBackground(m_lastUrl, true);
}

void BoxFrame::setBackground(const QString &url, bool force)
{
    static const QString objName("GreeterBackground");

    if (!url.isEmpty() && m_lastUrl == url && !force) return;

    QString path = QUrl(url).isLocalFile() ? QUrl(url).toLocalFile() : url;
    if (path.isEmpty() || !QFile::exists(path))
        path = DefaultBackground;

    const QString blurredPath = GetBlurredImagePath(path);
    if (QFile::exists(blurredPath)) {
        path = blurredPath;
    }

    m_lastUrl = path;
    m_lastPixmap = QPixmap(m_lastUrl);

    QList<QLabel*> labels = findChildren<QLabel*>(objName);

    qDeleteAll(labels);

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++)
    {
        const QRect rect = screenList[i]->geometry();

        QLabel* m_background = new QLabel(this);
        m_background->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_background->setObjectName(objName);
        m_background->setFixedSize(rect.size());
        m_background->setPixmap(m_lastPixmap.scaled(rect.size(), Qt::KeepAspectRatioByExpanding));
        m_background->move(rect.x(), rect.y());
        m_background->setVisible(true);
        m_background->lower();

        qDebug() << "create new bg: " << rect;
    }
}
