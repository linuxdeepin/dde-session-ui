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

#include "mainwidget.h"
#include "updatecontent.h"
#include "constants.h"

#include <QGSettings/QGSettings>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>

#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>

//Load the System cursor --begin
static XcursorImages*
xcLoadImages(const char *image, int size)
{
    QSettings settings(DDESESSIONCC::DEFAULT_CURSOR_THEME, QSettings::IniFormat);
    //The default cursor theme path
    qDebug() << "Theme Path:" << DDESESSIONCC::DEFAULT_CURSOR_THEME;
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

static int set_rootwindow_cursor() {
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        qDebug() << "Open display failed";
        return -1;
    }

    const char *cursorPath = qApp->devicePixelRatio() > 1.7
        ? "/usr/share/icons/deepin/cursors/loginspinner@2x"
        : "/usr/share/icons/deepin/cursors/loginspinner";

    Cursor cursor = (Cursor)XcursorFilenameLoadCursor(display, cursorPath);
    if (cursor == 0) {
        cursor = (Cursor)loadCursorHandle(display, "watch", 24);
    }
    XDefineCursor(display, XDefaultRootWindow(display),cursor);

    // XFixesChangeCursorByName is the key to change the cursor
    // and the XFreeCursor and XCloseDisplay is also essential.

    XFixesChangeCursorByName(display, cursor, "watch");

    XFreeCursor(display, cursor);
    XCloseDisplay(display);

    return 0;
}
// Load system cursor --end

MainWidget::MainWidget(QWidget *parent)
    : FullscreenBackground(parent)
    , m_blurImageInter(new ImageBlur("com.deepin.daemon.Accounts",
                                     "/com/deepin/daemon/ImageBlur",
                                     QDBusConnection::systemBus(), this))
{
    connect(m_blurImageInter, &ImageBlur::BlurDone, this, &MainWidget::onBlurWallpaperFinished);
    connect(qApp, &QApplication::aboutToQuit, this, [=] {
        QProcess::startDetached("qdbus --literal com.deepin.daemon.Zone /com/deepin/daemon/Zone com.deepin.daemon.Zone.EnableZoneDetected true");
    });

    if (QFile::exists(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + "/autostart/dde-first-run.desktop")) {
        m_isUpgrade = false;

        QString currentVersion = getSystemVersion();

        if (!currentVersion.isEmpty()) {
            QSettings welcomeSetting("deepin", "dde-welcome");
            QString version = welcomeSetting.value("Version").toString();
            if (version.isEmpty()) {
                welcomeSetting.setValue("Version", currentVersion);
            }
        }
    } else {
        m_isUpgrade = checkVersionChanged();
    }

    QGSettings gsettings("com.deepin.dde.appearance", "", this);
    const QStringList list = gsettings.get("background-uris").toStringList();
    m_wallpaper = list.first();

    const QUrl url(m_wallpaper);
    m_wallpaper = url.isLocalFile() ? url.path() : m_wallpaper;

    if (m_isUpgrade) {
        UpdateContent *content = new UpdateContent(getSystemVersion(), this);
        setContent(content);

        // blur wallpaper
        const QString &w = m_blurImageInter->Get(m_wallpaper);

        updateBackground(w.isEmpty() ? m_wallpaper : w);

        QProcess::startDetached("qdbus --literal com.deepin.daemon.Zone /com/deepin/daemon/Zone com.deepin.daemon.Zone.EnableZoneDetected false");
    }

#ifdef QT_DEBUG
    showFullScreen();
    QTimer::singleShot(100, this, [=] {
        grabKeyboard();
    });
#endif
}

bool MainWidget::checkVersionChanged()
{
    const QString currentVersion = getSystemVersion();

    if (currentVersion.isEmpty())
        return false;

    // check file exist
    QSettings welcomeSetting("deepin", "dde-welcome");
    const QString version = welcomeSetting.value("Version").toString();

    welcomeSetting.setValue("Version", currentVersion);

    return version != currentVersion;
}

const QString MainWidget::getSystemVersion()
{
    QSettings lsbSetting("/etc/deepin-version", QSettings::IniFormat);
    lsbSetting.beginGroup("Release");
    return lsbSetting.value("Version").toString();
}

void MainWidget::onBlurWallpaperFinished(const QString &source, const QString &blur, bool status)
{
    const QString &sourcePath = QUrl(source).isLocalFile() ? QUrl(source).toLocalFile() : source;

    if (status && m_wallpaper == sourcePath)
        updateBackground(blur);
}

void MainWidget::dbus_show()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_isUpgrade)
        return;

    showFullScreen();

    QTimer::singleShot(100, this, [=] {
        grabKeyboard();
    });
}

void MainWidget::dbus_exit()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_isUpgrade) {
        qApp->quit();
    }
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (m_isUpgrade)
            qApp->quit();
    default:;
    }

    FullscreenBackground::keyPressEvent(event);
}
