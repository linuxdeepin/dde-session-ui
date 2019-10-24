isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/notifydelegate.h \
    $$PWD/notifymodel.h \
    $$PWD/notifyview.h \
    $$PWD/notifywidget.h \
    $$PWD/notifycenterwidget.h

SOURCES += \
    $$PWD/notifydelegate.cpp \
    $$PWD/notifymodel.cpp \
    $$PWD/notifyview.cpp \
    $$PWD/notifywidget.cpp \
    $$PWD/notifycenterwidget.cpp
