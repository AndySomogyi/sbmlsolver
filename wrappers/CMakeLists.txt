CMAKE_MINIMUM_REQUIRED(VERSION 3.15..3.18 )

set(RR_CAPI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/C)
if (INSTALL_C_API)
    add_subdirectory("C")
endif ()

if (BUILD_PYTHON)
    add_subdirectory("Python")
endif ()

#if (INSTALL_C_API_PYTHON)
#    add_subdirectory("c_api_python")
#endif ()

#if (INSTALL_DELPHI)
#    add_subdirectory("Delphi")
#endif ()
#
#
#if (BUILD_JAVA_INTERFACE)
#    add_subdirectory("Java")
#endif ()

