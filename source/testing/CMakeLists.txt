CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_Tests)

set(target cxx_api_tests)
set(tests
core/base
core/sbmlTests
)

add_executable( ${target} 
main.cpp
Args.cpp
${tests}
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

if(UNIX)
set(staticLibPrefix ".a")
set(sharedLibPrefix ".so")
add_definitions(-DUNITTEST_POSIX)
else()
set(staticLibPrefix "")
set(sharedLibPrefix "")
endif()

target_link_libraries (${target} 
roadrunner-static
unit_test-static${staticLibPrefix}
libf2c
)

if(UNIX)
target_link_libraries (${target} 
libunit_test-static${staticLibPrefix}
nleq-static.a
rr-libstruct-static
lapack
blas
f2c
libsbml-static.a
libxml2.so
sundials_nvecserial.a
sundials_cvode.a
pthread
poco_foundation-static.a
pugi-static.a
dl
)
endif()


link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)

include_directories(
${RR_ROOT}
${THIRD_PARTY_FOLDER}/unit_test/src
)

set(resultsDir "${CMAKE_SOURCE_DIR}/reports")

if(${BORLAND})
set(resultsFile "${resultsDir}/cpp_api_tests-cg_xe1.xml")
elseif(${MSVC})
set(resultsFile "${resultsDir}/cpp_api_tests-vs_2010.xml")
elseif(CMAKE_COMPILER_IS_GNUCC)
set(resultsFile "${resultsDir}/cpp_api_tests-gcc.xml")
else()
set(resultsFile "${resultsDir}/cpp_api_tests_unknown_compiler.xml")
endif()

set(sbmlModelsDir       "${CMAKE_SOURCE_DIR}/models")
set(compilerFolder      "${CMAKE_SOURCE_DIR}/ThirdParty/compilers/tcc")
set(supportCodeFolder   "/r/rrl/rr_support")

if(UNIX)
set(compilerFolder     "")
endif()

Message("SupportCodeFolder is: " ${supportCodeFolder})
# Call like this -mr:\rrl\models -rr:\rrl\wiki\reports\file -tr:\rrTemp -sSupportCodeFolder -lCompilerFolder
#add_custom_command(
#    OUTPUT ${resultsFile}
#    COMMAND ${target} -m${sbmlModelsDir} -l${compilerFolder} -s${supportCodeFolder} -r${resultsFile} -v
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#    COMMENT "=========== Generating CPP-API Wiki reports =============="
#    VERBATIM 
#    )
#
#add_custom_target(cppTestingReport ALL DEPENDS ${resulsFile}
#    COMMAND ${target} -m${sbmlModelsDir} -l${compilerFolder} -s"${supportCodeFolder}" -r${resultsFile} -v 
#    COMMENT "=========== Generating CPP-API Wiki reports =============="
#    COMMENT "Executing ${target} -m${sbmlModelsDir} -l${compilerFolder} -s${supportCodeFolder} -r${resultsFile} -v"
#
#)

install (TARGETS ${target}  		
DESTINATION bin 
COMPONENT testing
)

if(BORLAND)
install(
FILES run_tests_cg.bat 
DESTINATION bin
COMPONENT testing
)
else()
install(
FILES run_tests_vs.bat 
DESTINATION bin
COMPONENT testing
)
endif()


