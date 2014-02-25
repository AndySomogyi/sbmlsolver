CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_TESTS)

set(target llvm_testing)


add_executable( ${target}
    main
    src/tests
    src/cpplapack
    src/CSRMatrixTest
    src/LLVMCSRMatrixTest
    src/test_compiler
    src/TestBase
    src/TestEvalInitialConditions
    src/TestGetSetValues
    src/TestEvalReactionRates
    src/TestEvalModel
    src/TestRoadRunner
    src/GetBoundarySpeciesAmountTest
    src/TestCapabilities
    src/ConfigurableTest
    )

set_property(TARGET ${target}
    PROPERTY  COMPILE_DEFINITIONS
    LIBSBML_USE_CPP_NAMESPACE
    LIBSBML_STATIC
    STATIC_LIBSTRUCT
    STATIC_PUGI
    STATIC_RR
    STATIC_NLEQ
    )

link_directories(
    ${THIRD_PARTY_INSTALL_FOLDER}/lib
    )

include_directories(
    src
    ${RR_ROOT}
    ${RR_ROOT}/wrappers/C
    ${THIRD_PARTY_INSTALL_FOLDER}/include/clapack
    )

if(UNIX)
    set(staticLibPrefix ".a")
    set(sharedLibPrefix ".so")
else()
    set(staticLibPrefix "")
    set(sharedLibPrefix "")
endif()


if(WIN32)
    target_link_libraries (${target}
        roadrunner-static
        unit_test-static${staticLibPrefix}
        poco_net-static
        
        )
endif()

if(UNIX)
    target_link_libraries (${target}
        roadrunner_c_api
        roadrunner-static
        libunit_test-static${staticLibPrefix}
        lapack
        blas
        f2c
        dl
        poco_net-static
        )
endif()

install (TARGETS ${target}
    DESTINATION bin
    COMPONENT testing
    )

install(
    FILES run_cpp_tests.bat
    DESTINATION bin
    COMPONENT testing
    )


