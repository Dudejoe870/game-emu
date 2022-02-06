cmake_minimum_required (VERSION 3.8)

add_executable (game-emu-cli "src/main.cpp")
target_link_libraries(game-emu-cli game-emu-common args cli)

include(GNUInstallDirs)
install(TARGETS game-emu-cli)
