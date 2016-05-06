/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <gtk/gtk.h>
#include "osd.h"
#include <QApplication>
#include <QTranslator>

QString getThemeIconPath(QString iconName)
{
    QByteArray bytes = iconName.toUtf8();
    const char *name = bytes.constData();

    GtkIconTheme *theme = gtk_icon_theme_get_default();

    GtkIconInfo *info = gtk_icon_theme_lookup_icon(theme, name, 240, GTK_ICON_LOOKUP_GENERIC_FALLBACK);

    if (info) {
        char *path = g_strdup(gtk_icon_info_get_filename(info));
#if GTK_MAJOR_VERSION >= 3
        g_object_unref(info);
#elif GTK_MAJOR_VERSION == 2
        gtk_icon_info_free(info);
#endif
        return QString(path);
    } else {
        return "";
    }
}

void showThemeImage(QString iconName, QSvgWidget *svgLoader, QLabel *notSvgLoader)
{
    if (iconName.endsWith(".svg")) {
        svgLoader->load(iconName);
    } else if (iconName.isEmpty()) {
        svgLoader->load(getThemeIconPath("application-default-icon"));
    } else {
        // 56 is the size of image
        notSvgLoader->setPixmap(QPixmap(iconName).scaled(56, 56, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

int main(int argc, char *argv[])
{
    gtk_init(NULL, NULL);
    gdk_error_trap_push();

    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    Osd osd;

    //print help
    osd.processParser();

    //dbus-send --dest=com.deepin.dde.osd / com.deepin.dde.showosd  string:""
    OsdDBus osdDBus(&osd);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerObject("/",  &osdDBus,
                                   QDBusConnection::ExportAllSlots |
                                   QDBusConnection::ExportAllSignals |
                                   QDBusConnection::ExportAllProperties)) {
        qDebug() << "registerObject Error" << QDBusConnection::sessionBus().lastError();
        exit(0x0003);
    }

    if (connection.registerService("com.deepin.dde.osd")) {
        qDebug() << "registerService Successfully";
    } else {
        qDebug() << "registerService Failed" << QDBusConnection::sessionBus().lastError();
//        qDebug() << "Call dbus to show osd";
//        osd.dbusShowOSD();
        exit(0x0001);
    }

    return a.exec();
}
