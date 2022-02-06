cmake_minimum_required (VERSION 3.8)

add_library (gameemu-core-gbz80 SHARED "src/gbz80.cpp" "include/game-emu/cores/processor/gbz80.h" "src/gameemu_interface.cpp")
target_include_directories (gameemu-core-gbz80 PUBLIC "include")
target_link_libraries (gameemu-core-gbz80 PRIVATE libgame-emu-common fmt)

add_custom_command(TARGET gameemu-core-gbz80 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gameemu-core-gbz80> $<TARGET_FILE_DIR:game-emu-cli>/cores/$<TARGET_FILE_NAME:gameemu-core-gbz80>
	VERBATIM
)

include(GNUInstallDirs)
install(TARGETS gameemu-core-gbz80 RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/cores LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}/cores)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
