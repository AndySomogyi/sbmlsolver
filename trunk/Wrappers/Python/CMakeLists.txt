CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(rrPythonWrapper)


#We will assume that the C DLL has been built
if(${MSVC})
if(CMAKE_BUILD_TYPE MATCHES "Debug")
set(C_DLL "${CMAKE_CURRENT_BINARY_DIR}/../../../../all/trunk/bin/Debug/rr_c_api.dll")
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
set(C_DLL "${CMAKE_CURRENT_BINARY_DIR}/../../../../all/trunk/bin/Release/rr_c_api.dll")
endif()
else()
set(C_DLL "${CMAKE_CURRENT_BINARY_DIR}/../../../../all/trunk/bin/rr_c_api.dll")
endif()


add_subdirectory(Apps)
add_subdirectory(Examples)
#add_subdirectory(Testing)

#=============== INSTALL =====================
install( 
         FILES ${C_DLL} 
         DESTINATION bin
         COMPONENT Python
        )

install(
        FILES 
        rrPython.py 
        Readme.txt
        DESTINATION Python  	
        COMPONENT Python
       )

