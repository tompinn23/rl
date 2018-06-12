# CMakeList.txt : CMake project for rlv3, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


#
# Configuration Section
#
include(CheckIncludeFileCXX)
check_include_file_cxx(dirent.h HAVE_DIRENT)
check_include_file_cxx(windows.h HAVE_WINDOWS)
check_include_file_cxx(sys/types.h HAVE_SYS_TYPES)
# Wait for filesystem to stabalize properly
#check_include_file_cxx(filesystem HAVE_FILESYSTEM)
#if(NOT HAVE_FILESYSTEM)
#	check_include_file_cxx(experimental/filesystem HAVE_FILESYSTEM_EXP)
#endif()
configure_file(config.h.in config.h)


#
# End Configuration
#




# Add the dependency directory.
add_subdirectory(Dependencies)


add_library(rl STATIC console.cpp u-util.cpp data.cpp u-file.cpp)
target_include_directories(rl PUBLIC ./)
target_include_directories(rl PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rl spdlog sol BearLibTerminal)

# Add source to this project's executable.
add_executable (rlv3 rlv3.cpp)
target_include_directories(rlv3 PRIVATE ./)
target_include_directories(rlv3 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rlv3 rl spdlog sol BearLibTerminal)
add_custom_command(TARGET rlv3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/data"
        $<TARGET_FILE_DIR:rlv3>)

set_target_properties(rl rlv3 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


if(MSVC)
	get_target_property(LIB_LOCATION BearLibTerminal  IMPORTED_LOCATION)
else()
	#get_target_property(LIB_LOCATION BearLibTerminal LOCATION)
endif()
add_custom_command(TARGET rlv3 POST_BUILD
	COMMENT "Copying Bearlibterminal"# Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
	$<TARGET_FILE:BearLibTerminal>      # <--this is in-file
        $<TARGET_FILE_DIR:rlv3>)

# TODO: Add tests and install targets if needed.
