CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(lmfit)

set(target lmfit) 
set(headers lib/lmcurve.h lib/lmmin.h)

set(sources 
lib/lmmin.c
lib/lmcurve.c
)

add_library(${target}-static STATIC ${sources})

if(WIN32)
set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
                       WIN32
                       STATIC_LMFIT
             ) 
else()
set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
                       STATIC_LMFIT
             ) 

endif()


#Install required files
install(FILES ${headers} 
        DESTINATION include/lmfit)

install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib
)

