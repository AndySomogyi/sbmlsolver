CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rr-libstruct)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(LIBSTRUCT_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(LIBSTRUCT_BUILD_SHARED_LIBS "Build shared libraries" OFF)
set(target rr-libstruct) 

set(sources 
lsMatrix
lsLibStructural
lsSBMLModel
lsUtils
lsLibla
lsLA
lsLUResult
)


include_directories(
.
${THIRD_PARTY_DIRECTORY}/clapack3.2.1/include
${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source/src
)

#Then the static version
add_library(${target}-static STATIC ${sources})

set(static_compile_definitions
STATIC_LIBSTRUCT
LIBSBML_USE_CPP_NAMESPACE
LIBSBML_STATIC
)

if(WIN32)
set(static_compile_definitions 
${static_compile_definitions}
WIN32
) 
endif()



target_include_directories(${target}-static PRIVATE
#        ${LIBSBML_INCLUDE_DIR}
        ${CLAPACK_INCLUDE_DIR}
        /mnt/d/rrbuild/roadrunner/third_party/libSBML-5.18.1-experimental-Source/install-GNU/include
        )

target_link_libraries(${target}-static PRIVATE ${LIBSBML_STATIC_LIBRARY})
add_dependencies(${target}-static libsbml lapack)


set_property(TARGET ${target}-static  
             PROPERTY  COMPILE_DEFINITIONS 
             ${static_compile_definitions}
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
lsLibStructural.h
lsLA.h
lsLibla.h
lsLUResult.h
lsUtils.h
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

