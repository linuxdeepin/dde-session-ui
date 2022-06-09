file(GLOB_RECURSE Notification_Plugin_SRCS
    "src/*.h"
    "src/*.cpp"
    "src/*.qrc"
    )

list(REMOVE_ITEM Notification_Plugin_SRCS "${CMAKE_SOURCE_DIR}/dde-notification-plugin/src/main.cpp")

set(Notification_Plugin_Includes
    ${DtkWidget_INCLUDE_DIRS}
    ${DdeDockInterface_INCLUDE_DIRS}
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${GSETTINGS_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    )

set(Notification_Plugin_Libraries
    ${DtkWidget_LIBRARIES}
    ${DdeDockInterface_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    ${GSETTINGS_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    )

add_subdirectory("src")
add_subdirectory("tests")
