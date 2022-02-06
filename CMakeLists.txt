cmake_minimum_required (VERSION 3.8)

project ("game-emu")

set(CMAKE_CXX_STANDARD 20)

if(NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
	message(FATAL_ERROR "Unsupported Platform")
endif()

if(MSVC)
    add_compile_options("$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-Wall" "-Wextra" "$<$<CONFIG:RELEASE>:-O3>")
endif()

set(LLVM_BUILD_STATIC ON)

set(LLVM_OPTIMIZED_TABLEGEN ON)

set(LLVM_BUILD_LLVM_C_DYLIB OFF)
set(LLVM_LINK_LLVM_DYLIB OFF)

set(LLVM_BUILD_TESTS OFF)
set(LLVM_INCLUDE_TESTS OFF)

set(LLVM_INCLUDE_TOOLS OFF)
set(LLVM_BUILD_TOOLS OFF)

set(LLVM_INCLUDE_UTILS OFF)
set(LLVM_BUILD_UTILS OFF)

set(LLVM_INCLUDE_RUNTIMES OFF)
set(LLVM_BUILD_RUNTIMES OFF)

set(LLVM_ENABLE_BINDINGS OFF)
set(LLVM_ENABLE_OCAMLDOC OFF)
set(LLVM_INCLUDE_BENCHMARKS OFF)
set(LLVM_INCLUDE_EXAMPLES OFF)
set(LLVM_TARGETS_TO_BUILD "host")

add_subdirectory ("llvm-project/llvm" EXCLUDE_FROM_ALL)

add_subdirectory ("dynalo" EXCLUDE_FROM_ALL)

set(ARGS_BUILD_EXAMPLE OFF)
set(ARGS_BUILD_UNITTESTS OFF)

add_subdirectory ("args" EXCLUDE_FROM_ALL)
add_subdirectory ("cli" EXCLUDE_FROM_ALL)
add_subdirectory ("fmt" EXCLUDE_FROM_ALL)

add_subdirectory ("game-emu-common")
add_subdirectory ("game-emu-cli")

add_subdirectory ("gameemu-core-gbz80")
add_subdirectory ("gameemu-core-gb")
