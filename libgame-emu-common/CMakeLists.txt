cmake_minimum_required (VERSION 3.8)

add_library (libgame-emu-common STATIC "src/core.cpp" "include/core.h")
target_include_directories (libgame-emu-common PUBLIC "include")
target_link_libraries(libgame-emu-common PRIVATE dynalo)
