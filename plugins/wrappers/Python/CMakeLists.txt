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
        README.txt 
        DESTINATION plugins/python  	
        COMPONENT plugins_api
       )


if(WIN32)
    install(FILES install_roadrunner_plugins_win.py
        DESTINATION plugins    
        )
endif(WIN32)
