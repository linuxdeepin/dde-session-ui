TARGET = switchtouser
TEMPLATE = app

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0 gio-unix-2.0

include(../common.pri)

SOURCES += switchtogreeter.c

target.path = $${PREFIX}/bin/
INSTALLS += target
