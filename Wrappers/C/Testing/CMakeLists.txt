CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_Tests)

set(target c_api_tests)

set(tests
core/base
core/steady_state
)

add_executable( ${target} 
main.cpp
Args.cpp
${tests}
)

include_directories(
${RR_ROOT}
${THIRD_PARTY_FOLDER}/unit_test/src
)

target_link_libraries (${target} 
rr_c_api
unit_test-static
roadrunner
)

link_directories( ${THIRD_PARTY_INSTALL_FOLDER}/lib)


set(resultsDir "${CMAKE_SOURCE_DIR}/wiki/reports")

if(${BORLAND})
set(resultsFile "${resultsDir}/c_api_tests_cg.xml")
elseif(${MSVC})
set(resultsFile "${resultsDir}/c_api_tests_msvc.xml")
else()
set(resultsFile "${resultsDir}/c_api_tests_unknown_compiler.xml")
endif()

set(sbmlModelsDir "${CMAKE_SOURCE_DIR}/models")
set(compilerFolder "${CMAKE_SOURCE_DIR}/trunk/compilers/tcc")
set(supportCodeFolder "${CMAKE_SOURCE_DIR}/trunk/rr_support")

Message("SBML model dir is: " ${sbmlModelsDir})
Message("Result file is: " ${resultsFile})
#-mr:\rrl\models -rr:\rrl\wiki\reports\file -tr:\rrTemp
add_custom_command(OUTPUT ${resultsFile}
  COMMAND ${target} -m${sbmlModelsDir} -l${compilerFolder} -s${supportCodeFolder} -r${resultsFile}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating C-API wiki reports"
  VERBATIM)

add_custom_target(CTestingReport ALL DEPENDS ${resultsFile})
   
install(
        TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT testing)


