#include "welcome.h"

#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QScreen>
#include <QSettings>

#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>

//Load the System cursor --begin
static XcursorImages*
xcLoadImages(const char *image, int size)
{
    QSettings settings("/usr/share/icons/default/index.theme", QSettings::IniFormat);
    //The default cursor theme path
    QString item = "Icon Theme";
    const char* defaultTheme = "";
    QVariant tmpCursorTheme = settings.value(item + "/Inherits");
    if (tmpCursorTheme.isNull()) {
        qDebug() << "Set a default one instead, if get the cursor theme failed!";
        defaultTheme = "Deepin";
    } else {
        defaultTheme = tmpCursorTheme.toString().toLocal8Bit().data();
    }

    qDebug() << "Get defaultTheme:" << tmpCursorTheme.isNull()
             << defaultTheme;
    return XcursorLibraryLoadImages(image, defaultTheme, size);
}

static unsigned long loadCursorHandle(Display *dpy, const char *name, int size)
{
    if (size == -1) {
        size = XcursorGetDefaultSize(dpy);
    }

    // Load the cursor images
    XcursorImages *images = NULL;
    images = xcLoadImages(name, size);

    if (!images) {
        images = xcLoadImages(name, size);
        if (!images) {
            return 0;
        }
    }

    unsigned long handle = (unsigned long)XcursorImagesLoadCursor(dpy,
                          images);
    XcursorImagesDestroy(images);

    return handle;
}

Welcome::Welcome(QWidget *parent)
    : QWidget(parent),

      m_sizeAdjustTimer(new QTimer(this))
{
    m_sizeAdjustTimer->setInterval(100);
    m_sizeAdjustTimer->setSingleShot(true);

    setMouseTracking(true);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    connect(m_sizeAdjustTimer, &QTimer::timeout, this, &Welcome::onScreenRectChanged, Qt::QueuedConnection);

    m_sizeAdjustTimer->start();
    QTimer::singleShot(1, this, &Welcome::overrideCursor);

#ifdef QT_DEBUG
    show();
#endif
}

Welcome::~Welcome()
{
}

void Welcome::dbus_show()
{
    qDebug() << Q_FUNC_INFO;

    show();
}

void Welcome::dbus_exit()
{
    qDebug() << Q_FUNC_INFO;

    qApp->quit();
}

void Welcome::overrideCursor()
{
//    const auto disp = XOpenDisplay(nullptr);
//    Q_ASSERT(disp);
//    const auto window = DefaultRootWindow(disp);

//    Cursor invisibleCursor;
//    Pixmap bitmapNoData;
//    XColor black;
//    static char noData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
//    black.red = black.green = black.blue = 0;

//    bitmapNoData = XCreateBitmapFromData(disp, window, noData, 8, 8);
//    invisibleCursor = XCreatePixmapCursor(disp, bitmapNoData, bitmapNoData,
//                                          &black, &black, 0, 0);
//    XDefineCursor(disp, window, invisibleCursor);
//    XFixesChangeCursorByName(disp, invisibleCursor, "watch");
//    XFreeCursor(disp, invisibleCursor);
//    XFreePixmap(disp, bitmapNoData);
//    XFlush(disp);

    // override cursor
    Display* disp = XOpenDisplay(nullptr);

    Cursor cursor = (Cursor)XcursorFilenameLoadCursor(disp, "/usr/share/icons/deepin/cursors/loginspinner");
    if (cursor == 0)
        cursor = (Cursor)loadCursorHandle(disp, "watch", 24);

    XDefineCursor(disp, winId(), cursor);
    XFixesChangeCursorByName(disp, cursor, "watch");
    XFreeCursor(disp, cursor);
    XFlush(disp);
}

void Welcome::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }

    QWidget::keyPressEvent(e);
}

void Welcome::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    // debug
//    painter.fillRect(QRect(0, 0, 500, 500), Qt::red);
}

void Welcome::showEvent(QShowEvent *e)
{
    qDebug() << Q_FUNC_INFO;

    QWidget::showEvent(e);

    QTimer::singleShot(1, this, [this] {
        raise();
        activateWindow();
        grabMouse();
        grabKeyboard();
        overrideCursor();
    });
}

void Welcome::mouseMoveEvent(QMouseEvent *e)
{
    e->ignore();

    QCursor::setPos(qApp->primaryScreen()->geometry().center());
}

void Welcome::onScreenRectChanged()
{
    setFixedSize(qApp->desktop()->size());

    QCursor::setPos(qApp->primaryScreen()->geometry().center());
}
