HEADERS += \
    $$PWD/interface/deepinauthinterface.h \
    $$PWD/authagent.h \
    $$PWD/deepinauthframework.h

SOURCES += \
    $$PWD/authagent.cpp \
    $$PWD/deepinauthframework.cpp

DBUS_ADAPTORS += $$PWD/dbus/com.deepin.daemon.authority.Agent.xml
DBUS_INTERFACES += $$PWD/dbus/com.deepin.daemon.Authority.xml

dbus.files += $$PWD/dbus/com.deepin.daemon.authority.Transaction.xml
dbus.header_flags += -N

DBUS_INTERFACES += dbus
