# "CMAKE_CURRENT_SOURCE_DIR" changes in the different files were
# add_test_executable gets called. We can use this as a constant
set(ThisDirectory ${CMAKE_CURRENT_SOURCE_DIR})

# Adds a new executable cmake target to cmake.
# Links roadrunner-static and gtest.
# Parameters:
#   - BINARY: name of the binary
#   - OUT_VARIABLE: list variable to append BINARY. Creates the list if not exist.
#   - All further arguments are added to the binary as sources.
function(add_test_executable BINARY OUT_VARIABLE)
    add_executable(${BINARY} ${ARGN})

    target_include_directories(
            ${BINARY} PRIVATE
            "${ThisDirectory}"
    )

    target_link_libraries(${BINARY} PUBLIC roadrunner-static gtest gtest_main gmock gmock_main)

    add_dependencies(${BINARY} roadrunner-static gtest gtest_main gmock gmock_main)
    set_target_properties(${BINARY} PROPERTIES LINKER_LANGUAGE CXX)

    # Add to ctest.
    # add_test(NAME ${BINARY} COMMAND "$<TARGET_FILE:${BINARY}>")
    gtest_discover_tests(${BINARY})

    set(test_targets "${test_targets}" "${testid}")
    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${BINARY}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${BINARY}" PARENT_SCOPE)
    endif ()

endfunction()

add_subdirectory(CVODEIntegratorTests)
add_subdirectory(SundialsSteadyStateTests)