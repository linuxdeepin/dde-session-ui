isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/notifywidget.h \
    $$PWD/notifymodel.h \
    $$PWD/notifycenterwidget.h \
    $$PWD/bubbleitem.h \
    $$PWD/bubblegroup.h \
    $$PWD/appgroupmodel.h \
    $$PWD/appgroupdelegate.h \
    $$PWD/bubbledelegate.h \
    $$PWD/notifycommon.h \
    $$PWD/expandanimation.h

SOURCES += \
    $$PWD/notifywidget.cpp \
    $$PWD/notifymodel.cpp \
    $$PWD/notifycenterwidget.cpp \
    $$PWD/bubbleitem.cpp \
    $$PWD/bubblegroup.cpp \
    $$PWD/appgroupmodel.cpp \
    $$PWD/appgroupdelegate.cpp \
    $$PWD/bubbledelegate.cpp \
    $$PWD/expandanimation.cpp
