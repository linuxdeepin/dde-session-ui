TARGET = dde-switchtogreeter
TEMPLATE = app
QMAKE_CFLAGS += -std=c99

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0 gio-unix-2.0 libsystemd

include(../common.pri)

SOURCES += switchtogreeter.c

target.path = $${PREFIX}/bin/
INSTALLS += target
