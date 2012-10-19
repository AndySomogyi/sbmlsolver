CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(nleq)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------

OPTION(NLEQ_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(NLEQ_BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(target nleq) 

set(headers nleq1.h f2c_nleq.h)
set(sources 
nleq1.c
wnorm.c
zibconst.c
zibmon.c
zibsec.c
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
                       STATIC_NLEQ
             ) 


#libraries to link with
if(${MINGW})
else()
#target_link_libraries (${target} libf2c)
#target_link_libraries (${target}-static libf2c)
endif()


#Install required files
install(FILES ${headers} 
        DESTINATION include/nleq)

#install(TARGETS ${target} 
#        LIBRARY DESTINATION lib 
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin)
#
install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

