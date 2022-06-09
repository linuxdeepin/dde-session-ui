file(GLOB_RECURSE Welcome_SRCS
    "src/*.h"
    "src/*.cpp"
    )

list(REMOVE_ITEM Welcome_SRCS "${CMAKE_SOURCE_DIR}/dde-welcome/src/main.cpp")

set(Welcome_Includes
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    ${GSETTINGS_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Xml
    )

set(Welcome_Libraries
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${GSETTINGS_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Xml
    )

add_subdirectory("src")
add_subdirectory("tests")
