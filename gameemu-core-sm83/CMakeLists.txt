cmake_minimum_required (VERSION 3.8)

add_library (gameemu-core-sm83 SHARED "src/sm83.cpp" "include/game-emu/cores/processor/sm83.h" "src/gameemu_interface.cpp" "src/interpreter.cpp")
target_include_directories (gameemu-core-sm83 PUBLIC "include")
target_link_libraries (gameemu-core-sm83 PRIVATE gameemu-common fmt)

include(GNUInstallDirs)
install(TARGETS gameemu-core-sm83 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/game-emu-cores)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
