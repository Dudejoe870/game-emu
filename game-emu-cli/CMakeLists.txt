cmake_minimum_required (VERSION 3.8)

add_executable (game-emu-cli "src/main.cpp")
target_link_libraries(game-emu-cli libgame-emu-common)

add_custom_command(TARGET game-emu-cli POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:libgame-emu-common> $<TARGET_FILE_DIR:game-emu-cli>
)
