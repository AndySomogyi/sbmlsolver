CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rr-libstruct)

# Option that allows users to build static and/or shared libraries
# ----------------------------------------------------------------
OPTION(LIBSTRUCT_BUILD_STATIC_LIBS "Build static libraries" ON)
OPTION(LIBSTRUCT_BUILD_SHARED_LIBS "Build shared libraries" OFF)

get_filename_component(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

#set(LIBSBML_INSTALL_PREFIX ${THIRD_PARTY_DIR}/libSBML-5.18.1-experimental-Source/install-${CMAKE_CXX_COMPILER_ID}
#        CACHE PATH "Path to the root of libsbml installation")
#set(CLAPACK_INSTALL_PREFIX ${THIRD_PARTY_DIR}/clapack3.2.1/install-${CMAKE_CXX_COMPILER_ID}
#        CACHE PATH "Path to the root of clapack installation")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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


#Then the static version
add_library(${target}-static STATIC ${sources})

set(static_compile_definitions
        STATIC_LIBSTRUCT
        LIBSBML_USE_CPP_NAMESPACE
        LIBSBML_STATIC
        )

if (WIN32)
    set(static_compile_definitions
            ${static_compile_definitions}
            WIN32
            )
endif ()

target_include_directories(${target}-static PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CLAPACK_INCLUDE_DIR}
        ${LIBSBML_INCLUDE_DIR}
        )

target_link_libraries(${target}-static PRIVATE ${LIBSBML_STATIC_LIBRARY})


set_property(TARGET ${target}-static
        PROPERTY COMPILE_DEFINITIONS
        ${static_compile_definitions}
        )

#libraries to link with
if (${MINGW})
else ()
    #target_link_libraries (${target} libf2c)
    #target_link_libraries (${target}-static libf2c)
endif ()

set(headers
        lsExporter.h
        lsMatrix.h
        lsLibStructural.h
        lsLA.h
        lsLibla.h
        lsLUResult.h
        lsUtils.h
        )

# install the headers at configure time so we can build rr without the full install configuration
set(RR_STRUCT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(MAKE_DIRECTORY ${RR_STRUCT_INCLUDE_DIR})
file(COPY ${headers} DESTINATION "${RR_STRUCT_INCLUDE_DIR}/rr-libstruct")
set(RR_STRUCT_INCLUDE_DIR ${RR_STRUCT_INCLUDE_DIR} PARENT_SCOPE)

if (WIN32)

endif ()

install(TARGETS ${target}-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#Install required files
#install(FILES ${headers}
#        DESTINATION include/rr-libstruct)

#install(TARGETS ${target}
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        RUNTIME DESTINATION bin)
#

