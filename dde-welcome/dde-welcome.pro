
QT       += core gui dbus x11extras svg widgets

LIBS += -lX11 -lXext
TARGET = dde-welcome
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dframeworkdbus dtkwidget dtkbase dframeworkdbus

SOURCES += \
    main.cpp \
    welcome.cpp

HEADERS  += \
    welcome.h

RESOURCES += \

target.path = /usr/lib/deepin-daemon/
INSTALLS   += target

service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.welcome.service
INSTALLS += service_file
