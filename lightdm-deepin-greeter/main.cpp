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

#include <DApplication>
#include <QtCore/QTranslator>
#include <QLabel>
#include <QProcess>
#include <QThread>
#include <QSettings>

#include <DLog>

#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

DCORE_USE_NAMESPACE

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

    LoginWindow lw;
    lw.showFullScreen();

    return a.exec();
}
