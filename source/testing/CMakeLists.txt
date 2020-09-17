CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_TESTS)

set(target cxx_api_tests)

set(tests
tests/base
tests/sbml_test_suite
tests/steady_state
tests/stoichiometric
tests/model_editing
tests/state_saving
tests/other_tests
)

add_executable( ${target} 
main
src/Args
src/rrTestUtils
${tests}
)

set_property(TARGET ${target}  
             PROPERTY  COMPILE_DEFINITIONS 
LIBSBML_USE_CPP_NAMESPACE
LIBSBML_STATIC
STATIC_LIBSTRUCT
STATIC_PUGI
STATIC_RR
STATIC_NLEQ1
STATIC_NLEQ2
) 

link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)

include_directories(
src
${RR_ROOT}
${THIRD_PARTY_INSTALL_FOLDER}/include/
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
)
endif()

if(UNIX) #Don't know why static roadrunner fails....?
target_link_libraries (${target} 
roadrunner-static
libunit_test-static${staticLibPrefix}
lapack
blas
f2c
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


