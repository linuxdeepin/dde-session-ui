set(UT_Notification_Plugin_Name ut-dde-notification-plugin)

file(GLOB_RECURSE UT_Notification_Plugin_SRCS
    "*.h"
    "*.cpp"
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
    ../src/
    )

target_link_libraries(${UT_Notification_Plugin_Name} PRIVATE
    ${Notification_Plugin_Libraries}
    ${Test_Libraries}
    Qt5::Test
    )
