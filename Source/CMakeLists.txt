add_executable(rl main.cc z-graphics.cc terminal.cc)
target_include_directories(rl PUBLIC ${CMAKE_SOURCE_DIR}/Dependencies/sdl2/include)

target_link_libraries(rl lodepng SDL2 spdlog)

add_custom_command(TARGET rl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:rl>)
