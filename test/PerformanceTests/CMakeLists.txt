set(target RoadRunnerMapPerformanceTests)

add_executable(
        ${target}
        RoadRunnerMapPerformanceTests.cpp
        "${CMAKE_SOURCE_DIR}/test/sbml-test-suite/STS.h"
        "${CMAKE_SOURCE_DIR}/test/sbml-test-suite/STS.cpp"
)

target_link_libraries(${target} PRIVATE roadrunner-static)
target_include_directories(${target} PRIVATE "${SBMLTestSuiteRoot}")
target_compile_definitions(${target} PRIVATE
        "SBMLTestSuiteRoot=\"${SBMLTestSuiteRoot}\""
        "SBMLTestSuiteSemantic=\"${SBMLTestSuiteSemantic}\""
        "SBMLTestSuiteStochastic=\"${SBMLTestSuiteStochastic}\""
        )

set(test_targets ${test_targets} PARENT_SCOPE)
