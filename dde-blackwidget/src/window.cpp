// SPDX-FileCopyrightText: 2015 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.h"

#include <QApplication>
#include <QColor>
#include <QCursor>
#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QPalette>
#include <QPixmap>
#include <QWindow>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QDBusConnection>

#define Window X11Window
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#undef Window
#include <X11/keysym.h>

#include <DConfig>
#include <DGuiApplicationHelper>
#include <DPlatformWindowHandle>

namespace {

const QString defaultLogoPath = QStringLiteral("/usr/share/deepin/uos_logo.svg");

const QString dsgQuitInterval = "blackWidgetQuitInterval";

const QString dsgShutdownText = "blackWidgetShutdownText";

const QString dsgBlackWidgetLogoVisible = "blackWidgetLogoVisible";

const QString dsgPath = "org.deepin.dde.session.ui";

const int shutdownTextBottomMargin = 260;

bool isValidThemeName(const QString &theme)
{
    return !theme.isEmpty()
           && !theme.contains(QStringLiteral(".."))
           && !theme.contains(QLatin1Char('/'))
           && !theme.contains(QLatin1Char('\\'));
}

QString plymouthLogoPath()
{
    const QString configPath = QStringLiteral("/etc/plymouth/plymouthd.conf");
    if (!QFileInfo(configPath).isFile()) {
        qWarning() << "plymouth config is missing, use default logo:" << configPath;
        return defaultLogoPath;
    }

    QSettings settings(configPath, QSettings::IniFormat);
    QString theme = settings.value(QStringLiteral("Daemon/Theme")).toString().trimmed();
    if (theme.isEmpty()) {
        theme = settings.value(QStringLiteral("Theme")).toString().trimmed();
    }
    if (theme.isEmpty()) {
        qWarning() << "plymouth theme is empty, use default logo";
        return defaultLogoPath;
    }
    if (!isValidThemeName(theme)) {
        qWarning() << "plymouth theme is invalid, use default logo:" << theme;
        return defaultLogoPath;
    }

    const QString themePath = QStringLiteral("/usr/share/plymouth/themes/%1").arg(theme);
    const QString logoPath = themePath + QStringLiteral("/logo.png");
    if (!QFileInfo(themePath).isDir() || !QFileInfo(logoPath).isFile()) {
        qWarning() << "plymouth logo is missing, use default logo:" << logoPath;
        return defaultLogoPath;
    }

    return logoPath;
}

Display *x11Display()
{
    auto *x11App = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
    return x11App ? x11App->display() : nullptr;
}

bool isX11Platform()
{
    static const bool isX11 = Dtk::Gui::DGuiApplicationHelper::isXWindowPlatform();
    return isX11;
}

QSize nativeResolution(const QScreen *screen)
{
    // 用 XRandR 读输出首选模式（EDID native），用户降分辨率/改缩放下仍能拿到面板
    // 真实分辨率（与 plymouth 的 get_preferred_mode 同源）；失败时回退 size()*DPR。
    const QSize fallback = screen ? screen->size() * screen->devicePixelRatio() : QSize();
    if (!screen || !isX11Platform())
        return fallback;
    Display *display = x11Display();
    if (!display)
        return fallback;

    int eventBase = 0, errorBase = 0;
    if (!XRRQueryExtension(display, &eventBase, &errorBase))
        return fallback;

    XRRScreenResources *resources = XRRGetScreenResources(display, DefaultRootWindow(display));
    if (!resources)
        return fallback;

    QSize native = fallback;
    for (int i = 0; i < resources->noutput; ++i) {
        XRROutputInfo *info = XRRGetOutputInfo(display, resources, resources->outputs[i]);
        if (!info || info->connection != RR_Connected || info->nmode <= 0) {
            if (info)
                XRRFreeOutputInfo(info);
            continue;
        }
        // modes[0] 按优先级排序，即首选（native）模式
        const XRRModeInfo *preferred = nullptr;
        for (int m = 0; m < resources->nmode; ++m) {
            if (resources->modes[m].id == info->modes[0]) {
                preferred = &resources->modes[m];
                break;
            }
        }
        if (preferred && info->name && screen->name() == QLatin1String(info->name)) {
            native = QSize(preferred->width, preferred->height);
            XRRFreeOutputInfo(info);
            break;
        }
        XRRFreeOutputInfo(info);
    }
    XRRFreeScreenResources(resources);
    return native;
}

qreal plymouthLogoScale(const QScreen *screen)
{
    // 复刻 plymouth 的 ply_get_device_scale（正常 DRM 路径，按物理 DPI 判定）：
    //   height < 1200 → 1；物理 DPI(x,y) 均 > 192 → 2；否则 1。
    // 数据源与 plymouth 同源（均来自 EDID）：native 分辨率 = size()*devicePixelRatio()
    // 对应 DRM 首选模式的 hdisplay/vdisplay，physicalSize() 对应 connector 的 mmWidth/mmHeight。
    if (!screen)
        return 1.0;

    const QSize native = nativeResolution(screen);
    if (native.height() < 1200)
        return 1.0;

    const int widthMm = qRound(screen->physicalSize().width());
    const int heightMm = qRound(screen->physicalSize().height());
    // 部分显示器把宽高比(16:9/16:10)编码进物理尺寸，是假值，plymouth 同样过滤
    if ((widthMm == 160 && heightMm == 90) ||
        (widthMm == 160 && heightMm == 100) ||
        (widthMm == 16 && heightMm == 9) ||
        (widthMm == 16 && heightMm == 10))
        return 1.0;

    if (widthMm > 0 && heightMm > 0) {
        const qreal dpiX = qreal(native.width()) / (qreal(widthMm) / 25.4);
        const qreal dpiY = qreal(native.height()) / (qreal(heightMm) / 25.4);
        if (dpiX > 192.0 && dpiY > 192.0)
            return 2.0;
    }
    return 1.0;
}

QPixmap makeLogoPixmap(const QScreen *screen)
{
    QPixmap logoPixmap(plymouthLogoPath());
    if (logoPixmap.isNull())
        return logoPixmap;

    const QSize native = nativeResolution(screen);
    const qreal deviceScale = plymouthLogoScale(screen);
    // logo 目标物理尺寸 = 原始像素 × deviceScale；再换算到当前 X 物理分辨率
    // （用户降分辨率后 X 物理像素比面板 native 少，需等比缩小才能与 plymouth 对齐）。
    const qreal xPhysical = screen->size().width() * screen->devicePixelRatio();
    const qreal factor = (native.width() > 0) ? deviceScale * (xPhysical / qreal(native.width())) : deviceScale;
    if (factor != 1.0)
        logoPixmap = logoPixmap.scaled(logoPixmap.size() * factor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logoPixmap.setDevicePixelRatio(screen ? screen->devicePixelRatio() : 1.0);
    return logoPixmap;
}

}

Window::Window(QWidget *parent)
    : QWidget(parent)
    , m_clickCount(0)
    , m_timer(new QTimer(this))
    , m_dConfig(Dtk::Core::DConfig::create(dsgPath, dsgPath, QString(), nullptr))
    , m_logo(nullptr)
    , m_shutdownTextLabel(nullptr)
    , m_logoVisible(false)
    , m_platformWindowInitialized(false)
    , m_raisePending(false)
    , m_nativeKeyboardGrabbed(false)
{
    setAccessibleName("Window");
    move(0, 0);
    Qt::WindowFlags flags = Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint;
    if (isX11Platform()) {
        flags |= Qt::X11BypassWindowManagerHint;
    } else {
        // Reserved for Treeland-specific window flags.
    }
    setWindowFlags(flags);
    setFocusPolicy(Qt::StrongFocus);

    setAutoFillBackground(true);
    QPalette pal(palette());
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    QCursor cursor(Qt::BlankCursor);
    setCursor(cursor);
}

Window::~Window()
{
    qDeleteAll(m_screenWindows);
}

QVariant Window::configValue(const QString &key, const QVariant &defaultValue) const
{
    return m_dConfig ? m_dConfig->value(key, defaultValue) : defaultValue;
}

void Window::setLogoVisible(bool visible)
{
    const bool logoVisible = configValue(dsgBlackWidgetLogoVisible, false).toBool();
    if (!logoVisible) {
        qInfo() << "dsg of blackWidgetLogoVisible is false";
        return;
    }
    m_logoVisible = visible;
    if (visible && !m_logo) {
        m_logo = new QLabel(this);
        m_logo->setAccessibleName("BlackWidgetLogo");
        m_logo->setVisible(false);
        const QPixmap logoPixmap = makeLogoPixmap(qApp->primaryScreen());
        if (logoPixmap.isNull()) {
            qWarning() << "blackwidget logo pixmap is null";
        } else {
            m_logo->setPixmap(logoPixmap);
        }
    }

    const bool hasLogo = m_logo && !m_logo->pixmap().isNull();
    if (m_logo) {
        m_logo->setVisible(visible && hasLogo);
    }
    if (!isHidden()) {
        setupSize();
    }
}

void Window::setupSize()
{
    updateScreenWindows();
    if (!m_logoVisible || !m_logo) {
        return;
    }

    // 每次重新生成缩放后的主屏 logo：分辨率/DPR/首选模式变化后仍与 plymouth 一致
    const QPixmap pixmap = makeLogoPixmap(qApp->primaryScreen());
    m_logo->setPixmap(pixmap);
    if (pixmap.isNull()) {
        m_logo->setVisible(false);
        return;
    }
    m_logo->setFixedSize(pixmap.size() / devicePixelRatioF());
    m_logo->move(rect().center() - m_logo->rect().center());
    m_logo->raise();

    const QString text = configValue(dsgShutdownText, QString()).toString();
    qInfo() << "blackwidget shutdown text:" << text;
    if (text.isEmpty()) {
        return;
    }
    if (!m_shutdownTextLabel) {
        m_shutdownTextLabel = new QLabel(this);
        m_shutdownTextLabel->setAccessibleName("BlackWidgetShutdownText");
        QPalette textPal = m_shutdownTextLabel->palette();
        textPal.setColor(QPalette::WindowText, QColor("#E6E6E6"));
        m_shutdownTextLabel->setPalette(textPal);
    }
    m_shutdownTextLabel->setText(text);
    m_shutdownTextLabel->adjustSize();
    const QPoint logoCenter = m_logo->geometry().center();
    m_shutdownTextLabel->move(logoCenter.x() - m_shutdownTextLabel->width() / 2, m_logo->y() + m_logo->height() + shutdownTextBottomMargin);
    m_shutdownTextLabel->setVisible(true);
    m_shutdownTextLabel->raise();
}

void Window::updateScreenWindows()
{
    const QList<QScreen *> screens = qApp->screens();
    if (screens.isEmpty()) {
        return;
    }

    QScreen *currentScreen = qApp->primaryScreen();
    if (!currentScreen) {
        currentScreen = screens.first();
        qWarning() << "primary screen is null, use first screen:" << currentScreen;
    }

    const QRect currentGeometry = currentScreen->geometry();
    if (geometry().topLeft() != currentGeometry.topLeft()) {
        move(currentGeometry.topLeft());
    }
    if (size() != currentGeometry.size()) {
        setFixedSize(currentGeometry.size());
    }

    const int coverWindowCount = screens.size() - 1;
    if (coverWindowCount <= 0) {
        while (!m_screenWindows.isEmpty()) {
            QWidget *screenWindow = m_screenWindows.takeLast();
            m_screenLogos.remove(screenWindow);
            delete screenWindow;
        }
        return;
    }
    while (m_screenWindows.size() < coverWindowCount) {
        QWidget *screenWindow = new QWidget(nullptr, Qt::WindowStaysOnTopHint | Qt::Tool | Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
        screenWindow->setAccessibleName("BlackWidgetScreenWindow");
        screenWindow->setAttribute(Qt::WA_ShowWithoutActivating);
        screenWindow->setAutoFillBackground(true);
        QPalette screenPal(screenWindow->palette());
        screenPal.setColor(QPalette::Window, Qt::black);
        screenWindow->setPalette(screenPal);
        screenWindow->setCursor(QCursor(Qt::BlankCursor));
        Dtk::Widget::DPlatformWindowHandle handle(screenWindow, screenWindow);
        handle.setWindowRadius(-1);

        QLabel *logo = new QLabel(screenWindow);
        logo->setAccessibleName("BlackWidgetLogo");
        logo->setVisible(false);
        m_screenLogos.insert(screenWindow, logo);
        m_screenWindows.append(screenWindow);
    }

    while (m_screenWindows.size() > coverWindowCount) {
        QWidget *screenWindow = m_screenWindows.takeLast();
        m_screenLogos.remove(screenWindow);
        delete screenWindow;
    }

    int coverIndex = 0;
    for (QScreen *screen : screens) {
        if (screen == currentScreen) {
            continue;
        }

        QWidget *screenWindow = m_screenWindows.at(coverIndex++);
        screenWindow->setGeometry(screen->geometry());
        screenWindow->setVisible(isVisible());
        if (!screenWindow->isHidden()) {
            screenWindow->raise();
        }

        QLabel *logo = m_screenLogos.value(screenWindow);
        if (logo) {
            const QPixmap pixmap = makeLogoPixmap(screen);
            logo->setPixmap(pixmap);
            logo->setFixedSize(pixmap.size() / screen->devicePixelRatio());
            logo->move(screenWindow->rect().center() - logo->rect().center());
            logo->setVisible(m_logoVisible && !pixmap.isNull());
            logo->raise();
        }
    }
}

void Window::setScreenWindowsVisible(bool visible)
{
    for (QWidget *screenWindow : qAsConst(m_screenWindows)) {
        screenWindow->setVisible(visible);
        QLabel *logo = m_screenLogos.value(screenWindow);
        if (logo) {
            logo->setVisible(visible && m_logoVisible);
        }
        if (visible) {
            screenWindow->raise();
        }
    }
}

bool Window::ensurePlatformReady()
{
    if (qApp->screens().isEmpty()) {
        return false;
    }

    if (isX11Platform() && !x11Display()) {
        return false;
    }

    return true;
}

void Window::initializePlatformWindow()
{
    if (m_platformWindowInitialized) {
        return;
    }

    Dtk::Widget::DPlatformWindowHandle handle(this, this);
    handle.setWindowRadius(-1);
    m_platformWindowInitialized = true;
}

void Window::scheduleRaiseWindow(int delay)
{
    if (m_raisePending) {
        return;
    }

    m_raisePending = true;
    QTimer::singleShot(delay, this, [this] {
        m_raisePending = false;
        raiseWindow();
    });
}

void Window::raiseWindow()
{
    if (!ensurePlatformReady()) {
        scheduleRaiseWindow();
        return;
    }

    initializePlatformWindow();
    setupSize();
    if (!isVisible()) {
        setVisible(true);
    }
    raise();
    setScreenWindowsVisible(true);
    activateWindow();
    setFocus(Qt::ActiveWindowFocusReason);
    QTimer::singleShot(0, this, [this] {
        if (!isVisible() || !ensurePlatformReady()) {
            return;
        }

        grabNativeKeyboard();
        grabMouse();
        grabKeyboard();
    });
}

bool Window::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Show:
        if (isVisible()) {
            setupSize();
            setScreenWindowsVisible(true);
            grabNativeKeyboard();
        }
        break;
    case QEvent::WindowActivate:
        if (isVisible()) {
            grabNativeKeyboard();
        }
        break;
    case QEvent::Hide:
        setScreenWindowsVisible(false);
        break;
    case QEvent::WindowDeactivate:
        if (!isHidden()) {
            scheduleRaiseWindow(30);
        }
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void Window::setTimer()
{
    const int interval = configValue(dsgQuitInterval, quitInterval / 1000).toInt() * 1000;
    qInfo() << "blackwidget setTimer interval :" << interval;
    m_timer->setSingleShot(true);
    m_timer->start(interval);
    connect(m_timer, &QTimer::timeout, this, [] {
        qApp->quit();
    });
}

void Window::grabNativeKeyboard()
{
    if (m_nativeKeyboardGrabbed || isHidden()) {
        return;
    }
    if (!isX11Platform()) {
        // Reserved for Treeland input inhibition.
        return;
    }

    Display *display = x11Display();
    if (!display) {
        return;
    }

    const X11Window window = static_cast<X11Window>(winId());
    if (window == 0) {
        return;
    }

    XSync(display, False);
    const int result = XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    if (result == GrabSuccess) {
        m_nativeKeyboardGrabbed = true;
        XSync(display, False);
    }
}

void Window::releaseNativeKeyboard()
{
    if (!m_nativeKeyboardGrabbed) {
        return;
    }

    m_nativeKeyboardGrabbed = false;
    if (!isX11Platform()) {
        // Reserved for Treeland input release.
        return;
    }

    Display *display = x11Display();
    if (!display) {
        return;
    }

    XUngrabKeyboard(display, CurrentTime);
    XSync(display, False);
}

void Window::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        return;
    }

    QWidget::mousePressEvent(event);
}

void Window::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        event->accept();
        return;
    }

    QWidget::mouseMoveEvent(event);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

void Window::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        ++m_clickCount;
        if (m_clickCount == 5) {
            setVisible(false);
            m_clickCount = 0;
        }
    }
}

void Window::onNameLost(QString name)
{
    qInfo() << "Window name lost: " << name;
    if (name == "org.deepin.dde.BlackScreen1") {
        qApp->quit();
    }
}

void Window::cleanupBeforeExit()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }

    if (mouseGrabber() == this) {
        releaseMouse();
    }

    if (keyboardGrabber() == this) {
        releaseKeyboard();
    }

    releaseNativeKeyboard();
    setScreenWindowsVisible(false);

    if (isVisible()) {
        hide();
    }
}

BlackWidgetAdaptor::BlackWidgetAdaptor(Window * parent)
    : QDBusAbstractAdaptor(parent)
{
}

BlackWidgetAdaptor::~BlackWidgetAdaptor()
{
}

Window *BlackWidgetAdaptor::parent() const
{
    return qobject_cast<Window *>(QObject::parent());
}

void BlackWidgetAdaptor::Raise()
{
    Window * w = parent();
    if (w) {
        w->raiseWindow();
    }
}

void BlackWidgetAdaptor::Quit()
{
    quitDBusService();
}

void BlackWidgetAdaptor::setActive(bool visible)
{
    auto *w = parent();
    if (w) {
        if (visible) {
            Raise();
            w->setTimer();
        } else {
            releaseGrabDevicesHideBlack();
        }
    }
}

void BlackWidgetAdaptor::quitDBusService()
{
    qInfo() << "Quit DBus service";
    QDBusConnection::sessionBus().unregisterObject("/org/deepin/dde/BlackScreen1");
    QDBusConnection::sessionBus().unregisterService("org.deepin.dde.BlackScreen1");
}

bool BlackWidgetAdaptor::blackScreenVisible()
{
    auto *w = parent();
    return w ? w->isVisible() : false;
}

void BlackWidgetAdaptor::releaseGrabDevicesHideBlack()
{
    qInfo() << Q_FUNC_INFO;
    Window * w = parent();
    if (w) {
        releaseGrabDevices();
        if (w->isVisible())
            w->setVisible(false);
    }
}

void BlackWidgetAdaptor::releaseGrabDevices()
{
    qInfo() << Q_FUNC_INFO;
    Window * w = parent();
    if (w) {
        w->releaseMouse();
        w->releaseKeyboard();
        w->releaseNativeKeyboard();
    }
}
