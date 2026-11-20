// SPDX-FileCopyrightText: 2015 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.h"

#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QWindow>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>

#include <DGuiApplicationHelper>
#include <DPlatformWindowHandle>

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE
DS_USE_NAMESPACE

namespace {
bool isX11Platform()
{
    static const bool isX11 = Dtk::Gui::DGuiApplicationHelper::isXWindowPlatform();
    return isX11;
}
} // namespace

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_image(new QLabel(this))
    , m_text(new QLabel(tr("Low battery, please plug in"), this))
    , m_bSleepLock(false)
{
    setAccessibleName("Window");
    m_image->setAccessibleName("LowPowerImage");
    m_text->setAccessibleName("LowPowerText");
    Qt::WindowFlags flags = Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint;
    if (isX11Platform()) {
        flags |= Qt::X11BypassWindowManagerHint;
    }
    setWindowFlags(flags);

    // Wayland: use platform window handle for fullscreen rendering
    if (!isX11Platform()) {
        m_platformHandle = new Dtk::Widget::DPlatformWindowHandle(this, this);
        m_platformHandle->setWindowRadius(0);
        winId(); // ensure native window handle exists for windowHandle()
        if (QWindow *w = windowHandle()) {
            auto *layerShell = DLayerShellWindow::get(w);
            if (layerShell) {
                layerShell->setLayer(DLayerShellWindow::LayerOverlay);
                layerShell->setExclusiveZone(-1);
                layerShell->setAnchors(DLayerShellWindow::Anchors(DLayerShellWindow::AnchorTop
                                       | DLayerShellWindow::AnchorBottom
                                       | DLayerShellWindow::AnchorLeft
                                       | DLayerShellWindow::AnchorRight));
            }
        }
    }

    const qreal ratio = devicePixelRatioF();
    const QString iconPath = qt_findAtNxFile(":/images/lowpower.png", ratio);
    QPixmap pix(iconPath);
    pix.setDevicePixelRatio(ratio);
    m_pix = pix;
    m_image->setPixmap(m_pix);
    m_text->setStyleSheet("QLabel{ color:#ff8000; font-size:15px }");

    setupSize();
    setStyleSheet("Window { background: black }");

    QDBusConnection::sessionBus().connect("org.deepin.dde.LockFront1",
                                              "/org/deepin/dde/LockFront1",
                                              "org.deepin.dde.LockFront1",
                                              "Visible",
                                              this, SLOT(HideShowToRaise(bool)));

    // Wayland: monitor screen changes to keep overlay windows in sync
    if (!isX11Platform()) {
        connect(qApp, &QGuiApplication::screenAdded, this, [this](QScreen *s) {
            connect(s, &QScreen::geometryChanged, this, &Window::updateScreenWindows);
            updateScreenWindows();
        });
        connect(qApp, &QGuiApplication::screenRemoved, this, [this](QScreen *s) {
            disconnect(s, &QScreen::geometryChanged, this, &Window::updateScreenWindows);
            updateScreenWindows();
        });
        for (QScreen *s : qApp->screens()) {
            connect(s, &QScreen::geometryChanged, this, &Window::updateScreenWindows);
        }
    }
}

Window::~Window()
{
    qDeleteAll(m_coverHandles);
    m_coverHandles.clear();
    qDeleteAll(m_screenWindows);
    m_screenWindows.clear();
    delete m_platformHandle;
    m_platformHandle = nullptr;
}

bool Window::SleepLock()
{
    return m_bSleepLock;
}

void Window::setupSize()
{
    // Wayland: create separate cover windows per screen for multi-monitor fullscreen
    if (!isX11Platform()) {
        updateScreenWindows();
    } else {
        int totalWidth = 0;
        int totalHeight = 0;
        for (const QScreen *screen : qApp->screens()) {
            totalWidth = qMax(totalWidth, screen->geometry().x() + screen->geometry().width());
            totalHeight = qMax(totalHeight, screen->geometry().y() + screen->geometry().height());
        }
        setFixedSize(totalWidth, totalHeight);
    }

    QScreen *screen = qApp->screenAt(QCursor::pos());
    if (!screen) {
        screen = qApp->primaryScreen();
    }
    if (!screen) {
        qWarning() << "No screen available for low power window";
        return;
    }

    const qreal ratio = screen->devicePixelRatio();
    m_image->setFixedSize(m_pix.size() / ratio);

    // Position child widgets using local coordinates relative to the main window.
    // X11: window covers all screens from origin, so global coords == local.
    // Wayland: window covers primary screen only; use rect() for local centering.
    if (isX11Platform()) {
        m_image->move(screen->geometry().center() - m_image->rect().center());
        m_text->setFixedSize(screen->geometry().width(), 30);
    } else {
        m_image->move(rect().center() - m_image->rect().center());
        m_text->setFixedSize(width(), 30);
    }
    m_text->setAlignment(Qt::AlignHCenter);
    m_text->move(0, m_image->y() + m_pix.height());

    QDBusInterface inter2Power("org.deepin.dde.Power1",
                               "/org/deepin/dde/Power1",
                               "org.deepin.dde.Power1",
                               QDBusConnection::sessionBus(), this);
    m_bSleepLock =  inter2Power.property("SleepLock").toBool();
}

void Window::updateScreenWindows()
{
    const QList<QScreen *> screens = qApp->screens();
    if (screens.isEmpty()) {
        return;
    }

    QScreen *primaryScreen = qApp->primaryScreen();
    if (!primaryScreen) {
        qWarning() << "Primary screen is null, falling back to first available screen";
        primaryScreen = screens.first();
    }

    const QRect primaryGeometry = primaryScreen->geometry();
    if (geometry().topLeft() != primaryGeometry.topLeft()) {
        move(primaryGeometry.topLeft());
    }
    if (size() != primaryGeometry.size()) {
        setFixedSize(primaryGeometry.size());
    }

    // Count screens needing cover windows: non-primary and non-mirrored
    // (mirrored/clone screens share the same geometry as the primary)
    int coverWindowCount = 0;
    for (QScreen *screen : screens) {
        if (screen == primaryScreen || screen->geometry() == primaryGeometry) {
            continue;
        }
        coverWindowCount++;
    }
    if (coverWindowCount <= 0) {
        qDeleteAll(m_coverHandles);
        m_coverHandles.clear();
        qDeleteAll(m_screenWindows);
        m_screenWindows.clear();
        return;
    }

    while (m_screenWindows.size() < coverWindowCount) {
        QWidget *screenWindow = new QWidget(nullptr,
            Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
        screenWindow->setAccessibleName("LowPowerScreenWindow");
        screenWindow->setAttribute(Qt::WA_ShowWithoutActivating);
        screenWindow->setAutoFillBackground(true);
        screenWindow->setCursor(QCursor(Qt::BlankCursor));
        QPalette screenPal(screenWindow->palette());
        screenPal.setColor(QPalette::Window, Qt::black);
        screenWindow->setPalette(screenPal);
        auto *coverHandle = new Dtk::Widget::DPlatformWindowHandle(screenWindow, screenWindow);
        coverHandle->setWindowRadius(0);
        m_coverHandles.append(coverHandle);
        m_screenWindows.append(screenWindow);
    }

    while (m_screenWindows.size() > coverWindowCount) {
        delete m_coverHandles.takeLast();
        delete m_screenWindows.takeLast();
    }

    int coverIndex = 0;
    for (QScreen *screen : screens) {
        if (screen == primaryScreen || screen->geometry() == primaryGeometry) {
            continue;
        }

        QWidget *screenWindow = m_screenWindows.at(coverIndex++);

        // Wayland: bind the cover window to the target screen before setting
        // geometry or showing. The compositor controls window placement;
        // without setScreen() the cover window lands on the wrong output.
        // Reference: widgets/fullscreenbackground.cpp showEvent pattern.
        screenWindow->winId(); // ensure native window handle exists
        if (QWindow *w = screenWindow->windowHandle()) {
            if (w->screen() != screen) {
                w->setScreen(screen);
            }
            if (!isX11Platform()) {
                auto *layerShell = DLayerShellWindow::get(w);
                if (layerShell) {
                    layerShell->setLayer(DLayerShellWindow::LayerOverlay);
                    layerShell->setExclusiveZone(-1);
                    layerShell->setAnchors(DLayerShellWindow::Anchors(DLayerShellWindow::AnchorTop
                                           | DLayerShellWindow::AnchorBottom
                                           | DLayerShellWindow::AnchorLeft
                                           | DLayerShellWindow::AnchorRight));
                }
            }
        }

        screenWindow->setGeometry(screen->geometry());
        screenWindow->setVisible(isVisible());
        if (!screenWindow->isHidden()) {
            screenWindow->raise();
        }
    }
}

void Window::setScreenWindowsVisible(bool visible)
{
    for (QWidget *screenWindow : qAsConst(m_screenWindows)) {
        screenWindow->setVisible(visible);
        if (visible) {
            screenWindow->raise();
        }
    }
}

void Window::HideShowToRaise(bool visible)
{
    if (visible) {
        QTimer::singleShot(200, this, [ = ]  {
            hide();
            this->raise();
            show();
            setScreenWindowsVisible(true);
        });
    }
}

bool Window::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Show:
        if (isVisible()) {
            setupSize();
            setScreenWindowsVisible(true);
        }
        break;
    case QEvent::Hide:
        setScreenWindowsVisible(false);
        break;
    case QEvent::WindowActivate:
        if (isVisible()) {
            setScreenWindowsVisible(true);
        }
        break;
    case QEvent::WindowDeactivate:
        // Wayland only: compositor may deactivate the window, re-raise after delay.
        // X11 is excluded to avoid stealing focus (original X11 behavior).
        if (!isX11Platform() && !isHidden()) {
            QTimer::singleShot(30, this, [this]() {
                if (!isVisible()) {
                    return;
                }
                raise();
                activateWindow();
                setScreenWindowsVisible(true);
            });
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

LowPowerAdaptor::LowPowerAdaptor(Window * parent) :
    QDBusAbstractAdaptor(parent)
{

}

LowPowerAdaptor::~LowPowerAdaptor()
{

}

void LowPowerAdaptor::Raise()
{
    Window * w = qobject_cast<Window*>(parent());
    if (w) {
        w->raise();
        w->activateWindow();
        w->grabMouse();
        w->setScreenWindowsVisible(true);
        if (!w->SleepLock()) {
           w->grabKeyboard();
        }
    }
}

void LowPowerAdaptor::Quit()
{
    qApp->quit();
}
