cmake_minimum_required (VERSION 3.8)

add_library (gameemu-core-gb SHARED "src/gb.cpp" "include/game-emu/cores/system/gb.h" "src/gameemu_interface.cpp")
target_include_directories (gameemu-core-gb PUBLIC "include")
target_link_libraries (gameemu-core-gb PRIVATE game-emu-common gameemu-core-gbz80)

include(GNUInstallDirs)
install(TARGETS gameemu-core-gb RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
