#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T14:27:44
#
#-------------------------------------------------
INCLUDEPATH += $$PWD
QT += x11extras svg

include(../session-widgets/session-widgets.pri)

PKGCONFIG   += liblightdm-qt5-3 dtkwidget x11 xi xtst dframeworkdbus

HEADERS += \
    $$PWD/useravatar.h \
    $$PWD/userbutton.h \
    $$PWD/sessionwidget.h \
    $$PWD/userbreathinglabel.h \
    $$PWD/rounditembutton.h \
    $$PWD/kblayoutwidget.h \
    $$PWD/shutdownwidget.h \
    $$PWD/darrowrectangle.h \
    $$PWD/errortooltip.h \
    $$PWD/keyboardmonitor.h \
    $$PWD/mediawidget.h \
    $$PWD/controlwidget.h \
    $$PWD/fullscreenbackground.h \
    $$PWD/otheruserinput.h \
    $$PWD/virtualkbinstance.h \
    $$PWD/loginbutton.h \
    $$PWD/propertygroup.h



SOURCES += \
    $$PWD/useravatar.cpp \
    $$PWD/userbutton.cpp \
    $$PWD/sessionwidget.cpp \
    $$PWD/userbreathinglabel.cpp \
    $$PWD/rounditembutton.cpp \
    $$PWD/kblayoutwidget.cpp \
    $$PWD/shutdownwidget.cpp \
    $$PWD/darrowrectangle.cpp \
    $$PWD/errortooltip.cpp \
    $$PWD/keyboardmonitor.cpp \
    $$PWD/mediawidget.cpp \
    $$PWD/controlwidget.cpp \
    $$PWD/fullscreenbackground.cpp \
    $$PWD/otheruserinput.cpp \
    $$PWD/virtualkbinstance.cpp \
    $$PWD/loginbutton.cpp \
    $$PWD/propertygroup.cpp

RESOURCES += \
    $$PWD/widgetstheme.qrc \
    $$PWD/widgetsimages.qrc

DISTFILES +=

