


function(add_test_executable BINARY OUT_VARIABLE)
    add_executable(${BINARY} ${ARGN})

    target_include_directories(${BINARY} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(${BINARY} PUBLIC roadrunner-static gtest gtest_main gmock gmock_main)

    add_dependencies(${BINARY} roadrunner-static gtest gtest_main gmock gmock_main)
    set_target_properties(${BINARY} PROPERTIES LINKER_LANGUAGE CXX)

    if ("${${OUT_VARIABLE}}" STREQUAL "")
        set(${OUT_VARIABLE} "${BINARY}" PARENT_SCOPE)
    else ()
        set(${OUT_VARIABLE} "${${OUT_VARIABLE}}" "${BINARY}" PARENT_SCOPE)
    endif ()

endfunction()

set(SHARED_SOURCES MockExecutableModel.h SBMLTestModelFactory.h)
add_test_executable(CVODEIntegratorUnitTest CiaranTests CVODEIntegratorUnitTest.cpp ${SHARED_SOURCES})
add_test_executable(CVODEIntegratorIntegrationTest CiaranTests CVODEIntegratorIntegrationTest.cpp ${SHARED_SOURCES})



























