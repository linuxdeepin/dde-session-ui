#include <gtk/gtk.h>
#include "osd.h"
#include <QApplication>
#include <QTranslator>

QString getThemeIconPath(QString iconName)
{
    QByteArray bytes = iconName.toUtf8();
    const char *name = bytes.constData();

    GtkIconTheme* theme = gtk_icon_theme_get_default();

    GtkIconInfo* info = gtk_icon_theme_lookup_icon(theme, name, 240, GTK_ICON_LOOKUP_GENERIC_FALLBACK);

    if (info) {
        char* path = g_strdup(gtk_icon_info_get_filename(info));
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

void showThemeImage(QString iconName, QSvgWidget* svgLoader, QLabel* notSvgLoader){
    if(iconName.endsWith(".svg")){
        svgLoader->load(iconName);
    }else if(iconName.isEmpty()){
        svgLoader->load(getThemeIconPath("application-default-icon"));
    }else{
        // 56 is the size of image
        notSvgLoader->setPixmap(QPixmap(iconName).scaled(56,56,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
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

    // To future maintainer: add your cmd and image as followings.
    osd.addCmdImage("NumLockOn","numlock-enabled-symbolic");
    osd.addCmdImage("NumLockOff","numlock-disabled-symbolic");
    osd.addCmdImage("CapsLockOn","capslock-enabled-symbolic");
    osd.addCmdImage("CapsLockOff","capslock-disabled-symbolic");
    osd.addCmdImage("TouchpadOn","touchpad-enabled-symbolic");
    osd.addCmdImage("TouchpadOff","touchpad-disabled-symbolic");
    osd.addCmdImage("TouchpadToggle","touchpad-toggled-symbolic");
    osd.addCmdImageWithText("SwitchWM3D", "wm-effect-enabled", a.translate("Osd", "Enable window effects"));
    osd.addCmdImageWithText("SwitchWM2D","wm-effect-disabled",a.translate("Osd", "Disable window effects"));
    osd.addCmdImageWithText("SwitchWMError","wm-effect-error",a.translate("Osd", "Failed to enable window effects"));

    osd.processParser();
    if(!osd.handleBasicCmd() && !osd.handleAdditionalCmd() && !osd.handleAdditionalCmdWithText()){
        return 0;
    }
    osd.show();

    return a.exec();
}
