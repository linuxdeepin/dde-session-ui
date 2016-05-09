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

#include "boxframe.h"
#include "backgroundlabel.h"

static const QString DefaultBackground = "/usr/share/backgrounds/default_background.jpg";
static const QString BlurredImageDir = "/var/cache/image-blur/";

static QString GetBlurredImagePath(QString path) {
    QString ext = path.split(".").last();
    QString md5 = QString(QCryptographicHash::hash(path.toLatin1(), QCryptographicHash::Md5).toHex());
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
        QTimer::singleShot(500, [this] { setBackground(m_lastUrl, true); });
    });
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
    QSize boxFrameSize = e->size();
    this->setFixedSize(boxFrameSize);
}

BoxFrame::~BoxFrame()
{
}

// Message for maintainers: DON'T use QSS to set the background !

// This function takes ~2ms by setting QSS to set backgrounds, but causes show() of
// ShutdownFrame takes ~260ms to complete. On the other hand, this function takes
// ~130ms by setting pixmap, yet takes only ~12ms to complete the show() of ShutdownFrame.
// It'll be more obvious on dual screens environment.
void BoxFrame::setBackground(const QString &url, bool force)
{
    static const QString objName("GreeterBackground");

    if (m_lastUrl == url && !force) return;

    m_lastUrl = url;
    const QString path = QUrl(url).isLocalFile() ? QUrl(url).toLocalFile() : url;

    QPixmap pix(path);
    QString blurredPath = GetBlurredImagePath(path);
    if (QFile::exists(blurredPath)) {
        pix = QPixmap(blurredPath);
    }
    if (pix.isNull()) {
        pix.load(DefaultBackground);
    }

    QList<QLabel*> labels = findChildren<QLabel*>(objName);
    if (labels.isEmpty()) {
        QList<QScreen *> screenList = qApp->screens();
        for (int i = 0; i < screenList.length(); i++) {
            const QRect rect = screenList[i]->geometry();

            QLabel* m_background = new QLabel(this);
            m_background->setObjectName(objName);
            m_background->setFixedSize(rect.size());
            m_background->setPixmap(pix.scaled(rect.size(), Qt::KeepAspectRatioByExpanding));
            m_background->move(rect.x(), rect.y());
            m_background->lower();
        }
    } else {
        foreach (QLabel * label, labels) {
            label->setPixmap(pix.scaled(label->size(), Qt::KeepAspectRatioByExpanding));
        }
    }
}
