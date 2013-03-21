CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrWrappers)

#=============== INSTALL =====================
install(
        FILES 
        rrRunTests.py
        rrTester.py
        rrDebug.py 
        DESTINATION python  	
        COMPONENT testing
       )

