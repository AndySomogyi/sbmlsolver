########################################################
#   Add the googletest framework as a subdirectory
#
add_subdirectory(googletest-release-1.10.0)
# we be naughty and use include_directories here to include googletest globally
include_directories(googletest-release-1.10.0/googletest/include)

# configure a text file in binary tree containing location of roadrunner root
# We use this root location to automatically find the testdir location (<rr_root>/test).
# This is not work in every case, so we fall back on testdir environment variable
set(RR_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file(RoadRunnerTestDirectory.h.in "${CMAKE_CURRENT_BINARY_DIR}/RoadRunnerTestDirectory.h" @ONLY)
configure_file(RoadRunnerTestDirectory.cpp.in "${CMAKE_CURRENT_BINARY_DIR}/RoadRunnerTestDirectory.cpp" @ONLY)

#file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/RoadRunnerRootLocation.txt" "${RR_ROOT}")
#message(STATUS "RoadRunner root location writen to binary tree \"${CMAKE_CURRENT_BINARY_DIR}/RoadRunnerRootLocation.txt\"")

############################################################
#   Function for creating roadrunner test binaries
#
# "CMAKE_CURRENT_SOURCE_DIR" changes in the different files were
# add_test_executable gets called. We can use this as a constant
set(ThisSourceDirectory ${CMAKE_CURRENT_SOURCE_DIR})
set(ThisBinaryDirectory ${CMAKE_CURRENT_BINARY_DIR})
set(TestModelFactorySrcDir "${CMAKE_CURRENT_SOURCE_DIR}/sundials-tests")

# Adds a new executable cmake target to cmake.
# Links roadrunner-static and gtest.
# Parameters:
#   - TEST_TARGET: name of the binary
#   - OUT_VARIABLE: list variable to append BINARY. Creates the list if not exist.
#   - All further arguments are added to the binary as sources.
function(add_test_executable TEST_TARGET OUT_VARIABLE)
    #message(STATUS "Adding test ${TEST_TARGET} as part of ${OUT_VARIABLE}. Source Files: ARGN: ${ARGN}" )
    add_executable(${TEST_TARGET} ${ARGN})

    target_include_directories(
            ${TEST_TARGET} PRIVATE
            "${ThisSourceDirectory}"
            "${ThisBinaryDirectory}"
            "${TestModelFactorySrcDir}"
    )

    target_link_libraries(${TEST_TARGET} PRIVATE
            roadrunner_c_api roadrunner-static gtest gtest_main gmock gmock_main
            )
    # This causes a weird build error whereby it is no longer possible
    # to name a class the name as the binary filename. However, since w
    # link to roadrunner-static, the flags needed should be transitive
    #    add_compile_definitions(${TEST_TARGET} PRIVATE STATIC_RR)

    add_dependencies(${TEST_TARGET} roadrunner-static gtest gtest_main gmock gmock_main)
    set_target_properties(${TEST_TARGET} PROPERTIES LINKER_LANGUAGE CXX)

    # Add to ctest.
    # add_test(NAME ${BINARY} COMMAND "$<TARGET_FILE:${BINARY}>")
    gtest_discover_tests(
            ${TEST_TARGET}
            DISCOVERY_TIMEOUT 500
            PROPERTIES TIMEOUT 500
    )

    set_target_properties(${TEST_TARGET} PROPERTIES ENVIRONMENT
            "testdir=${RR_ROOT}/test")

    if (WIN32)
        add_definitions(/bigobj)
    endif ()

    # helpful for debugging this function
    # message(STATUS "OUT_VARIABLE; ${${OUT_VARIABLE}} ${OUT_VARIABLE} OUT_VARIABLE"  )

    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${TEST_TARGET}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${TEST_TARGET}" PARENT_SCOPE)
    endif ()

endfunction()

#########################################################
#   Add Tests
#

set(
        SharedTestFiles
        "${CMAKE_CURRENT_BINARY_DIR}/RoadRunnerTestDirectory"
        ${CMAKE_CURRENT_SOURCE_DIR}/test_util
        ${CMAKE_CURRENT_SOURCE_DIR}/RoadRunnerTest
)

add_subdirectory(c_api_core)
add_subdirectory(c_api_rrtests)
add_subdirectory(model_analysis)
add_subdirectory(model_editing)
add_subdirectory(sbml_features)
add_subdirectory(state_saving)
add_subdirectory(sundials-tests)
add_subdirectory(python)
add_subdirectory(cxx_api_tests)

#List the semantic_STS last, since it takes much longer to run than the others.
add_subdirectory(semantic_STS)

#########################################################
#   Install tests and create wrapper for ctest command
#
install(TARGETS ${test_targets})

add_custom_target(
        run-ctest
        ctest --progress --verbose --extra-verbose ${CMAKE_BINARY_DIR}
)




