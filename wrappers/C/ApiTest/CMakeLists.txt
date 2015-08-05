CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_Tests)

set(target test_new_api)

add_executable( ${target}
    main.c
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
    roadrunner_c_api
    unit_test-static
    PocoXML
    )

link_directories(
    ${THIRD_PARTY_INSTALL_FOLDER}/lib
    )

include_directories(
    "."
    ${RR_ROOT}
    ${THIRD_PARTY_FOLDER}/unit_test/src
${THIRD_PARTY_FOLDER}/rr-libstruct
)

install (TARGETS ${target}
    DESTINATION bin COMPONENT testing
    )

# Steady state solver test
add_executable(ss-solver-test ss-solver-test.c)
install(TARGETS ss-solver-test DESTINATION bin COMPONENT testing)
target_link_libraries (ss-solver-test roadrunner_c_api PocoXML)

set_property(TARGET ss-solver-test
    PROPERTY  COMPILE_DEFINITIONS
    STATIC_LIBSTRUCT
    STATIC_RR
    POCO_STATIC
    XML_STATIC
    POCO_NO_AUTOMATIC_LIBS
    )




