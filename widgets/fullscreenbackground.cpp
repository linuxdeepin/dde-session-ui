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
    , m_fadeOutAni(new QVariantAnimation(this))
{
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);

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

    updateBackground(QPixmap(m_bgPath));
}

void FullscreenBackground::setContent(QWidget * const w)
{
    Q_ASSERT(m_content.isNull());

    m_content = w;
    m_content->setParent(this);
    m_content->raise();
    m_content->move(0, 0);
}

void FullscreenBackground::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    const float current_ani_value = m_fadeOutAni->currentValue().toFloat();

    if (!m_background.isNull()) {
        const QPixmap &pix = pixmapHandle(m_background);

        // tr is need redraw rect, sourceRect need correct upper left corner
        painter.drawPixmap(rect(), pix, rect());
    }

    if (!m_fakeBackground.isNull()) {
        // draw background
        const QPixmap &fadePixmap = pixmapHandle(m_fakeBackground);

        painter.setOpacity(current_ani_value);
        painter.drawPixmap(rect(), fadePixmap, rect());
        painter.setOpacity(1);
    }
}

void FullscreenBackground::enterEvent(QEvent *event)
{
    m_content->show();

    return QWidget::enterEvent(event);
}

void FullscreenBackground::leaveEvent(QEvent *event)
{
    m_content->hide();

    return QWidget::leaveEvent(event);
}

void FullscreenBackground::resizeEvent(QResizeEvent *event)
{
    m_content->setFixedSize(event->size());

    return QWidget::resizeEvent(event);
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

const QPixmap FullscreenBackground::pixmapHandle(const QPixmap &pixmap)
{
    const QSize trueSize { size() * devicePixelRatioF() };
    QPixmap pix = pixmap.scaled(trueSize,
                                Qt::KeepAspectRatioByExpanding,
                                Qt::SmoothTransformation);

    pix = pix.copy(QRect((pix.width() - trueSize.width()) / 2,
                         (pix.height() - trueSize.height()) / 2,
                         trueSize.width(),
                         trueSize.height()));

    // draw pix to widget, so pix need set pixel ratio from qwidget devicepixelratioF
    pix.setDevicePixelRatio(devicePixelRatioF());

    return pix;
}
