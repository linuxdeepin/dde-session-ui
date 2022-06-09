set(Wm_Chooser_Name dde-wm-chooser)

add_executable(${Wm_Chooser_Name}
    ${Wm_Chooser_SRCS}
    ${Widgets_SRCS}
    ${Global_Util_SRCS}
    main.cpp
    )

target_include_directories(${Wm_Chooser_Name} PUBLIC
    ${Wm_Chooser_Includes}
    )

target_link_libraries(${Wm_Chooser_Name} PRIVATE
    ${Wm_Chooser_Libraries}
    )

## bin
install(TARGETS ${Wm_Chooser_Name} DESTINATION bin)
