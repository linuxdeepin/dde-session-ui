/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fullscreenbackground.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>
#include <QCryptographicHash>

static const QString BlurredImageDir = "/var/cache/image-blur/";

FullscreenBackground::FullscreenBackground(QWidget *parent)
    : QWidget(parent),

      m_adjustTimer(new QTimer(this))
{
    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);

    m_blurWatcher.addPath(BlurredImageDir);
//    connect(&m_blurWatcher, &QFileSystemWatcher::directoryChanged, this, [=] {
//        // It takes time to blur a wallpaper.
//        QTimer::singleShot(500, this, [=] {
//            setBackground(m_bgPath);
//        });
//    });
}

void FullscreenBackground::setBackground(const QString &file)
{
    const QUrl url(file);
    if (url.isLocalFile())
        return setBackground(url.path());

    m_bgPath = file;

    Q_ASSERT(QFileInfo(file).isFile());

    setBackground(QPixmap(getBlurImagePath(file)));
}

void FullscreenBackground::setBackground(const QPixmap &pixmap)
{
    Q_ASSERT(!pixmap.isNull());

    m_background = pixmap;

    update();
}

void FullscreenBackground::setContent(QWidget * const w)
{
    Q_ASSERT(m_content.isNull());

    m_content = w;
    m_content->installEventFilter(this);

    m_adjustTimer->start();
}

void FullscreenBackground::adjustGeometry()
{
    const QPoint cp(QCursor::pos());
    QRect r, pr;
    for (const auto *s : qApp->screens())
    {
        const QRect& g = s->geometry();
        if (g.contains(cp))
            pr = g;

        r = r.united(g);
    }

    QWidget::setGeometry(r);

    if (!m_content.isNull())
        m_content->setGeometry(pr.isNull() ? qApp->primaryScreen()->geometry() : pr);
}

const QString FullscreenBackground::getBlurImagePath(const QString &path)
{
    const QString ext = path.split(".").last();
    const QString md5 = QString(QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex());
    const QString blur = BlurredImageDir + QString("%1.%2").arg(md5).arg(ext);
    QFile file(blur);
    if (file.exists())
        return blur;
    else
        return path;
}

bool FullscreenBackground::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_content && event->type() == QEvent::Leave)
        m_adjustTimer->start();

    return QWidget::eventFilter(watched, event);
}

void FullscreenBackground::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);

    m_adjustTimer->start();
}

void FullscreenBackground::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QRect r;
    for (auto *s : qApp->screens())
    {
        const QRect g = s->geometry();
        const QRect tr(r.width(), r.y(), g.width(), g.height());
        const QPixmap pix = m_background.scaled(s->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        const QRect pix_r = QRect((pix.width() - tr.width()) / 2, (pix.height() - tr.height()) / 2, tr.width(), tr.height());

        r = r.united(tr);
        painter.drawPixmap(tr, pix, pix_r);
    }
}

void FullscreenBackground::keyPressEvent(QKeyEvent *e)
{
    QWidget::keyPressEvent(e);

    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }
}
