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
    , m_fakeBackground(new FakeBackground(this))
{
    setAttribute(Qt::WA_TranslucentBackground);

    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);

    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &FullscreenBackground::onBlurFinished);

    connect(m_fakeBackground, &FakeBackground::finished, this, [=] (const QPixmap &pixmap) {
        setBackground(pixmap);
    });
}

void FullscreenBackground::setBackground(const QString &file, FakeBackground::Type type)
{
    const QUrl url(file);
    if (url.isLocalFile())
        return setBackground(url.path(), type);

    if (m_bgPath == file)
        return;

    if (QFile::exists(file))
        m_bgPath = file;
    else
        m_bgPath = "/usr/share/backgrounds/deepin/desktop.jpg";

    Q_ASSERT(QFileInfo(file).isFile());

    const QString &p = m_blurImageInter->Get(m_bgPath);

    setBackground(QPixmap(p.isEmpty() ? m_bgPath : p), type);
}

void FullscreenBackground::setBackground(const QPixmap &pixmap, FakeBackground::Type type)
{
    // NOTE(kirigaya): If file is not exist, use default wallpaper
    QPixmap p = pixmap.isNull() ? QPixmap("/usr/share/backgrounds/deepin/desktop.jpg") : pixmap;

    if (type == FakeBackground::None) {
        m_background = p;
        update();
    } else {
        m_fakeBackground->setPixmap(p, type);
    }
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
    m_fakeBackground->setGeometry(r);

    if (m_content.isNull())
        return;

    if (!pr.isNull())
        return m_content->setGeometry(pr);

    const QRect &pg(qApp->primaryScreen()->geometry());
    const QRect realPg(pg.topLeft() / ratio, pg.size());
    m_content->setGeometry(realPg);
    m_content->raise();
}

void FullscreenBackground::onBlurFinished(const QString &source, const QString &blur, bool status)
{
    const QString &sourcePath = QUrl(source).isLocalFile() ? QUrl(source).toLocalFile() : source;

    if (status && m_bgPath == sourcePath)
        setBackground(QPixmap(blur));
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

    if (m_background.isNull())
        return;

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

FakeBackground::FakeBackground(QWidget *parent)
    : QFrame(parent)
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);

    effect->setOpacity(0);

    m_backgroundAnimation = new QPropertyAnimation(effect, "opacity", this);
    m_backgroundAnimation->setDuration(1000);
    m_backgroundAnimation->setEasingCurve(QEasingCurve::InOutCubic);

    // NOTE(kirigaya): Draw a real background
    connect(m_backgroundAnimation, &QVariantAnimation::finished, this, [=] {
        emit finished(m_pixmap);
    });
}

void FakeBackground::setPixmap(const QPixmap &pixmap, Type type)
{
    // NOTE(kirigaya): Login type need parent draw new wallpaper,
    // and fade out
    switch (type) {
    case FadeIn: {
        m_backgroundAnimation->setStartValue(0);
        m_backgroundAnimation->setEndValue(1);
        m_pixmap = pixmap;
        break;
    }
    case FadeOut: {
        m_backgroundAnimation->setStartValue(1);
        m_backgroundAnimation->setEndValue(0);
        emit finished(pixmap);
        break;
    }
    default:
        break;
    }

    update();
    m_backgroundAnimation->start();
}

QPixmap FakeBackground::pixmap() const
{
    return m_pixmap;
}

void FakeBackground::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for (auto *s : qApp->screens())
    {
        const QRect &geom = s->geometry();
        const QRect tr(geom.topLeft() / devicePixelRatioF(), geom.size());
        const QPixmap pix = m_pixmap.scaled(s->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        const QRect pix_r = QRect((pix.width() - tr.width()) / 2, (pix.height() - tr.height()) / 2, tr.width(), tr.height());

        painter.drawPixmap(tr, pix, pix_r);
    }
}
