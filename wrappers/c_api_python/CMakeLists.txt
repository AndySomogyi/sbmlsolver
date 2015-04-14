CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrWrappers)

if(BUILD_TEST_TOOLS)
add_subdirectory(Testing)

if(INSTALL_EXAMPLES)
add_subdirectory(Examples)
endif()

#=============== INSTALL =====================
install(
    FILES 
    rrPython.py README.txt
    DESTINATION c_api_python  	
    COMPONENT testing
    )
endif(BUILD_TEST_TOOLS)
