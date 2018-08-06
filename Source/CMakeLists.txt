add_executable(rl main.cc)
target_include_directories(rl PUBLIC ${CMAKE_SOURCE_DIR}/Dependencies/sdl2/include)

add_subdirectory(magma)

target_link_libraries(rl magma)

add_custom_command(TARGET rl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:rl>)
