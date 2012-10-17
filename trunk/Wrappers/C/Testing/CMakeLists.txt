CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_Tests)

set(target c_api_tests)
set(tests
core/base
)
add_executable( ${target} 
main.cpp
${tests}
)

target_link_libraries (${target} 
rr_c_api
unit_test-static
roadrunner
)

link_directories(
${THIRD_PARTY_INSTALL_FOLDER}/lib
)


include_directories(
${RR_ROOT}
${THIRD_PARTY_FOLDER}/unit_test/src
)

set( resultFileFolder "${CMAKE_SOURCE_DIR}/wiki/reports")
set( resultFile "${resultFileFolder}/c_api_tests.xml")

add_custom_command(OUTPUT ${resultFile}
  COMMAND ${target} ${resultFileFolder}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating Wiki reports"
  VERBATIM)

add_custom_target(CTestingReport ALL DEPENDS ${resultFile})
   
install(
        TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT testing)


