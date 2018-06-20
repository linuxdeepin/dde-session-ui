/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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
    , m_fadeOutAni(new QVariantAnimation(this))
{
    m_adjustTimer->setSingleShot(true);

    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

    connect(m_adjustTimer, &QTimer::timeout, this, &FullscreenBackground::adjustGeometry);
    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &FullscreenBackground::onBlurFinished);
    connect(QApplication::desktop(), &QDesktopWidget::resized, m_adjustTimer, static_cast<void (QTimer::*)()>(&QTimer::start));

    m_fadeOutAni->setEasingCurve(QEasingCurve::InOutCubic);
    m_fadeOutAni->setDuration(1000);
    m_fadeOutAni->setStartValue(1.0f);
    m_fadeOutAni->setEndValue(0.0f);

    connect(m_fadeOutAni, &QVariantAnimation::valueChanged, this, static_cast<void (FullscreenBackground::*)()>(&FullscreenBackground::update));
}

void FullscreenBackground::updateBackground(const QPixmap &background)
{
    // show old background fade out
    m_fakeBackground = m_background;
    m_background = background;

    m_fadeOutAni->start();
}

void FullscreenBackground::updateBackground(const QString &file)
{
    const QUrl url(file);
    if (url.isLocalFile())
        return updateBackground(url.path());

    if (m_bgPath == file)
        return;

    if (QFile::exists(file))
        m_bgPath = file;
    else
        m_bgPath = "/usr/share/backgrounds/default_background.jpg";

    Q_ASSERT(QFileInfo(m_bgPath).isFile());

    // 这里之后要去掉，在 FullscreenBackground 里面就不要做模糊的查找了，在调用前将图片准备好
//    const QString &p = m_blurImageInter->Get(m_bgPath);

    updateBackground(QPixmap(m_bgPath));
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

        // Without using the processed geometry and mouse coordinates,
        // the results can be obtained using the original information.
        // If the original screen contains the original mouse, save the scaled geometry.
        if (g.contains(cp))
            pr = realRect;

        r = r.united(realRect);
    }

    setGeometry(r);
//    m_fakeBackground->setGeometry(r);

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
        updateBackground(blur);
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
    const float current_ani_value = m_fadeOutAni->currentValue().toFloat();

    for (auto *s : qApp->screens())
    {
        const QRect &geom = s->geometry();
        const QRect tr(geom.topLeft() / devicePixelRatioF(), geom.size());
        const QPixmap &pix = m_background.scaled(s->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        if (!pix.isNull())
            painter.drawPixmap(tr, pix, QRect((pix.width() - tr.width()) / 2, (pix.height() - tr.height()) / 2, tr.width(), tr.height()));

        if (!m_fakeBackground.isNull()) {
        // draw background
        const QPixmap &fadePixmap = m_fakeBackground.scaled(s->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        painter.setOpacity(current_ani_value);
        painter.drawPixmap(tr, fadePixmap, QRect((fadePixmap.width() - tr.width()) / 2, (fadePixmap.height() - tr.height()) / 2, tr.width(), tr.height()));
        painter.setOpacity(1);
        }
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
    // can avoid the loop of changing the geometry and changing the screen(dpr).
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
