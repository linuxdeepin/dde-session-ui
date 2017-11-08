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

FullscreenBackground::FullscreenBackground(QWidget *parent)
    : QWidget(parent)
    , m_adjustTimer(new QTimer(this))
    , m_blurImageInter(new ImageBlur("com.deepin.daemon.Accounts",
                                     "/com/deepin/daemon/ImageBlur",
                                     QDBusConnection::systemBus(), this))
{
    m_blurImageInter->setSync(false);

    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);

    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &FullscreenBackground::onBlurFinished);
}

void FullscreenBackground::setBackground(const QString &file)
{
    const QUrl url(file);
    if (url.isLocalFile())
        return setBackground(url.path());

    m_bgPath = file;

    Q_ASSERT(QFileInfo(file).isFile());

    QDBusPendingReply<QString> call = m_blurImageInter->Get(m_bgPath);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &FullscreenBackground::onGetBlurFinished);
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
    m_content->setParent(this);
    m_content->installEventFilter(this);

    m_adjustTimer->start();
}

void FullscreenBackground::adjustGeometry()
{
    const auto ratio = devicePixelRatioF();
    const QPoint cp(QCursor::pos());
    QRect r, pr;
    for (const auto *s : qApp->screens())
    {
        const QRect &g(s->geometry());
        const QRect realRect(g.topLeft() / ratio, g.size());
        if (realRect.contains(cp))
            pr = realRect;

        r = r.united(realRect);
    }

    setGeometry(r);

    if (m_content.isNull())
        return;

    if (!pr.isNull())
        return m_content->setGeometry(pr);

    const QRect &pg(qApp->primaryScreen()->geometry());
    const QRect realPg(pg.topLeft() / ratio, pg.size());
    m_content->setGeometry(realPg);
}

void FullscreenBackground::onBlurFinished(const QString &source, const QString &blur, bool status)
{
    const QString &sourcePath = QUrl(source).isLocalFile() ? QUrl(source).toLocalFile() : source;

    if (status && m_bgPath == sourcePath)
        setBackground(QPixmap(blur));
}

void FullscreenBackground::onGetBlurFinished(QDBusPendingCallWatcher *watcher)
{
    if (!watcher->isError()) {
        QDBusPendingReply<QString> reply = watcher->reply();
        const QString &blur = reply.value();
        if (!blur.isEmpty())
            setBackground(QPixmap(blur));
    }

    watcher->deleteLater();
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
        const QRect &geom = s->geometry();
        const QRect tr(geom.topLeft() / devicePixelRatioF(), geom.size());
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

void FullscreenBackground::setGeometry(const QRect &rect)
{
    QRect r(rect);

    // guess the screen of this window before change the window geometry, so we
    // can avoid the loop of changing the geometry and chaning the screen(dpr).
    const QScreen *screen = screenForGeometry(rect);
    if (screen) {
        const qreal dpr = screen->devicePixelRatio();
        const QRect screenGeo = screen->geometry();
        r.moveTopLeft(screenGeo.topLeft() + (rect.topLeft() - screenGeo.topLeft()) / dpr);
    }

    QWidget::setGeometry(r);
}

// implements the basic idea used by Qt to find the associated QScreen of a window.
const QScreen *FullscreenBackground::screenForGeometry(const QRect &rect) const
{
    const qreal ratio = qApp->devicePixelRatio();

    for (const auto *s : qApp->screens())
    {
        const QRect &g(s->geometry());
        const QRect realRect(g.topLeft() / ratio, g.size());

        if (realRect.contains(rect.center()))
            return s;
    }

    return nullptr;
}
