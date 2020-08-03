# Some Notes on Dependencies
# ----------------------------
# - When possible the preferred method of adding the dependency is by having it as a subpackage under the third party directory
# - Then, when possible, the preferred method of including the dependency in the rr project is to use `add_subdirectory`
#   to include the subdirectory targets into the rr project.
# - Sometimes this is not possible, either because like sbml it has its own dependencies or because like LLVM its too big to want
#   to include it as a subdirectory.
# - In the case of sbml, we use ExternalProject_Add to manage the dependency between libsbml and its dependencies.
# - LLVM is handled separately, either build from source automatically or, when appropriate, go and collect precompiled binaries

include(ExternalProject)

# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")


add_subdirectory(nleq1)
#
#ExternalProject_Add(
#        nleq1
#        SOURCE_DIR ${NLEQ1_SOURCE_DIR}
#        LOG_DIR ${NLEQ1_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${NLEQ1_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)

add_subdirectory(nleq2)
#ExternalProject_Add(
#        nleq2
#        SOURCE_DIR ${NLEQ2_SOURCE_DIR}
#        LOG_DIR ${NLEQ2_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${NLEQ2_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)
#
## clapack3.2.1
add_subdirectory(clapack3.2.1)
#ExternalProject_Add(
#        clapack
#        SOURCE_DIR ${CLAPACK_SOURCE_DIR}
#        LOG_DIR ${CLAPACK_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${CLAPACK_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)
#
## libsbml dependencies
#add_subdirectory(libSBML-dependencies)

#set(LIBSBML_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
#set(LIBSBML_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
#set(BZIP2_SOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/bzip2)
#set(CHECK_SOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/check)
#set(ICONV_SOURCE_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/libiconv)
#set(LIBXML2_SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/libxml2)
#set(ZLIB_SOURCE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/zlib)

set(LIBSBML_DEPS_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-dependencies")
set(LIBSBML_DEPS_INSTALL_PREFIX "${LIBSBML_DEPS_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
set(HAVE_LIBSBML_DEPS OFF)
find_library(LIBICONV_STATIC_LIBRARY
        NAMES
        libiconv${CMAKE_STATIC_LIBRARY_SUFFIX}
        iconv${CMAKE_STATIC_LIBRARY_SUFFIX}
        PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
        NO_DEFAULT_PATH
        )

find_library(LIBXML2_STATIC_LIBRARY
        NAMES
        libxml2${CMAKE_STATIC_LIBRARY_SUFFIX}
        xml2${CMAKE_STATIC_LIBRARY_SUFFIX}
        PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
        NO_DEFAULT_PATH
        )

find_library(ZLIB_STATIC_LIBRARY
        NAMES
        zdll${CMAKE_STATIC_LIBRARY_SUFFIX}
        libz${CMAKE_STATIC_LIBRARY_SUFFIX}
        libzlib${CMAKE_STATIC_LIBRARY_SUFFIX}
        PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
        NO_DEFAULT_PATH
        )
set(LIBSBML_DEPENDENCIES_LIBRARIES
        ${LIBXML2_STATIC_LIBRARY}
        ${LIBICONV_STATIC_LIBRARY}
        ${ZLIB_STATIC_LIBRARY}
        )
foreach (lib ${LIBSBML_DEPENDENCIES_LIBRARIES})
    if (NOT EXISTS ${lib})
        set(HAVE_LIBSBML_DEPENDENCIES OFF)
        break()
    endif ()
endforeach ()

if (NOT ${HAVE_LIBSBML_DEPS})
    project(build-libsbml-dependencies)
    ExternalProject_Add(
            libsbml_dependencies
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies
            LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/logs-${CMAKE_CXX_COMPILER_ID}
            CMAKE_ARGS
            -DCMAKE_INSTALL_PREFIX=${LIBSBML_DEPS_INSTALL_PREFIX}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            #        -DWITH_STATIC_RUNTIME=ON
    )

endif ()
#
#
## libsbml - looks for its dependencies in a folder called `dependencies` at libsbml root.
#set(LIBSBML_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source)
#message(STATUS "1: LIBSBML_ROOT_SOURCE_DIR ${LIBSBML_ROOT_SOURCE_DIR}")
#set(LIBSBML_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source")
#CheckDirectoryExists("libsbml source" "${LIBSBML_SOURCE_DIR}")
#
#set(LIBSBML_INSTALL_PREFIX "${LIBSBML_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
#
#find_library(LIBSBML_STATIC_LIBRARY
#        NAMES
#        libsbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
#        sbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
#        HINTS ${LIBSBML_INSTALL_PREFIX}/lib
#        NO_DEFAULT_PATH
#        )
#
#find_path(LIBSBML_INCLUDE_DIR
#        NAMES sbml/SBMLTypes.h
#        HINTS ${LIBSBML_INSTALL_PREFIX}/include
#        NO_DEFAULT_PATH
#        )
# Adding sbml as a subdirectory causes ALL sorts of problems. The sbml
# build sript was not intended to be used this way and therefore it is
# hard to do. Preference would be to add_subdirectory but instead we
# add as external project:
#add_subdirectory(libSBML-5.18.1-experimental-Source)

set(HAVE_LIBSBML OFF)


#ExternalProject_Add(
#        libsbml
#        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source
#        LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DLIBSBML_DEPENDENCY_DIR=${LIBSBML_DEPS_INSTALL_PREFIX}
#        -DCMAKE_INSTALL_PREFIX=${LIBSBML_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DWITH_CPP_NAMESPACE=ON
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#        #        -DWITH_STATIC_RUNTIME=ON # causes problems.
#        DEPENDS libsbml_dependencies
#)


# add rr-libstruct subdirectory. Depends on libsbml
#add_subdirectory(rr-libstruct)
#add_subdirectory(rr-libstruct)
#ExternalProject_Add(
#        rr-libstruct
#        SOURCE_DIR ${RR_LIBSTRUCT_DIR}
#        LOG_DIR ${RR_LIBSTRUCT_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${RR_LIBSTRUCT_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        # These should default to the correct location inside rr-libstruct anyway
#        -DLIBSBML_INSTALL_PREFIX=${LIBSBML_INSTALL_PREFIX}
#        -DCLAPACK_INSTALL_PREFIX=${CLAPACK_INSTALL_PREFIX}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)
#add_dependencies(rr-libstruct libsbml clapack)
#
## poco
#add_subdirectory(poco_1.5.3)
#ExternalProject_Add(
#        poco
#        SOURCE_DIR ${POCO_SOURCE_DIR}
#        LOG_DIR ${POCO_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${POCO_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#        -DFoundation_EXPORTS=ON
#
#)
#
## sundials
#add_subdirectory(sundials)
#ExternalProject_Add(
#        sundials
#        SOURCE_DIR ${SUNDIALS_SOURCE_DIR}
#        LOG_DIR ${SUNDIALS_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${SUNDIALS_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)
#
## unit_test
#add_subdirectory(unit_test)
#ExternalProject_Add(
#        unittest
#        SOURCE_DIR ${UNIT_TEST_SOURCE_DIR}
#        LOG_DIR ${UNIT_TEST_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
#        CMAKE_ARGS
#        -DCMAKE_INSTALL_PREFIX=${UNIT_TEST_INSTALL_PREFIX}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
#)
#
#add_custom_target(all-dependencies)
#
#add_dependencies(all-dependencies
#        libsbml
#        poco
#        sundials
#        nleq1
#        nleq2
#        clapack
#        libsbml_dependencies
#        libsbml
#        poco
#        sundials
#        rr-libstruct
#        unittest
#        )
#



















