set(MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")

if(NOT EXISTS ${MANIFEST})
    message(FATAL_ERROR "Cannot find install manifest: '${MANIFEST}'")
endif()

file(STRINGS ${MANIFEST} FILES)
foreach(FILE ${FILES})
    if(EXISTS ${FILE})
        message(STATUS "Removing file: '${FILE}'")

        exec_program(
            ${CMAKE_COMMAND} ARGS "-E rm \"${FILE}\""
            OUTPUT_VARIABLE stdout
            RETURN_VALUE result
        )
        
        if(NOT ${result} EQUAL 0)
            message(FATAL_ERROR "Failed to remove file: '${FILE}'.")
        endif()
    else()
        MESSAGE(STATUS "File '${FILE}' does not exist.")
    endif()
endforeach()
