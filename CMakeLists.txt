project(rl)

cmake_minimum_required(VERSION 3.10)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/lib CACHE
    PATH "Directory where all the .lib files are dumped." FORCE)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin CACHE
    PATH "Directory where .exe and .dll files are dumped." FORCE)

add_subdirectory(Dependencies)
add_subdirectory(Source)

