/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "loginwindow.h"
#include "constants.h"
#include "lockworker.h"
#include "sessionbasemodel.h"
#include "propertygroup.h"

#include <DApplication>
#include <QtCore/QTranslator>
#include <QLabel>
#include <QProcess>
#include <QThread>
#include <QSettings>
#include <QWindow>
#include <QScreen>
#include <DLog>

#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib-xcb.h>
#include <X11/cursorfont.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/Xfixes.h>

DCORE_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

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

int main(int argc, char* argv[])
{
    Display *display = XOpenDisplay(NULL);

    XRRScreenResources *resources = XRRGetScreenResources(display, DefaultRootWindow(display));
    double scaleRatio = 1.0;

    for (int i = 0; i < resources->noutput; i++) {
        XRROutputInfo* outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
        if (outputInfo->crtc == 0 || outputInfo->mm_width == 0) continue;

        XRRCrtcInfo *crtInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
        if (crtInfo == nullptr) continue;

        scaleRatio = (double)crtInfo->width / (double)outputInfo->mm_width / (1366.0 / 310.0);

        if (scaleRatio > 1 + 2.0 / 3.0) {
            scaleRatio = 2;
        }
        else if (scaleRatio > 1 + 1.0 / 3.0) {
            scaleRatio = 1.5;
        }
        else {
            scaleRatio = 1;
        }
    }

    XRRFreeScreenResources(resources);

    // load dpi settings
    QSettings settings("/etc/lightdm/lightdm-deepin-greeter.conf", QSettings::IniFormat);
    const auto ratio = settings.value("ScreenScaleFactor", scaleRatio).toString();
    setenv("QT_SCALE_FACTOR", const_cast<char *>(ratio.toStdString().c_str()), 1);
    setenv("XCURSOR_SIZE", const_cast<char *>(QString::number(24.0 * ratio.toFloat()).toStdString().c_str()), 1);

    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    qApp->setOrganizationName("deepin");
    qApp->setApplicationName("lightdm-deepin-greeter");
    qApp->setApplicationVersion("2015.1.0");
    qApp->setAttribute(Qt::AA_ForceRasterWidgets);

    DLogManager::registerConsoleAppender();

    SessionBaseModel *model = new SessionBaseModel(SessionBaseModel::AuthType::LightdmType);
    LockWorker *worker = new LockWorker(model); //

    QObject::connect(model, &SessionBaseModel::authFinished, model, [=] {
        set_rootwindow_cursor();
    });

    PropertyGroup *property_group = new PropertyGroup(worker);

    property_group->addProperty("contentVisible");

    for (QScreen *screen : a.screens()) {
        LoginWindow *loginFrame = new LoginWindow(model);
        loginFrame->setScreen(screen);
        property_group->addObject(loginFrame);
        QObject::connect(loginFrame, &LoginWindow::requestSwitchToUser, worker, &LockWorker::switchToUser);
        QObject::connect(loginFrame, &LoginWindow::requestAuthUser, worker, &LockWorker::authUser);
        QObject::connect(loginFrame, &LoginWindow::requestSetLayout, worker, &LockWorker::setLayout);
        QObject::connect(worker, &LockWorker::requestUpdateBackground, loginFrame, static_cast<void (LoginWindow::*)(const QString &)>(&LoginWindow::updateBackground));
        loginFrame->show();
    }

    return a.exec();
}
