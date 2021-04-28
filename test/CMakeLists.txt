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

# Adds a new executable cmake target to cmake.
# Links roadrunner-static and gtest.
# Parameters:
#   - TEST_TARGET: name of the binary
#   - OUT_VARIABLE: list variable to append BINARY. Creates the list if not exist.
#   - All further arguments are added to the binary as sources.
function(add_test_executable TEST_TARGET OUT_VARIABLE)
    add_executable(${TEST_TARGET} ${ARGN})

    target_include_directories(
            ${TEST_TARGET} PRIVATE
            "${ThisSourceDirectory}"
            "${ThisBinaryDirectory}"
    )

    target_link_libraries(${TEST_TARGET} PUBLIC
            roadrunner_c_api roadrunner-static gtest gtest_main gmock gmock_main
            )
    add_compile_definitions(${TEST_TARGET} PRIVATE STATIC_RR)

    add_dependencies(${TEST_TARGET} roadrunner-static gtest gtest_main gmock gmock_main)
    set_target_properties(${TEST_TARGET} PROPERTIES LINKER_LANGUAGE CXX)

    # Add to ctest.
    # add_test(NAME ${BINARY} COMMAND "$<TARGET_FILE:${BINARY}>")
    gtest_discover_tests(
            ${TEST_TARGET}
            TEST_LIST OUT_VARIABLE
            DISCOVERY_TIMEOUT 500
    )

    #    set_tests_properties(${TEST_TARGET} PROPERTIES TIMEOUT 500)

    set_target_properties(${TEST_TARGET} PROPERTIES ENVIRONMENT
            "testdir=${RR_ROOT}/test")

    if (WIN32)
        add_definitions(/bigobj)
    endif ()

    set(test_targets "${test_targets}" "${testid}")
    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${TEST_LIST}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${TEST_LIST}" PARENT_SCOPE)
    endif ()

endfunction()

set(test_targets)

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
add_subdirectory(semantic_STS)


#########################################################
##   Tests
##
#
#add_subdirectory(python)
#add_subdirectory(rrtest_files)
#
#set(TESTDIRS
#        c_api_core
#        c_api_rrtests
#        model_analysis
#        model_editing
#        sbml_features
#        state_saving
#        steady_state
#        #List the semantic_STS last, since it takes much longer to run than the others.
#        semantic_STS
#        )
#
#set(test_targets)
#foreach (dirid IN LISTS TESTDIRS)
#    #    message(STATUS "dirid = ${dirid}")
#    file(GLOB MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
#    file(GLOB UTIL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/test_util.cpp)
#    file(GLOB UTIL_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/test_util.h)
#    file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${dirid}/*.cpp)
#    file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${dirid}/*.h)
#
#    set(TEST_FILES ${MAIN_FILE} ${UTIL_FILE} ${UTIL_HEADER} ${CPP_FILES} ${H_FILES})
#
#    set(testid "test_${dirid}")
#    add_executable(${testid} ${TEST_FILES} RoadRunnerTest)
#
#    add_compile_definitions(${testid} PRIVATE STATIC_RR)
#
#    if (WIN32)
#        add_definitions(/bigobj)
#    endif ()
#
#    # This order is important:  on linux, including the core library before the C api library
#    # causes the resulting program to crash in Poco.
#    target_link_libraries(${testid}
#            roadrunner_c_api
#            roadrunner-static
#            gtest gtest_main
#            gmock gmock_main
#            )
#
#    set_target_properties(${testid} PROPERTIES ENVIRONMENT
#            "testdir=${RR_ROOT}/test")
#
#    target_include_directories(${testid} PRIVATE
#            "$<TARGET_PROPERTY:roadrunner_c_api,INCLUDE_DIRECTORIES>"
#            "${CMAKE_CURRENT_BINARY_DIR}"
#            )
#    add_test(${testid}_run ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testid})
#    # set the testdir variable for when using ctest to run tests
#
#    set(TEST_ENV_VARS "testdir=${RR_ROOT}/test" "CTEST_OUTPUT_ON_FAILURE=TRUE")
#    set_tests_properties(${testid}_run PROPERTIES ENVIRONMENT "${TEST_ENV_VARS}")
#
#    set(test_targets "${test_targets}" "${testid}")
#
#endforeach ()
#
#install(TARGETS ${test_targets})
#
#add_subdirectory(sundials-tests)
#
#add_custom_target(
#        run-ctest
#        ctest --progress --verbose --extra-verbose ${CMAKE_BINARY_DIR}
#)

