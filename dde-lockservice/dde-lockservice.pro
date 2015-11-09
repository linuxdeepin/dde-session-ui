TARGET = dde-lockservice
TEMPLATE = app
QMAKE_CFLAGS += -std=c99

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0 gio-unix-2.0 
LIBS += -lcrypt

include(../common.pri)

SOURCES += lockservice.c

target.path = $${PREFIX}/bin/

conf_file.files += com.deepin.dde.lock.conf 
conf_file.path = /etc/dbus-1/system.d

service_file.files += com.deepin.dde.lock.service
service_file.path = /usr/share/dbus-1/system-services

INSTALLS += target conf_file service_file
