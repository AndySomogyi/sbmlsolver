CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_Tests)

set(target cxx_api_tests)
set(tests
core/base
)

add_executable( ${target} 
main.cpp
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

set( resultFileFolder "${CMAKE_SOURCE_DIR}/wiki/reports")
set( resultFile "${resultFileFolder}/cpp_api_tests.xml")

add_custom_command(OUTPUT ${resultFile}
  COMMAND ${target} ${resultFileFolder}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMENT "Generating Wiki reports"
  VERBATIM)

add_custom_target(cxxTestingReport ALL DEPENDS ${resultFile})
   
install(
        TARGETS ${target} 		
        DESTINATION bin 
        COMPONENT testing)


