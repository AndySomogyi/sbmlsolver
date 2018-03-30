CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_Tests)

set(target run_test_suite)

set(tests
    Suite_Core_Tests
    Suite_TestModel
    Suite_SBML_Tests
    Suite_NOM_Tests
    Suite_LibStruct_Tests
    )

add_executable( ${target}
    main.cpp
    src/Args.cpp
    src/TestUtils
    src/rrSBMLTestSuiteSimulation_CAPI
    ${tests}
    )

set_property(TARGET ${target}
    PROPERTY  COMPILE_DEFINITIONS
    STATIC_LIBSTRUCT
    STATIC_RR
    POCO_STATIC
    XML_STATIC
    POCO_NO_AUTOMATIC_LIBS
    )

if(UNIX)
    set(staticLibPrefix ".a")
    set(sharedLibPrefix ".so")
endif()

target_link_libraries (${target}
    sbmlsolver_c_api
    unit_test-static
    PocoXML
    )

link_directories(
    ${SBMLSOLVER_DEP_DIR}/lib
    )

include_directories(
    "."
    ${RR_ROOT}
    ${THIRD_PARTY_FOLDER}/unit_test/src
    ${THIRD_PARTY_FOLDER}/rr-libstruct
)

# configure path.h 
set(SBML_TEST_SUITE_PATH ${SBMLSOLVER_DEP_DIR}/testing/sbml-semantic-test-cases/cases/semantic)
set(LOCAL_TEST_SUITE_PATH ${CMAKE_SOURCE_DIR}/testing)
configure_file(path.configure ${CMAKE_CURRENT_SOURCE_DIR}/path.h)

add_subdirectory(utils)

install (TARGETS ${target}
    DESTINATION bin COMPONENT testing
    )





