cmake_minimum_required (VERSION 3.8)

add_library (libgame-emu-common SHARED "src/core.cpp" "include/game-emu/common/core.h"  "include/game-emu/common/stdcommon.h" "include/game-emu/common/util.h" "src/util.cpp" "include/game-emu/common/coreloader.h" "src/coreloader.cpp" "include/game-emu/common/propertyvalue.h" "include/game-emu/common/coreinstance.h" "src/coreinstance.cpp" "include/game-emu/common/runloop.h" "src/runloop.cpp" "include/game-emu/common/cpu/instructionstream.h" "include/game-emu/common/cpu/instructiondecoder.h"  "include/game-emu/common/cpu/cpu.h" "src/cpu/cpu.cpp" "include/game-emu/common/runstate.h")
target_include_directories (libgame-emu-common PUBLIC "include")
target_link_libraries(libgame-emu-common PUBLIC dynalo fmt-header-only)
target_compile_definitions(libgame-emu-common PRIVATE _LIBGAMEEMU_COMMON_DLL_EXPORT_SYMBOLS)
set_target_properties(libgame-emu-common PROPERTIES PREFIX "")
