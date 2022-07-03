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

set(GAME_EMU_ALL_CORES 
    "gameemu-core-gb"
)

set(GAME_EMU_CORE_LIST "all" CACHE STRING "A semicolon separated List of System cores to build (can be \"all\")")

if(GAME_EMU_CORE_LIST STREQUAL "all")
    set(GAME_EMU_CORE_LIST ${GAME_EMU_ALL_CORES})
endif()

set(LLVM_BUILD_STATIC ON CACHE INTERNAL "" FORCE)

set(LLVM_OPTIMIZED_TABLEGEN ON CACHE INTERNAL "" FORCE)

set(LLVM_BUILD_LLVM_C_DYLIB OFF CACHE INTERNAL "" FORCE)
set(LLVM_LINK_LLVM_DYLIB OFF CACHE INTERNAL "" FORCE)

set(LLVM_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(LLVM_INCLUDE_TESTS OFF CACHE INTERNAL "" FORCE)

set(LLVM_INCLUDE_TOOLS OFF CACHE INTERNAL "" FORCE)
set(LLVM_BUILD_TOOLS OFF CACHE INTERNAL "" FORCE)

set(LLVM_INCLUDE_UTILS OFF CACHE INTERNAL "" FORCE)
set(LLVM_BUILD_UTILS OFF CACHE INTERNAL "" FORCE)

set(LLVM_INCLUDE_RUNTIMES OFF CACHE INTERNAL "" FORCE)
set(LLVM_BUILD_RUNTIMES OFF CACHE INTERNAL "" FORCE)

set(LLVM_ENABLE_BINDINGS OFF CACHE INTERNAL "" FORCE)
set(LLVM_ENABLE_OCAMLDOC OFF CACHE INTERNAL "" FORCE)
set(LLVM_INCLUDE_BENCHMARKS OFF CACHE INTERNAL "" FORCE)
set(LLVM_INCLUDE_EXAMPLES OFF CACHE INTERNAL "" FORCE)
set(LLVM_TARGETS_TO_BUILD "host" CACHE INTERNAL "" FORCE)

add_subdirectory ("llvm-project/llvm" EXCLUDE_FROM_ALL)

add_subdirectory ("dynalo" EXCLUDE_FROM_ALL)

set(ARGS_BUILD_EXAMPLE OFF CACHE INTERNAL "" FORCE)
set(ARGS_BUILD_UNITTESTS OFF CACHE INTERNAL "" FORCE)

add_subdirectory ("args" EXCLUDE_FROM_ALL)
add_subdirectory ("cli" EXCLUDE_FROM_ALL)
add_subdirectory ("fmt" EXCLUDE_FROM_ALL)

configure_file("uninstall.cmake" "uninstall.cmake" NO_SOURCE_PERMISSIONS @ONLY)
install(CODE "execute_process(COMMAND \"${CMAKE_COMMAND}\" -P \"${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake\")")

add_subdirectory ("gameemu-common")
add_subdirectory ("gameemu-cli")

function(add_core_dep CORE)
    list(FIND GAME_EMU_CORE_LIST ${CORE} CORE_INDEX)
    if (NOT ${CORE_INDEX} EQUAL -1)
        return()
    endif()
    message(STATUS "Adding Core Dependency ${CORE}...")
    set(CORE_DEPS "")
    add_subdirectory(${CORE})
    foreach (DEP ${CORE_DEPS})
        add_core_dep(${DEP})
    endforeach()
endfunction()

foreach (CORE ${GAME_EMU_CORE_LIST})
    message(STATUS "Adding Core ${CORE}...")
    set(CORE_DEPS "")
    add_subdirectory(${CORE})
    foreach (DEP ${CORE_DEPS})
        add_core_dep(${DEP})
    endforeach()
endforeach()

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
)
