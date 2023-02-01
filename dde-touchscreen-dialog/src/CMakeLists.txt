set(Touchscreen_Dialog_Name dde-touchscreen-dialog)

add_executable(${Touchscreen_Dialog_Name}
    ${Touchscreen_Dialog_SRCS}
    ${DBUS_INTERFACE_SRCS}
    main.cpp
    )

target_include_directories(${Touchscreen_Dialog_Name} PUBLIC
    ${Touchscreen_Dialog_Includes}
    )

target_link_libraries(${Touchscreen_Dialog_Name} PRIVATE
    ${Touchscreen_Dialog_Libraries}
    )

## bin
install(TARGETS ${Touchscreen_Dialog_Name} DESTINATION lib/deepin-daemon)
