


if(MSVC)
	add_library(BearLibTerminal SHARED IMPORTED GLOBAL)


	math(EXPR BITNESS "8*${CMAKE_SIZEOF_VOID_P}")

	if(UNIX AND NOT APPLE)
		set(LINUX TRUE)
	endif()

	if(WIN32)
		if(BITNESS EQUAL "32")
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaries/Windows32/BearLibTerminal.dll")
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaries/Windows32/BearLibTerminal.lib")
		else()
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaries/Windows64/BearLibTerminal.dll")
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaries/Windows64/BearLibTerminal.lib")
		endif()
	elseif(LINUX)
	if(BITNESS EQUAL "32")
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaries/Linux32/libBearLibTerminal.so")
		else()
			set_target_properties(BearLibTerminal PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/binaires/Linux64/libBearLibTerminal.so")
		endif()
	else()
	message(FATAL_ERROR "Unsupported Platform")
	endif()
	target_include_directories(BearLibTerminal INTERFACE ${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/include)
else()
	
	add_subdirectory(source)
	target_include_directories(BearLibTerminal PUBLIC ${CMAKE_SOURCE_DIR}/rlv3/Dependencies/bearlib/source/Include/C)

endif()

