cmake_minimum_required (VERSION 3.8)

add_library (gameemu-core-gbz80 SHARED "src/gbz80.cpp" "include/game-emu/cores/processor/gbz80.h" "src/gameemu_interface.cpp")
target_include_directories (gameemu-core-gbz80 PUBLIC "include")
target_link_libraries (gameemu-core-gbz80 PRIVATE game-emu-common fmt)

include(GNUInstallDirs)
install(TARGETS gameemu-core-gbz80 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
