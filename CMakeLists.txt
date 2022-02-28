cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME dde-session-ui)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS "-g -Wall")

# 增加安全编译参数
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-all -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-all -fPIC")
set(CMAKE_EXE_LINKER_FLAGS  "-z relro -z now -z noexecstack -pie")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fsanitize=address -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -O2")
endif()

if (DEFINED ENABLE_MIEEE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mieee")
endif()

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

    # generate qm
    execute_process(COMMAND bash "translate_generation.sh"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

# Find the library
find_package(PkgConfig REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(Qt5 COMPONENTS Widgets DBus X11Extras Xml Concurrent Svg Sql Test REQUIRED)

pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(GSETTINGS REQUIRED gsettings-qt)
pkg_check_modules(DdeDockInterface REQUIRED dde-dock)
pkg_check_modules(XCB_EWMH REQUIRED xcb-ewmh)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GIO REQUIRED gio-unix-2.0)
pkg_check_modules(SYSTEMD REQUIRED libsystemd)

function(generation_dbus_adaptor xml class_name class_file option)
    execute_process(COMMAND qdbusxml2cpp ${option} -a ${class_file} -c ${class_name} ${xml}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction(generation_dbus_adaptor)

include_directories(
    common
    global_util
    widgets
)

set(Test_Libraries
    -lpthread
    -lgcov
    -lgtest
)

#----------------------------install config------------------------------
## qm files
file(GLOB QM_FILES "translations/*.qm")
install(FILES ${QM_FILES} DESTINATION share/${PROJECT_NAME}/translations)

#----------------------------common sources------------------------------
file(GLOB_RECURSE Common_SRCS
    "common/*.h"
    "common/*.cpp"
)

file(GLOB_RECURSE Widgets_SRCS
    "widgets/*.h"
    "widgets/*.cpp"
)

file(GLOB_RECURSE Global_Util_SRCS
    "global_util/*.h"
    "global_util/*.cpp"
)

#--------------------------dde-bluetooth-dialog--------------------------
set(Bluetooth_Dialog_Name dde-bluetooth-dialog)
file(GLOB_RECURSE Bluetooth_Dialog_SRCS
    "dde-bluetooth-dialog/*.h"
    "dde-bluetooth-dialog/*.cpp"
)
list(REMOVE_ITEM Bluetooth_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-bluetooth-dialog/main.cpp")

add_executable(${Bluetooth_Dialog_Name}
    ${Common_SRCS}
    ${Bluetooth_Dialog_SRCS}
    dde-bluetooth-dialog/main.cpp
)
set(Bluetooth_Dialog_Includes
    ${DtkWidget_INCLUDE_DIRS}
    ${GSETTINGS_INCLUDE_DIRS}
    ${DFrameworkDBus_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
)
set(Bluetooth_Dialog_Libraries
    ${DtkWidget_LIBRARIES}
    ${GSETTINGS_LIBRARIES}
    ${DFrameworkDBus_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
)
target_include_directories(${Bluetooth_Dialog_Name} PUBLIC
    ${Bluetooth_Dialog_Includes}
)

target_link_libraries(${Bluetooth_Dialog_Name} PRIVATE
    ${Bluetooth_Dialog_Libraries}
)

## bin
install(TARGETS ${Bluetooth_Dialog_Name} DESTINATION lib/deepin-daemon)

#-------------------------ut-dde-bluetooth-dialog-------------------------
set(UT_Bluetooth_Dialog_Name ut-dde-bluetooth-dialog)
file(GLOB_RECURSE UT_Bluetooth_Dialog_SRCS
    "tests/dde-bluetooth-dialog/*.h"
    "tests/dde-bluetooth-dialog/*.cpp"
)
add_executable(${UT_Bluetooth_Dialog_Name}
    ${Common_SRCS}
    ${Bluetooth_Dialog_SRCS}
    ${UT_Bluetooth_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Bluetooth_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Bluetooth_Dialog_Name} PUBLIC
    ${Bluetooth_Dialog_Includes}
    dde-bluetooth-dialog/
)

target_link_libraries(${UT_Bluetooth_Dialog_Name} PRIVATE
    ${Bluetooth_Dialog_Libraries}
    ${Test_Libraries}
)

#---------------------------dde-hints-dialog---------------------------
set(Hints_Dialog_Name dde-hints-dialog)
file(GLOB_RECURSE Hints_Dialog_SRCS
    "dde-hints-dialog/*.h"
    "dde-hints-dialog/*.cpp"
)
list(REMOVE_ITEM Hints_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-hints-dialog/main.cpp")

add_executable(${Hints_Dialog_Name}
    ${Hints_Dialog_SRCS}
    dde-hints-dialog/main.cpp
)
target_include_directories(${Hints_Dialog_Name} PUBLIC
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
)

target_link_libraries(${Hints_Dialog_Name} PRIVATE
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
)

## bin
install(TARGETS ${Hints_Dialog_Name} DESTINATION bin)

#--------------------------dde-license-dialog--------------------------
set(License_Dialog_Name dde-license-dialog)
file(GLOB_RECURSE License_Dialog_SRCS
    "dde-license-dialog/*.h"
    "dde-license-dialog/*.cpp"
)
list(REMOVE_ITEM License_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-license-dialog/main.cpp")

add_executable(${License_Dialog_Name}
    ${Common_SRCS}
    ${License_Dialog_SRCS}
    dde-license-dialog/main.cpp
)
set(License_Dialog_Includes
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::Concurrent
)
set(License_Dialog_Libraries
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
    Qt5::Concurrent
)
target_include_directories(${License_Dialog_Name} PUBLIC
    ${License_Dialog_Includes}
)

target_link_libraries(${License_Dialog_Name} PRIVATE
    ${License_Dialog_Libraries}
)

## bin
install(TARGETS ${License_Dialog_Name} DESTINATION bin)

#-------------------------ut-dde-license-dialog-------------------------
set(UT_License_Dialog_Name ut-dde-license-dialog)
file(GLOB_RECURSE UT_License_Dialog_SRCS
    "tests/dde-license-dialog/*.h"
    "tests/dde-license-dialog/*.cpp"
)
add_executable(${UT_License_Dialog_Name}
    ${Common_SRCS}
    ${License_Dialog_SRCS}
    ${UT_License_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_License_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_License_Dialog_Name} PUBLIC
    ${License_Dialog_Includes}
    dde-license-dialog/
)

target_link_libraries(${UT_License_Dialog_Name} PRIVATE
    ${License_Dialog_Libraries}
    ${Test_Libraries}
)

#------------------------------dde-lowpower-----------------------------
set(Lowpower_Name dde-lowpower)
file(GLOB_RECURSE Lowpower_SRCS
    "dde-lowpower/*.h"
    "dde-lowpower/*.cpp"
)
list(REMOVE_ITEM Lowpower_SRCS "${CMAKE_SOURCE_DIR}/dde-lowpower/main.cpp")

add_executable(${Lowpower_Name}
    ${Lowpower_SRCS}
    dde-lowpower/main.cpp
)
set(Lowpower_Includes
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
)
set(Lowpower_Libraries
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
)

target_include_directories(${Lowpower_Name} PUBLIC
    ${Lowpower_Includes}
)

target_link_libraries(${Lowpower_Name} PRIVATE
    ${Lowpower_Libraries}
)

## bin
install(TARGETS ${Lowpower_Name} DESTINATION lib/deepin-daemon)

#----------------------------ut-dde-lowpower----------------------------
set(UT_Lowpower_Name ut-dde-lowpower)
file(GLOB_RECURSE UT_Lowpower_SRCS
    "tests/dde-lowpower/*.h"
    "tests/dde-lowpower/*.cpp"
)
add_executable(${UT_Lowpower_Name}
    ${Lowpower_SRCS}
    ${UT_Lowpower_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Lowpower_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Lowpower_Name} PUBLIC
    ${Lowpower_Includes}
    Qt5::Test
    dde-lowpower/
)

target_link_libraries(${UT_Lowpower_Name} PRIVATE
    ${Lowpower_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#-------------------------dde-notification-plugin------------------------
set(Notification_Plugin_Name notifications)
file(GLOB_RECURSE Notification_Plugin_SRCS
    "dde-notification-plugin/*.h"
    "dde-notification-plugin/*.cpp"
    "dde-notification-plugin/*.qrc"
)
add_library(${Notification_Plugin_Name} MODULE
    ${Notification_Plugin_SRCS}
)
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
target_include_directories(${Notification_Plugin_Name} PUBLIC
    ${Notification_Plugin_Includes}
)

target_link_libraries(${Notification_Plugin_Name} PRIVATE
    ${Notification_Plugin_Libraries}
)

## bin
install(TARGETS ${Notification_Plugin_Name} LIBRARY DESTINATION lib/dde-dock/plugins)

## schemas
install(FILES dde-notification-plugin/gschema/com.deepin.dde.dock.module.notifications.gschema.xml DESTINATION share/glib-2.0/schemas)

#-------------------------ut-dde-notification-plugin------------------------
set(UT_Notification_Plugin_Name ut-dde-notification-plugin)
file(GLOB_RECURSE UT_Notification_Plugin_SRCS
    "tests/dde-notification-plugin/*.h"
    "tests/dde-notification-plugin/*.cpp"
)
add_executable(${UT_Notification_Plugin_Name}
    ${Notification_Plugin_SRCS}
    ${UT_Notification_Plugin_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Notification_Plugin_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Notification_Plugin_Name} PUBLIC
    ${Notification_Plugin_Includes}
    Qt5::Test
    dde-notification-plugin/
)

target_link_libraries(${UT_Notification_Plugin_Name} PRIVATE
    ${Notification_Plugin_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#--------------------------------dde-osd-------------------------------
set(OSD_Name dde-osd)
file(GLOB_RECURSE OSD_SRCS
    "dde-osd/*.h"
    "dde-osd/*.cpp"
    "dde-osd/*.qrc"
)
list(REMOVE_ITEM OSD_SRCS "${CMAKE_SOURCE_DIR}/dde-osd/main.cpp")

add_executable(${OSD_Name}
    ${OSD_SRCS}
    dde-osd/main.cpp
)
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
    dde-osd/
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

target_include_directories(${OSD_Name} PUBLIC
    ${OSD_Includes}
)

target_link_libraries(${OSD_Name} PRIVATE
    ${OSD_Libraries}
)

## bin
install(TARGETS ${OSD_Name} DESTINATION lib/deepin-daemon)

## service
file(GLOB OSD_SERVICE_FILES "dde-osd/files/*.service")
install(FILES ${OSD_SERVICE_FILES} DESTINATION share/dbus-1/services)

#-----------------------------ut-dde-osd-----------------------------
#--todo: 单元测试编译不通过，需修改后取消注释

# set(UT_OSD_Name ut-dde-osd)
# file(GLOB_RECURSE UT_OSD_SRCS
#     "tests/dde-osd/*.h"
#     "tests/dde-osd/*.cpp"
# )
# add_executable(${UT_OSD_Name}
#     ${OSD_SRCS}
#     ${UT_OSD_SRCS}
# )

# # 用于测试覆盖率的编译条件
# target_compile_options(${UT_OSD_Name} PRIVATE -fprofile-arcs -ftest-coverage)

# target_include_directories(${UT_OSD_Name} PUBLIC
#     ${OSD_Includes}
#     tests/dde-osd/
#     dde-osd/notification/
#     dde-osd/notification-center/
# )

# target_link_libraries(${UT_OSD_Name} PRIVATE
#     ${OSD_Libraries}
#     ${Test_Libraries}
# )

#-------------------------------dde-pixmix-----------------------------
set(Pixmix_Name dde-pixmix)
file(GLOB_RECURSE Pixmix_SRCS
    "dde-pixmix/*.h"
    "dde-pixmix/*.cpp"
)
list(REMOVE_ITEM Pixmix_SRCS "${CMAKE_SOURCE_DIR}/dde-pixmix/main.cpp")

add_executable(${Pixmix_Name}
    ${Pixmix_SRCS}
    dde-pixmix/main.cpp
)
set(Pixmix_Includes
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
)
set(Pixmix_Libraries
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
)

target_include_directories(${Pixmix_Name} PUBLIC
    ${Pixmix_Includes}
)

target_link_libraries(${Pixmix_Name} PRIVATE
    ${Pixmix_Libraries}
)

## bin
install(TARGETS ${Pixmix_Name} DESTINATION bin)

#-------------------------------dde-suspend-dialog-----------------------------
set(Suspend_Dialog_Name dde-suspend-dialog)
file(GLOB_RECURSE Suspend_Dialog_SRCS
    "dde-suspend-dialog/*.h"
    "dde-suspend-dialog/*.cpp"
)
list(REMOVE_ITEM Suspend_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-suspend-dialog/main.cpp")

add_executable(${Suspend_Dialog_Name}
    ${Suspend_Dialog_SRCS}
    dde-suspend-dialog/main.cpp
)
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

target_include_directories(${Suspend_Dialog_Name} PUBLIC
    ${Suspend_Dialog_Includes}
)

target_link_libraries(${Suspend_Dialog_Name} PRIVATE
    ${Suspend_Dialog_Libraries}
)

## bin
install(TARGETS ${Suspend_Dialog_Name} DESTINATION lib/deepin-daemon)

## icons
install(FILES dde-suspend-dialog/data/computer.svg DESTINATION share/icons/hicolor/scalable/devices)

#-------------------------ut-dde-suspend-dialog------------------------
set(UT_Suspend_Dialog_Name ut-dde-suspend-dialog)
file(GLOB_RECURSE UT_Suspend_Dialog_SRCS
    "tests/dde-suspend-dialog/*.h"
    "tests/dde-suspend-dialog/*.cpp"
)
add_executable(${UT_Suspend_Dialog_Name}
    ${Suspend_Dialog_SRCS}
    ${UT_Suspend_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Suspend_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Suspend_Dialog_Name} PUBLIC
    ${Suspend_Dialog_Includes}
    Qt5::Test
    dde-suspend-dialog/
)

target_link_libraries(${UT_Suspend_Dialog_Name} PRIVATE
    ${Suspend_Dialog_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#-------------------------------dde-switchtogreeter-----------------------------
set(Switchtogreeter_Name dde-switchtogreeter)
file(GLOB_RECURSE Switchtogreeter_SRCS
    "dde-switchtogreeter/*.h"
    "dde-switchtogreeter/*.c"
)

add_executable(${Switchtogreeter_Name}
    ${Switchtogreeter_SRCS}
)
set(Switchtogreeter_Includes
    ${SYSTEMD_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GIO_INCLUDE_DIRS}
)
set(Switchtogreeter_Libraries
    ${SYSTEMD_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${GIO_LIBRARIES}
)

target_include_directories(${Switchtogreeter_Name} PUBLIC
    ${Switchtogreeter_Includes}
)

target_link_libraries(${Switchtogreeter_Name} PRIVATE
    ${Switchtogreeter_Libraries}
)

## bin
install(TARGETS ${Switchtogreeter_Name} DESTINATION bin)

#-------------------------------dde-touchscreen-dialog-----------------------------
set(Touchscreen_Dialog_Name dde-touchscreen-dialog)
file(GLOB_RECURSE Touchscreen_Dialog_SRCS
    "dde-touchscreen-dialog/*.h"
    "dde-touchscreen-dialog/*.cpp"
)
list(REMOVE_ITEM Touchscreen_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-touchscreen-dialog/main.cpp")

add_executable(${Touchscreen_Dialog_Name}
    ${Touchscreen_Dialog_SRCS}
    dde-touchscreen-dialog/main.cpp
)
set(Touchscreen_Dialog_Includes
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::X11Extras
    Qt5::Widgets
    Qt5::DBus
)
set(Touchscreen_Dialog_Libraries
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    Qt5::X11Extras
    Qt5::Widgets
    Qt5::DBus
    -lXext
)

target_include_directories(${Touchscreen_Dialog_Name} PUBLIC
    ${Touchscreen_Dialog_Includes}
)

target_link_libraries(${Touchscreen_Dialog_Name} PRIVATE
    ${Touchscreen_Dialog_Libraries}
)

## bin
install(TARGETS ${Touchscreen_Dialog_Name} DESTINATION lib/deepin-daemon)

#-------------------------ut-dde-touchscreen-dialog------------------------
set(UT_Touchscreen_Dialog_Name ut-dde-touchscreen-dialog)
file(GLOB_RECURSE UT_Touchscreen_Dialog_SRCS
    "tests/dde-touchscreen-dialog/*.h"
    "tests/dde-touchscreen-dialog/*.cpp"
)
add_executable(${UT_Touchscreen_Dialog_Name}
    ${Touchscreen_Dialog_SRCS}
    ${UT_Touchscreen_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Touchscreen_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Touchscreen_Dialog_Name} PUBLIC
    ${Touchscreen_Dialog_Includes}
    dde-touchscreen-dialog/
)

target_link_libraries(${UT_Touchscreen_Dialog_Name} PRIVATE
    ${Touchscreen_Dialog_Libraries}
    ${Test_Libraries}
)

#-------------------------------dde-warning-dialog-----------------------------
set(Warning_Dialog_Name dde-warning-dialog)
file(GLOB_RECURSE Warning_Dialog_SRCS
    "dde-warning-dialog/*.h"
    "dde-warning-dialog/*.cpp"
)
list(REMOVE_ITEM Warning_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dde-warning-dialog/main.cpp")

add_executable(${Warning_Dialog_Name}
    ${Warning_Dialog_SRCS}
    dde-warning-dialog/main.cpp
)
set(Warning_Dialog_Includes
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
)
set(Warning_Dialog_Libraries
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
)

target_include_directories(${Warning_Dialog_Name} PUBLIC
    ${Warning_Dialog_Includes}
)

target_link_libraries(${Warning_Dialog_Name} PRIVATE
    ${Warning_Dialog_Libraries}
)

## bin
install(TARGETS ${Warning_Dialog_Name} DESTINATION lib/deepin-daemon)

## service
install(FILES dde-warning-dialog/com.deepin.dde.WarningDialog.service DESTINATION share/dbus-1/services)

#-------------------------ut-dde-warning-dialog------------------------
set(UT_Warning_Dialog_Name ut-dde-warning-dialog)
file(GLOB_RECURSE UT_Warning_Dialog_SRCS
    "tests/dde-warning-dialog/*.h"
    "tests/dde-warning-dialog/*.cpp"
)
add_executable(${UT_Warning_Dialog_Name}
    ${Warning_Dialog_SRCS}
    ${UT_Warning_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Warning_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Warning_Dialog_Name} PUBLIC
    ${Warning_Dialog_Includes}
    dde-warning-dialog/
)

target_link_libraries(${UT_Warning_Dialog_Name} PRIVATE
    ${Warning_Dialog_Libraries}
    ${Test_Libraries}
)

#-------------------------------dde-welcome-----------------------------
set(Welcome_Name dde-welcome)
file(GLOB_RECURSE Welcome_SRCS
    "dde-welcome/*.h"
    "dde-welcome/*.cpp"
)
list(REMOVE_ITEM Welcome_SRCS "${CMAKE_SOURCE_DIR}/dde-welcome/main.cpp")

add_executable(${Welcome_Name}
    ${Welcome_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    dde-welcome/main.cpp
)
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

target_include_directories(${Welcome_Name} PUBLIC
    ${Welcome_Includes}
)

target_link_libraries(${Welcome_Name} PRIVATE
    ${Welcome_Libraries}
)

## bin
install(TARGETS ${Welcome_Name} DESTINATION lib/deepin-daemon)

## service
install(FILES dde-welcome/com.deepin.dde.welcome.service DESTINATION share/dbus-1/services)

#-------------------------ut-dde-welcome------------------------
set(UT_Welcome_Name ut-dde-welcome)
file(GLOB_RECURSE UT_Welcome_SRCS
    "tests/dde-welcome/*.h"
    "tests/dde-welcome/*.cpp"
)
add_executable(${UT_Welcome_Name}
    ${Welcome_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    ${UT_Welcome_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Welcome_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Welcome_Name} PUBLIC
    ${Welcome_Includes}
    Qt5::Test
    dde-welcome/
)

target_link_libraries(${UT_Welcome_Name} PRIVATE
    ${Welcome_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#-------------------------------dde-wm-chooser-----------------------------
set(Wm_Chooser_Name dde-wm-chooser)
file(GLOB_RECURSE Wm_Chooser_SRCS
    "dde-wm-chooser/*.h"
    "dde-wm-chooser/*.cpp"
)
list(REMOVE_ITEM Wm_Chooser_SRCS "${CMAKE_SOURCE_DIR}/dde-wm-chooser/main.cpp")

add_executable(${Wm_Chooser_Name}
    ${Wm_Chooser_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    dde-wm-chooser/main.cpp
)
set(Wm_Chooser_Includes
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    ${GSETTINGS_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Xml
)
set(Wm_Chooser_Libraries
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    ${GSETTINGS_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
    Qt5::Xml
)

target_include_directories(${Wm_Chooser_Name} PUBLIC
    ${Wm_Chooser_Includes}
)

target_link_libraries(${Wm_Chooser_Name} PRIVATE
    ${Wm_Chooser_Libraries}
)

## bin
install(TARGETS ${Wm_Chooser_Name} DESTINATION bin)

#-------------------------ut-dde-wm-chooser------------------------
set(UT_Wm_Chooser_Name ut-dde-wm-chooser)
file(GLOB_RECURSE UT_Wm_Chooser_SRCS
    "tests/dde-wm-chooser/*.h"
    "tests/dde-wm-chooser/*.cpp"
)
add_executable(${UT_Wm_Chooser_Name}
    ${Wm_Chooser_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    ${UT_Wm_Chooser_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Wm_Chooser_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Wm_Chooser_Name} PUBLIC
    ${Wm_Chooser_Includes}
    Qt5::Test
    dde-wm-chooser/
)

target_link_libraries(${UT_Wm_Chooser_Name} PRIVATE
    ${Wm_Chooser_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#-------------------------------dmemory-warning-dialog-----------------------------
set(Dmemory_Warning_Dialog_Name dmemory-warning-dialog)
file(GLOB_RECURSE Dmemory_Warning_Dialog_SRCS
    "dmemory-warning-dialog/*.h"
    "dmemory-warning-dialog/*.cpp"
)
list(REMOVE_ITEM Dmemory_Warning_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dmemory-warning-dialog/main.cpp")

add_executable(${Dmemory_Warning_Dialog_Name}
    ${Dmemory_Warning_Dialog_SRCS}
    dmemory-warning-dialog/main.cpp
)
set(Dmemory_Warning_Dialog_Includes
    ${DFrameworkDBus_INCLUDE_DIRS}
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
    Qt5::DBus
)
set(Dmemory_Warning_Dialog_Libraries
    ${DFrameworkDBus_LIBRARIES}
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
    Qt5::DBus
)

target_include_directories(${Dmemory_Warning_Dialog_Name} PUBLIC
    ${Dmemory_Warning_Dialog_Includes}
)

target_link_libraries(${Dmemory_Warning_Dialog_Name} PRIVATE
    ${Dmemory_Warning_Dialog_Libraries}
)

## bin
install(TARGETS ${Dmemory_Warning_Dialog_Name} DESTINATION bin)

## service
install(FILES dmemory-warning-dialog/com.deepin.dde.MemoryWarningDialog.service DESTINATION share/dbus-1/services)

#-------------------------ut-dmemory-warning-dialog------------------------
set(UT_Dmemory_Warning_Dialog_Name ut-dmemory-warning-dialog)
file(GLOB_RECURSE UT_Dmemory_Warning_Dialog_SRCS
    "tests/dmemory-warning-dialog/*.h"
    "tests/dmemory-warning-dialog/*.cpp"
)
add_executable(${UT_Dmemory_Warning_Dialog_Name}
    ${Dmemory_Warning_Dialog_SRCS}
    ${UT_Dmemory_Warning_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Dmemory_Warning_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Dmemory_Warning_Dialog_Name} PUBLIC
    ${Dmemory_Warning_Dialog_Includes}
    Qt5::Test
    dmemory-warning-dialog/
)

target_link_libraries(${UT_Dmemory_Warning_Dialog_Name} PRIVATE
    ${Dmemory_Warning_Dialog_Libraries}
    ${Test_Libraries}
    Qt5::Test
)

#-------------------------------dnetwork-secret-dialog-----------------------------
set(Dnetwork_Secret_Dialog_Name dnetwork-secret-dialog)
file(GLOB_RECURSE Dnetwork_Secret_Dialog_SRCS
    "dnetwork-secret-dialog/*.h"
    "dnetwork-secret-dialog/*.cpp"
)
list(REMOVE_ITEM Dnetwork_Secret_Dialog_SRCS "${CMAKE_SOURCE_DIR}/dnetwork-secret-dialog/main.cpp")

add_executable(${Dnetwork_Secret_Dialog_Name}
    ${Dnetwork_Secret_Dialog_SRCS}
    dnetwork-secret-dialog/main.cpp
)
set(Dmemory_Warning_Dialog_Includes
    ${DtkWidget_INCLUDE_DIRS}
    Qt5::Widgets
)
set(Dnetwork_Secret_Dialog_Libraries
    ${DtkWidget_LIBRARIES}
    Qt5::Widgets
)

target_include_directories(${Dnetwork_Secret_Dialog_Name} PUBLIC
    ${Dnetwork_Secret_Dialog_Includes}
)

target_link_libraries(${Dnetwork_Secret_Dialog_Name} PRIVATE
    ${Dnetwork_Secret_Dialog_Libraries}
)

## bin
install(TARGETS ${Dnetwork_Secret_Dialog_Name} DESTINATION lib/deepin-daemon)

#-------------------------ut-dnetwork-secret-dialog------------------------
set(UT_Dnetwork_Secret_Dialog_Name ut-dnetwork-secret-dialog)
file(GLOB_RECURSE UT_Dnetwork_Secret_Dialog_SRCS
    "tests/dnetwork-secret-dialog/*.h"
    "tests/dnetwork-secret-dialog/*.cpp"
)
add_executable(${UT_Dnetwork_Secret_Dialog_Name}
    ${Dnetwork_Secret_Dialog_SRCS}
    ${UT_Dnetwork_Secret_Dialog_SRCS}
)

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Dnetwork_Secret_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Dnetwork_Secret_Dialog_Name} PUBLIC
    ${Dnetwork_Secret_Dialog_Includes}
    dnetwork-secret-dialog/
)

target_link_libraries(${UT_Dnetwork_Secret_Dialog_Name} PRIVATE
    ${Dnetwork_Secret_Dialog_Libraries}
    ${Test_Libraries}
)
