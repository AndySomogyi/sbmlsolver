#include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIGIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

find_package(PythonInterp)
# specify that the same python library should be found 
# as the one the selected interpreter uses
set(Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION_STRING})

find_package (Python COMPONENTS Interpreter Development NumPy REQUIRED)
if (NOT ${Python_FOUND})
    message(FATAL_ERROR "Could not find Python")
endif()

message(STATUS "Using Python Version ${Python_VERSION}")

set(PYINTERFACE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pyinterface)
set(CMAKE_SWIG_FLAGS "")

if (SWIG_FOUND)
    add_subdirectory(roadrunner)
else (SWIG_FOUND)
    message("could not find SWIG in your system")
endif (SWIG_FOUND)

if (SWIG_VERSION STRGREATER "4.0")
    message(SEND_ERROR "SWIG version too high:  unable to use SWIG version 4.0 and higher.")
endif (SWIG_VERSION STRGREATER "4.0")

message(STATUS "RR_ROOT ${RR_ROOT}")
install(FILES INSTALL.txt setup.py MANIFEST.in "${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")

if (WITH_CONDA_BUILDER)
    configure_file(conda/meta.yaml.in conda/meta.yaml @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/conda/meta.yaml conda/build.sh conda/bld.bat DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}")
endif ()
