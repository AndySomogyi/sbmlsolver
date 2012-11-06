CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_Tests)

set(target cxx_api_tests)
set(tests
core/base
#core/sbmlTests
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
PUGI_STATIC
RR_STATIC
) 


target_link_libraries (${target} 
unit_test-static
roadrunner-static
)

link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)
if(UNIX)

link_directories(
/usr/local/lib
)
endif()

include_directories(
${RR_ROOT}
${THIRD_PARTY_FOLDER}/unit_test/src
)

set(resultsDir "${CMAKE_SOURCE_DIR}/wiki/reports")

if(${BORLAND})
set(resultsFile "${resultsDir}/cpp_api_tests_cg.xml")
elseif(${MSVC})
set(resultsFile "${resultsDir}/cpp_api_tests_msvc.xml")
else()
set(resultsFile "${resultsDir}/cpp_api_tests_unknown_compiler.xml")
endif()

set(sbmlModelsDir       "${CMAKE_SOURCE_DIR}/models")
set(compilerFolder      "${CMAKE_SOURCE_DIR}/trunk/ThirdParty/compilers/tcc")
set(supportCodeFolder   "${CMAKE_SOURCE_DIR}/trunk/rr_support")

Message("SBML model dir is: " ${sbmlModelsDir})
# Call like this -mr:\rrl\models -rr:\rrl\wiki\reports\file -tr:\rrTemp -sSupportCodeFolder -lCompilerFolder
add_custom_command(OUTPUT ${resultsFile}
  COMMAND ${target} -m${sbmlModelsDir} -l${compilerFolder} -s${supportCodeFolder} -r${resultsFile}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "=========== Generating CPP-API Wiki reports =============="
  VERBATIM)

add_custom_target(cppTestingReport ALL DEPENDS ${resultsFile})
   
install(
        TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT testing)


