cmake_minimum_required (VERSION 3.8)

project ("game-emu")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory ("dynalo" EXCLUDE_FROM_ALL)
add_subdirectory ("libgame-emu-common")
add_subdirectory ("game-emu-cli")
