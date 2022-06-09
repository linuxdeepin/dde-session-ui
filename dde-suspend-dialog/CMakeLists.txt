file(GLOB_RECURSE Suspend_Dialog_SRCS
    "src/*.h"
    "src/*.cpp"
    )
list(REMOVE_ITEM Suspend_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-suspend-dialog/src/main.cpp")

set(Suspend_Dialog_Includes
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
    )
set(Suspend_Dialog_Libraries
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
    )

add_subdirectory("src")
add_subdirectory("tests")
