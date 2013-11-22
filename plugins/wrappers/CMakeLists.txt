CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(plugins_wrappers)

OPTION (BUILD_PLUGINS_C_API	                        "Build Plugins C API"	            ON)
OPTION (INSTALL_PLUGINS_PYTHON_API	                "Install Plugins Python API"	            ON)

if(${BUILD_PLUGINS_C_API} STREQUAL ON)
add_subdirectory("C")
endif()

if(${INSTALL_PLUGINS_PYTHON_API} STREQUAL ON)
add_subdirectory("Python")
endif()


