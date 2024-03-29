CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_TESTS)

set(target llvm_testing)

if (BUILD_PYTHON)

    MESSAGE ("THIS IS PYTHON_INCLUDE_PATH " ${PYTHON_INCLUDE_PATH})

    INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

endif(BUILD_PYTHON)


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
    src/TestVariant
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
    ${SBMLSOLVER_DEP_DIR}/lib
    )

include_directories(
    src
    ${RR_ROOT}
    ${RR_ROOT}/wrappers/C
    ${SBMLSOLVER_DEP_DIR}/include/clapack
    )

if (BUILD_PYTHON)
    target_link_libraries (${target} ${PYTHON_LIBRARIES})
endif(BUILD_PYTHON)

if(UNIX)
    set(staticLibPrefix ".a")
    set(sharedLibPrefix ".so")
else()
    set(staticLibPrefix "")
    set(sharedLibPrefix "")
endif()


if(WIN32)
    target_link_libraries (${target}
        sbmlsolver_static
        unit_test-static${staticLibPrefix}
        )
endif()

if(UNIX)
    target_link_libraries (${target}
        sbmlsolver_c_api
        sbmlsolver_static
        libunit_test-static${staticLibPrefix}
        lapack
        blas
        f2c
        dl
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


