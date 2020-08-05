CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_C_API_APPS)

if (BUILD_TEST_TOOLS)
    add_subdirectory(sbml_ts_tester)
endif ()
