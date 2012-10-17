CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rr-libstruct)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(LIBSTRUCT_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(LIBSTRUCT_BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(target rr-libstruct) 

set(sources 
lsMatrix.cpp
lsComplex.cpp
lsLibStructural.cpp
lsSBMLModel.cpp
lsUtils.cpp
lsLibla.cpp
lsLA.cpp
)


include_directories(
.
${THIRD_PARTY_FOLDER}/clapack/3.2.1/INCLUDE
${THIRD_PARTY_FOLDER}/libsbml/src
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
                       STATIC_LIBSTRUCT
                       LIBSBML_USE_CPP_NAMESPACE
                       LIBSBML_STATIC
             ) 


#libraries to link with
if(${MINGW})
else()
#target_link_libraries (${target} libf2c)
#target_link_libraries (${target}-static libf2c)
endif()

set(headers 
lsExporter.h
lsMatrix.h
lsComplex.h
lsLibstructural.h
lsLA.h
lsLibla.h
)

#Install required files
install(FILES ${headers} 
        DESTINATION include/rr-libstruct)

#install(TARGETS ${target} 
#        LIBRARY DESTINATION lib 
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin)
#
install(TARGETS ${target}-static 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib)

