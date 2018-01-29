CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_TESTS)

set(target fbc_testing)


add_executable( ${target}
    main
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
    ${COINOR_CLP_INCLUDE_DIR}
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
        sbmlsolver-static
        unit_test-static${staticLibPrefix}
        )
endif()


message("COINOR_CLP_LIBRARIES: ${COINOR_CLP_LIBRARIES}")

if(UNIX)
  target_link_libraries (${target}
    sbmlsolver-static
    ${COINOR_CLP_LIBRARIES}
    bz2
    )
endif()

install (TARGETS ${target}
  DESTINATION bin
  COMPONENT testing
  )



