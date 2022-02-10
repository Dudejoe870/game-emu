cmake_minimum_required (VERSION 3.8)

set(CORE_DEPS "gameemu-core-sm83" PARENT_SCOPE)

add_library (gameemu-core-gb SHARED "src/gb.cpp" "include/game-emu/cores/system/gb.h" "src/gameemu_interface.cpp")
target_include_directories (gameemu-core-gb PUBLIC "include")
target_link_libraries (gameemu-core-gb PRIVATE game-emu-common gameemu-core-sm83)

include(GNUInstallDirs)
install(TARGETS gameemu-core-gb RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
