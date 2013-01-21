CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrWrappers)

#=============== INSTALL =====================
install(
        FILES 
        ssTests.py 
        rrTester.py
        testModel.py
        rrDebug.py 
        DESTINATION python  	
        COMPONENT testing
       )

