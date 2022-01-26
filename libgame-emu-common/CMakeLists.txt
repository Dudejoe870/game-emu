cmake_minimum_required (VERSION 3.8)

add_library (libgame-emu-common SHARED "src/core.cpp" "include/game-emu/common/core.h"  "include/game-emu/common/stdcommon.h" "include/game-emu/common/util.h" "src/util.cpp")
target_include_directories (libgame-emu-common PUBLIC "include")
target_link_libraries(libgame-emu-common PUBLIC dynalo)
target_compile_definitions(libgame-emu-common PRIVATE _LIBGAMEEMU_COMMON_DLL_EXPORT_SYMBOLS)
