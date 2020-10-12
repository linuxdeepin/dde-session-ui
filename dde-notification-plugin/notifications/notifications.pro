PREFIX = /usr
QT += core widgets dbus svg
TEMPLATE = lib
CONFIG += plugin c++11 link_pkgconfig
PKGCONFIG       += dtkwidget dframeworkdbus gsettings-qt

INCLUDEPATH += /usr/include/dde-dock

TARGET = $$qtLibraryTarget($$TARGET)

# Input
SOURCES += \
    notificationsplugin.cpp \
    notificationswidget.cpp

HEADERS += \
    notificationsplugin.h \
    notificationswidget.h


DISTFILES       += notifications.json

RESOURCES += \
    resources.qrc

target.path = /usr/lib/dde-dock/plugins/
gschema_files.path = /usr/share/glib-2.0/schemas/
gschema_files.files = gschema/com.deepin.dde.dock.module.notifications.gschema.xml
INSTALLS += target gschema_files
