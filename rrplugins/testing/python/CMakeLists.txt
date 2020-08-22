CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(python_testing)

#=============== INSTALL =====================

FILE (GLOB pyFiles *.py)
FILE (GLOB modelFiles *.xml)

install(
        FILES 
        ${pyFiles}
        ${modelFiles}
#        README.txt 
        DESTINATION testing/python  	
        COMPONENT telplugins
       )


