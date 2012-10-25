CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(poco_foundation_src)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(POCO_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(POCO_BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(target poco_foundation) 

set(poco_include_folder ${CMAKE_CURRENT_SOURCE_FOLDER}../include)
set(sources 
Thread
Runnable
Exception
Mutex
ErrorHandler
Bugcheck
ThreadLocal
Debugger
SharedLibrary
SharedLibrary_WIN32
SharedLibrary_WIN32U
Path
UnicodeConverter
UTF8Encoding
TextEncoding
TextIterator
Ascii
RWLock
AtomicCounter
RWLock
)

if(${BORLAND})
add_definitions(
             -w-8012                     #Comparing signed /unsigned
             -w-8057                     #Parameter never used
             -w-8004                     #'var' is assigned a value that is never used
             -w-8123                     #Path not found
             -w-8066                     #Unreachable code
             -w-8104                     #Local static with constructor...
             -w-8008                     #Condition is always false..

        )
endif()


if(${MSVC})
#set(....
endif()


include_directories(
${poco_include_folder}
)
#add_library(${target} SHARED ${sources})
#
#set_property(TARGET ${target}  
#             PROPERTY  COMPILE_DEFINITIONS 
#                       WIN32
#                       SHARED_NLEQ
#             ) 
#
#Then the static version
add_library(${target}-static STATIC ${sources})
set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
                       WIN32
                       STATIC_POCO_FOUNDATION
             ) 


#libraries to link with

FILE (GLOB headers ${poco_include_folder}/Poco/*.h)

#Install required files
install(FILES ${headers} 
        DESTINATION include/poco)

#install(TARGETS ${target} 
#        LIBRARY DESTINATION lib 
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin)
#
install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

