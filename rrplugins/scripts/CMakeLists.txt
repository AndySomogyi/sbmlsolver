CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(plugins_wrappers)

#=============== INSTALL =====================
if(WIN32)
    install(
    FILES install_tellurium_plugins.py
    DESTINATION .
    COMPONENT tel_plugins
        )
endif(WIN32)
