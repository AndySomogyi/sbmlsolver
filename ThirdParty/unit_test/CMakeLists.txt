CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(unit_test)

set(target unit_test) 

FILE (GLOB headers 
    src/*.h 
    )

FILE (GLOB win32_headers 
    src/Win32/*.h 
    )
set(sources 
src/AssertException
src/Checks
src/CurrentTest
src/DeferredTestReporter
src/DeferredTestResult
src/MemoryOutStream
src/ReportAssert
src/Test
src/TestDetails
src/TestList
src/TestReporter
src/TestReporterStdout
src/TestResults
src/TestRunner
src/TimeConstraint
src/XmlTestReporter
src/Win32/TimeHelpers
)

#This library can only be built as static
add_library(${target}-static STATIC ${sources})
set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
                       WIN32
                       STATIC_UNIT_TEST
             ) 

#Install required files
install(FILES ${headers} 
        DESTINATION include/unit_test)

install(FILES ${win32_headers} 
        DESTINATION include/unit_test/Win32)

install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

