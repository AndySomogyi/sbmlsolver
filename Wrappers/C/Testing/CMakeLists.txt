CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_Tests)

set(target c_api_tests)

set(tests
#core/base
#core/steady_state
Suite_TestModel_1
#Suite_CoreExceptions
Suite_SBML_Tests
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
) 

if(UNIX)
set(staticLibPrefix ".a")
set(sharedLibPrefix ".so")
#add_definitions(-DUNITTEST_POSIX)
endif()

target_link_libraries (${target} 
rrc_api
#roadrunner-static
unit_test-static
#rr-libstruct-static
)

if(UNIX)
target_link_libraries (${target} 
#unit_test-static${staticLibPrefix}
#libunit_test-static${staticLibPrefix}
#nleq-static.a
#rr-libstruct-static
lapack
blas
f2c
#libsbml-static.a
#libxml2.so
#sundials_nvecserial.a
#sundials_cvode.a
pthread
#poco_foundation-static.a
#pugi-static.a
#dl
)
endif()

link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)

include_directories(
"."
${RR_ROOT}
${THIRD_PARTY_FOLDER}/unit_test/src
${THIRD_PARTY_FOLDER}/rr-libstruct
)

add_subdirectory(utils)

install (TARGETS ${target}  		
DESTINATION bin COMPONENT testing
)




