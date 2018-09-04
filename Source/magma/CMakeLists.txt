add_library(magma Backend.cc Console.cc SDLBackend.cc input.cc)
target_include_directories(magma PUBLIC .)
target_include_directories(magma PUBLIC ${CMAKE_SOURCE_DIR}/Dependencies/sdl2/include)


target_link_libraries(magma SDL2 lodepng spdlog fmt)
