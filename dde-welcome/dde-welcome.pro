include(../common.pri)
QT       += core gui dbus svg widgets x11extras

TARGET = dde-welcome
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget dframeworkdbus gsettings-qt x11 xext xfixes xcursor

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)

SOURCES += \
    main.cpp \
    updatecontent.cpp \
    mainwidget.cpp

HEADERS  += \
    updatecontent.h \
    mainwidget.h \
    utils.h

RESOURCES += \
    resources.qrc

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.welcome.service
INSTALLS += service_file
