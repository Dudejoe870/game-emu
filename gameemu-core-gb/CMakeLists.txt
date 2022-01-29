cmake_minimum_required (VERSION 3.8)

add_library (gameemu-core-gb SHARED "src/gb.cpp" "include/game-emu/cores/system/gb.h" "src/gameemu_interface.cpp")
target_include_directories (gameemu-core-gb PUBLIC "include")
target_link_libraries (gameemu-core-gb PRIVATE libgame-emu-common gameemu-core-gbz80)

add_custom_command(TARGET gameemu-core-gb POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:gameemu-core-gb> $<TARGET_FILE_DIR:game-emu-cli>/cores/$<TARGET_FILE_NAME:gameemu-core-gb>
	VERBATIM
)
