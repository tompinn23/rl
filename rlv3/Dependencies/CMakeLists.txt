# CMakeLists.txt : All the dependencies for rlv3

# Add duktape
# add_subdirectory(duktape)

# Add bearlibterminal
add_subdirectory(bearlib)

# Add lua
add_subdirectory(lua)

add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE ./)

add_library(sol INTERFACE)
target_include_directories(sol INTERFACE ./sol)
target_link_libraries(sol INTERFACE libluadll)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ./catch)
