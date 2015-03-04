CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrWrappers)

if(INSTALL_C_API)
add_subdirectory("C")
endif()

if(BUILD_PYTHON)
    add_subdirectory("Python")
endif()

