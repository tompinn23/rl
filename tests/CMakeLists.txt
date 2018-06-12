

add_executable(rl_tests main.cpp lua.cpp)
target_link_libraries(rl_tests rl catch sol)

set_target_properties(rl_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)