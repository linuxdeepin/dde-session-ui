cmake_minimum_required(VERSION 3.7)

project(dmemory-warning-dialog
        VERSION 1.0.0)

set(BIN_NAME dmemory-warning-dialog)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_FLAGS -Wall)

if (NOT (${CMAKE_BUILD_TYPE} MATCHES "Debug"))
    set(CMAKE_CXX_FLAGS -O3)
endif ()

# Sources files
file(GLOB SRCS "src/*.h" "src/*.cpp")

# Find the library
find_package(PkgConfig REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent X11Extras DBus)
find_package(Dtk REQUIRED COMPONENTS Widget)

pkg_check_modules(XLIBS REQUIRED x11)

# driver-manager
add_executable(${BIN_NAME} ${SRCS})
target_include_directories(${BIN_NAME} PUBLIC ${XLIBS_INCLUDE_DIRS} ${Qt5X11Extras_INCLUDE_DIRS})
target_link_libraries(${BIN_NAME} PRIVATE
    ${DtkWidget_LIBRARIES}
    ${Qt5Widgets_LIBRARIES}
    ${Qt5Concurrent_LIBRARIES}
    ${Qt5DBus_LIBRARIES}
    ${Qt5X11Extras_LIBRARIES}
    ${XLIBS_LIBRARIES}
)

