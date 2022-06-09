file(GLOB_RECURSE OSD_SRCS
    "src/*.h"
    "src/*.cpp"
    "src/*.qrc"
    )

list(REMOVE_ITEM OSD_SRCS "${CMAKE_SOURCE_DIR}/dde-osd/src/main.cpp")

set(OSD_Includes
    ${DtkWidget_INCLUDE_DIRS}
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${GSETTINGS_INCLUDE_DIRS}
    Qt5::Concurrent
    Qt5::X11Extras
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    Qt5::Sql
    Qt5::Test
    )

set(OSD_Libraries
    ${DtkWidget_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    ${GSETTINGS_LIBRARIES}
    ${XCB_EWMH_LIBRARIES}
    Qt5::Concurrent
    Qt5::X11Extras
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    Qt5::Sql
    Qt5::Test
    )

add_subdirectory("src")
add_subdirectory("tests")
