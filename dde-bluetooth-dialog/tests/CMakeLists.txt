set(UT_Bluetooth_Dialog_Name ut-dde-bluetooth-dialog)

file(GLOB_RECURSE UT_Bluetooth_Dialog_SRCS
    "*.cpp"
    )

add_executable(${UT_Bluetooth_Dialog_Name}
    ${Common_SRCS}
    ${Bluetooth_Dialog_SRCS}
    ${DBUS_INTERFACE_SRCS}
    ${UT_Bluetooth_Dialog_SRCS}
    )

# 用于测试覆盖率的编译条件
target_compile_options(${UT_Bluetooth_Dialog_Name} PRIVATE -fprofile-arcs -ftest-coverage)

target_include_directories(${UT_Bluetooth_Dialog_Name} PUBLIC
    ${Bluetooth_Dialog_Includes}
    ../src/
    )

target_link_libraries(${UT_Bluetooth_Dialog_Name} PRIVATE
    ${Bluetooth_Dialog_Libraries}
    ${Test_Libraries}
    )
