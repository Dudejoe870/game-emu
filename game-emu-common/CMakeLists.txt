cmake_minimum_required (VERSION 3.8)

add_library (game-emu-common SHARED "src/core.cpp" "include/game-emu/common/core.h"  "include/game-emu/common/stdcommon.h" "include/game-emu/common/util.h" "src/util.cpp" "include/game-emu/common/coreloader.h" "src/coreloader.cpp" "include/game-emu/common/propertyvalue.h" "include/game-emu/common/coreinstance.h" "src/coreinstance.cpp" "include/game-emu/common/runloop.h" "src/runloop.cpp" "include/game-emu/common/instructionstream.h" "include/game-emu/common/instructiondecoder.h" "include/game-emu/common/runstate.h" "include/game-emu/common/endianutils.h" "include/game-emu/common/instructionbasedcoreinstance.h" "src/instructionbasedcoreinstance.cpp" "include/game-emu/common/corestate.h" "include/game-emu/common/register.h" "include/game-emu/common/physicaladdresstranslator.h" "include/game-emu/common/physicalmemorymap.h" "src/binarytreememorymap.cpp" "include/game-emu/common/logger.h")
target_include_directories (game-emu-common PUBLIC "include")
target_link_libraries(game-emu-common PUBLIC dynalo fmt-header-only LLVMCore)
target_compile_definitions(game-emu-common PRIVATE _LIBGAMEEMU_COMMON_DLL_EXPORT_SYMBOLS)

include(GNUInstallDirs)
install(TARGETS game-emu-common)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
