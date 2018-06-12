

add_executable(test main.cpp lua.cpp)
target_link_libraries(test rl catch sol)

set_target_properties(test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)