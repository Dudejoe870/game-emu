cmake_minimum_required (VERSION 3.8)

add_executable (gameemu-cli "src/main.cpp")
target_link_libraries(gameemu-cli gameemu-common args cli)

include(GNUInstallDirs)
install(TARGETS gameemu-cli)
