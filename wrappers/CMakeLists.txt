CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrWrappers)

if(INSTALL_C_API)
add_subdirectory("C")
endif()

if(INSTALL_C_API_PYTHON)
add_subdirectory("c_api_python")
endif()

if(INSTALL_DELPHI)
add_subdirectory("Delphi")
endif()

if(BUILD_PYTHON)
    add_subdirectory("Python")
endif()

