cmake_minimum_required (VERSION 3.8)

project ("game-emu")

set(CMAKE_CXX_STANDARD 20)

if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
	message(FATAL_ERROR "Unsupported Platform")
endif()

set(ARGS_BUILD_EXAMPLE OFF)
set(ARGS_BUILD_UNITTESTS OFF)

if(MSVC)
    add_compile_options("$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O3>")
endif()

add_subdirectory ("dynalo" EXCLUDE_FROM_ALL)
add_subdirectory ("args" EXCLUDE_FROM_ALL)
add_subdirectory ("cli" EXCLUDE_FROM_ALL)
add_subdirectory ("fmt" EXCLUDE_FROM_ALL)

add_subdirectory ("libgame-emu-common")
add_subdirectory ("game-emu-cli")

add_subdirectory ("gameemu-core-gbz80")
add_subdirectory ("gameemu-core-gb")
