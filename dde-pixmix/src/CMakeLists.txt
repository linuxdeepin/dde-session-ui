set(Pixmix_Name dde-pixmix)

add_executable(${Pixmix_Name}
    ${Pixmix_SRCS}
    main.cpp
    )

target_include_directories(${Pixmix_Name} PUBLIC
    ${Pixmix_Includes}
    )

target_link_libraries(${Pixmix_Name} PRIVATE
    ${Pixmix_Libraries}
    )

## bin
install(TARGETS ${Pixmix_Name} DESTINATION bin)
