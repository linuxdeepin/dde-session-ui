# SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(UT_Welcome_Name ut-dde-welcome)

file(GLOB_RECURSE UT_Welcome_SRCS
    "*.h"
    "*.cpp"
    )
add_executable(${UT_Welcome_Name}
    ${Welcome_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    ${UT_Welcome_SRCS}
    ${DBUS_INTERFACE_SRCS}
    )

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Welcome_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Welcome_Name} PUBLIC
    ${Welcome_Includes}
    Qt5::Test
    ../src/
    )

target_link_libraries(${UT_Welcome_Name} PRIVATE
    ${Welcome_Libraries}
    ${Test_Libraries}
    Qt5::Test
    )
