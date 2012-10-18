CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_Tests)

set(target cxx_api_tests)
set(tests
core/base
)

add_executable( ${target} 
main.cpp
Args.cpp
${tests}
)

set_property(TARGET ${target}  
             PROPERTY  COMPILE_DEFINITIONS 
                       WIN32
                       STATIC_LIBSTRUCT
                       LIBSBML_USE_CPP_NAMESPACE
                       LIBSBML_STATIC
             ) 


target_link_libraries (${target} 
roadrunner
rr-libstruct-static
unit_test-static
)

link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)


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

set(sbmlModelsDir "${CMAKE_SOURCE_DIR}/models")

#-mr:\rrl\models -rr:\rrl\wiki\reports\file -tr:\rrTemp
add_custom_command(OUTPUT ${resultsFile}
  COMMAND ${target} -m${sbmlModelsDir} -r${resultsFile}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating CPP Wiki reports"
  VERBATIM)

add_custom_target(cppTestingReport ALL DEPENDS ${resultsFile})
   
install(
        TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT testing)


