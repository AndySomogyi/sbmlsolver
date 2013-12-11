CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(plugins_wrappers)

#if(BUILD_TESTS)
#add_subdirectory(Testing)
#endif()

if(INSTALL_EXAMPLES)
add_subdirectory(Examples)
endif()
#=============== INSTALL =====================
install(
        FILES 
        rrPlugins.py 
        rrPlugins.txt 
        DESTINATION plugins/python  	
        COMPONENT plugins_api
       )

