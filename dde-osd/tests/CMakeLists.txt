set(UT_OSD_Name ut-dde-osd)

file(GLOB_RECURSE UT_OSD_SRCS
    "*.h"
    "*.cpp"
    )

add_executable(${UT_OSD_Name}
    ${OSD_SRCS}
    ${UT_OSD_SRCS}
    )

# 用于测试覆盖率的编译条件
target_compile_options(${UT_OSD_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_OSD_Name} PUBLIC
    ${OSD_Includes}
    Qt5::Test
    ../src/
    ../src/notification/
    ../src/notification-center/
    )

target_link_libraries(${UT_OSD_Name} PRIVATE
    ${OSD_Libraries}
    Qt5::Test
    ${Test_Libraries}
    )
