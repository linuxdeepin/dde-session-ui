PREFIX = /usr
QT += core widgets dbus svg
TEMPLATE = lib
CONFIG += plugin c++11 link_pkgconfig
PKGCONFIG       += dtkwidget dframeworkdbus

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

target.path = /usr/lib/dde-dock/plugins/system-trays/
INSTALLS += target
