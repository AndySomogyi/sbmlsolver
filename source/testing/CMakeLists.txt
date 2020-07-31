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
        )

add_executable(${target}
        main
        src/Args
        src/rrTestUtils
        ${tests}
        )

#set(CMAKE_CXX_FLAGS "-fno-rtti ${CMAKE_CXX_FLAGS}")

set_property(TARGET ${target}
        PROPERTY COMPILE_DEFINITIONS
        LIBSBML_USE_CPP_NAMESPACE
        LIBSBML_STATIC
        STATIC_LIBSTRUCT
        STATIC_PUGI
        STATIC_RR
        STATIC_NLEQ1
        STATIC_NLEQ2
        )


target_include_directories(${target} PRIVATE
        src
        ${UNIT_TEST_INCLUDE_DIR}
        ${INCLUDE_DIRECTORIES}
        )


target_link_libraries(${target} PRIVATE
        $<TARGET_FILE:roadrunner-static>
        ${UNIT_TEST_STATIC_LIBRARY}
        ${LINK_LIBRARIES}
        "/mnt/d/LLVM6.0.1/install-gcc7/lib/libLLVMSupport.a"
        )
add_dependencies(${target} unittest roadrunner-static)
if (UNIX)
    target_compile_options(${target} PRIVATE -frtti)
endif ()
#set(resultsDir "${CMAKE_SOURCE_DIR}/reports")
#
#if(${BORLAND})
#set(resultsFile "${resultsDir}/cpp_api_tests-cg_xe1.xml")
#elseif(${MSVC})
#set(resultsFile "${resultsDir}/cpp_api_tests-vs_2010.xml")
#elseif(CMAKE_COMPILER_IS_GNUCC)
#set(resultsFile "${resultsDir}/cpp_api_tests-gcc.xml")
#else()
#set(resultsFile "${resultsDir}/cpp_api_tests_unknown_compiler.xml")
#endif()
#
#set(sbmlModelsDir       "${CMAKE_SOURCE_DIR}/models")
#set(compilerFolder      "${CMAKE_SOURCE_DIR}/third_party/compilers/tcc")
#set(supportCodeFolder   "/r/rrl/rr_support")
#
#if(UNIX)
#set(compilerFolder     "")
#endif()
#
#Message("SupportCodeFolder is: " ${supportCodeFolder})
# Call like this -mr:/rrl/models -rr:/rrl/wiki/reports/file -tr:/rrTemp -sSupportCodeFolder -lCompilerFolder
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

install(TARGETS ${target}
        DESTINATION bin
        COMPONENT testing
        )

install(
        FILES run_cpp_tests.bat
        DESTINATION bin
        COMPONENT testing
)


