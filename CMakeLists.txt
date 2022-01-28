cmake_minimum_required (VERSION 3.8)

project ("game-emu")

set(CMAKE_CXX_STANDARD 20)

if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin"))
	message(FATAL_ERROR "Unsupported Platform")
endif()

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
endif()

add_subdirectory ("dynalo" EXCLUDE_FROM_ALL)
add_subdirectory ("args" EXCLUDE_FROM_ALL)

add_subdirectory ("libgame-emu-common")
add_subdirectory ("gameemu-core-gbz80")
add_subdirectory ("gameemu-core-gb")
add_subdirectory ("game-emu-cli")
