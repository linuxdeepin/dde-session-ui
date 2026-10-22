// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fullscreen.h"

#include "preLoginWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDebug>
#include <QFutureWatcher>
#include <QImageReader>
#include <QPaintEvent>
#include <QPainter>
#include <QPointer>
#include <QScreen>
#include <QWindow>
#include <QtConcurrent>

static const QString SOLID_BACKGROUND_COLOR = "#000F27"; // 纯色背景色号

#define DEFAULT_BACKGROUND_PATH "/usr/share/wallpapers/deepin/desktop.jpg"

namespace {

// Cached results are already sized; a cache miss can return the unsized source.
QImage loadBackgroundImage(const QString &path, const QSize &size, qreal scale)
{
    const QSize pixelSize = size * scale;
    if (pixelSize.isEmpty())
        return {};

    QImageReader reader(path);
    reader.setDecideFormatFromContent(true);
    QImage image = reader.read();
    if (image.isNull())
        return {};

    if (image.size() != pixelSize) {
        image = image.scaled(pixelSize, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
        image = image.copy(QRect(QPoint((image.width() - pixelSize.width()) / 2,
                                       (image.height() - pixelSize.height()) / 2), pixelSize));
    }
    image.setDevicePixelRatio(scale);
    return image;
}

} // namespace

FullScreen::FullScreen(QWidget *content, QWidget *parent)
    : QWidget(parent)
    , m_content(content)
    , m_showBlack(false)
    , m_backgroundPixmap()
    , m_newGeometry(0, 0, 0, 0)
{
    Q_ASSERT(m_content);

    this->installEventFilter(this);

#ifndef QT_DEBUG
    if (!qgetenv("XDG_SESSION_TYPE").startsWith("wayland")) {
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    } else {
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Window);

        setAttribute(Qt::WA_NativeWindow); // 创建窗口 handle
        create();
        // onScreenDisplay 低于 override，高于 tooltip，希望显示在锁屏上方的界面，均需要调整层级为 onScreenDisplay 或者 override
        windowHandle()->setProperty("_d_dwayland_window-type", "onScreenDisplay");
    }
#endif
}

void FullScreen::setScreen(QPointer<QScreen> screen, bool isVisible)
{
    Q_UNUSED(isVisible);

    if (!screen)
        return;

    m_newGeometry = screen->geometry();
    setGeometry(m_newGeometry);

    const QSize size = m_newGeometry.size();
    const qreal scale = devicePixelRatioF();
    const quint64 request = ++m_backgroundRequest;
    m_backgroundPixmap = QPixmap();

    auto *backgroundWatcher = new QFutureWatcher<QImage>(this);
    connect(backgroundWatcher, &QFutureWatcher<QImage>::finished, this, [this, backgroundWatcher, request] {
        const QImage r = backgroundWatcher->result();
        backgroundWatcher->deleteLater();
        if (request == m_backgroundRequest && !r.isNull() && m_backgroundPixmap.isNull()) {
            m_backgroundPixmap = QPixmap::fromImage(r);
            raise();
            repaint();
        }
    });
    backgroundWatcher->setFuture(QtConcurrent::run([size, scale]() -> QImage {
        return getBackgroundImage(size, scale);
    }));

    auto *cachedBackgroundWatcher = new QFutureWatcher<QImage>(this);
    connect(cachedBackgroundWatcher, &QFutureWatcher<QImage>::finished, this, [this, cachedBackgroundWatcher, request] {
        const QImage r = cachedBackgroundWatcher->result();
        cachedBackgroundWatcher->deleteLater();
        if (request == m_backgroundRequest && !r.isNull()) {
            m_backgroundPixmap = QPixmap::fromImage(r);
            raise();
            repaint();
        }
    });
    cachedBackgroundWatcher->setFuture(QtConcurrent::run([size, scale]() -> QImage {
        return getCachedBackgroundImage(size, scale);
    }));

    show();

    if (screen == qApp->primaryScreen() && !m_showBlack) {
        showContentToCutScreen();
    }
}

void FullScreen::onAuthFinished()
{
    m_showBlack = true;
    repaint();
}

void FullScreen::closeEvent(QCloseEvent *event)
{
    // Only acknowledgement dismisses the notice; screen hotplug uses hide().
    event->ignore();
}

void FullScreen::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    const QRect trueRect = rect();

    // 绘制纯黑色画面，提升体验
    if (m_showBlack) {
        painter.fillRect(trueRect, Qt::black);
        return;
    }

    if (m_backgroundPixmap.isNull()) {
        painter.fillRect(trueRect, QColor(SOLID_BACKGROUND_COLOR));
        return;
    }

    painter.drawPixmap(trueRect,
                       m_backgroundPixmap,
                       m_backgroundPixmap.rect());
}

bool FullScreen::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this && !m_showBlack && (event->type() == QEvent::Enter)) {
        showContentToCutScreen();
    }

    if (watched == this && !m_showBlack && event->type() == QEvent::Resize) {
        if (m_content && m_content->parent() == this) {
            m_content->resize(m_newGeometry.size());
        }
    }

    return QWidget::eventFilter(watched, event);
}

QImage FullScreen::getBackgroundImage(const QSize &size, qreal scale)
{
    return loadBackgroundImage(DEFAULT_BACKGROUND_PATH, size, scale);
}

QImage FullScreen::getCachedBackgroundImage(const QSize &size, qreal scale)
{
    const QSize pixelSize = size * scale;
    if (pixelSize.isEmpty())
        return {};

    QDBusMessage message = QDBusMessage::createMethodCall(
            "org.deepin.dde.WallpaperCache", "/org/deepin/dde/WallpaperCache",
            "org.deepin.dde.WallpaperCache", "GetWallpaperListForScreen");
    const QVariantList sizes{QVariant::fromValue(pixelSize)};
    message << DEFAULT_BACKGROUND_PATH << QVariant::fromValue(sizes) << true;
    const QDBusPendingReply<QStringList> reply = QDBusConnection::systemBus().call(message, QDBus::Block, 2 * 1000);
    if (reply.isError()) {
        qWarning() << "Failed to get cached user notice wallpaper:" << reply.error();
        return {};
    }

    // One size is requested, so the service returns its cache or an unsized fallback.
    const QStringList paths = reply.value();
    if (paths.isEmpty())
        return {};

    return loadBackgroundImage(paths.first(), size, scale);
}

void FullScreen::showContentToCutScreen()
{
    activateWindow();

    m_content->hide();
    m_content->setParent(this);
    qDebug() << "Full screen geometry:" << m_newGeometry.size() << geometry();
    auto contentWidget = qobject_cast<PreLoginWindow *>(m_content);
    if (contentWidget) {
        contentWidget->updateBrowserSize(m_newGeometry.width());
    }
    m_content->resize(m_newGeometry.size());

    m_content->show();
}
