#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T10:28:12
#
#-------------------------------------------------

QT       += core gui dbus x11extras svg sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lX11 -lXext
TARGET = dde-osd
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += gtk+-2.0 dframeworkdbus dtkwidget dtkbase

SOURCES += main.cpp \
    container.cpp \
    manager.cpp \
    listview.cpp \
    delegate.cpp \
    model.cpp \
    osdprovider.cpp \
    audioprovider.cpp \
    common.cpp \
    brightnessprovider.cpp \
    kblayoutprovider.cpp \
    displaymodeprovider.cpp \
    indicatorprovider.cpp \
    wmstateprovider.cpp \
    kblayoutindicator.cpp \
    src/actionbutton.cpp \
    src/appbody.cpp \
    src/appicon.cpp \
    src/bubble.cpp \
    src/bubblemanager.cpp \
    src/dbus_daemon_interface.cpp \
    src/dbuscontrol.cpp \
    src/dbusdock_interface.cpp \
    src/dbuslogin1manager.cpp \
    src/icondata.cpp \
    src/notificationentity.cpp \
    src/notifications_dbus_adaptor.cpp \
    src/persistence.cpp

HEADERS  += container.h \
    manager.h \
    listview.h \
    delegate.h \
    model.h \
    common.h \
    osdprovider.h \
    audioprovider.h \
    brightnessprovider.h \
    kblayoutprovider.h \
    displaymodeprovider.h \
    indicatorprovider.h \
    wmstateprovider.h \
    kblayoutindicator.h \
    src/actionbutton.h \
    src/appbody.h \
    src/appicon.h \
    src/bubble.h \
    src/bubblemanager.h \
    src/dbus_daemon_interface.h \
    src/dbuscontrol.h \
    src/dbusdock_interface.h \
    src/dbuslogin1manager.h \
    src/icondata.h \
    src/notificationentity.h \
    src/notifications_dbus_adaptor.h \
    src/persistence.h

RESOURCES += \
    image.qrc \
    theme.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files += com.deepin.dde.osd.service
service_file.files += com.deepin.Notifications.service

INSTALLS += service_file

isEqual(ENABLE_AUTOSTART, YES) {
    autostart_file.path = /etc/xdg/autostart/
    autostart_file.files = dde-osd_autostart.desktop
    INSTALLS += autostart_file
}

DISTFILES += \
    src/src.pri \
    src/org.freedesktop.login1.Manager.xml
