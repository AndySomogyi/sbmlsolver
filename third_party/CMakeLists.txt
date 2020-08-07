# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")

## nleq1
message(STATUS "Adding nleq1 as subdirectory")
add_subdirectory(nleq1)
set(NLEQ1_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/nleq1/include")
set(NLEQ1_INCLUDE_DIR "${NLEQ1_INCLUDE_DIR}" PARENT_SCOPE)

## nleq2
message(STATUS "Adding nleq2 as subdirectory")
add_subdirectory(nleq2)
set(NLEQ2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/nleq2/include")
set(NLEQ2_INCLUDE_DIR "${NLEQ2_INCLUDE_DIR}" PARENT_SCOPE)

## clapack3.2.1
message(STATUS "Adding clapack3.2.1 as subdirectory")
add_subdirectory(clapack3.2.1)
set(CLAPACK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/clapack3.2.1/include")
set(CLAPACK_INCLUDE_DIR "${CLAPACK_INCLUDE_DIR}" PARENT_SCOPE)
#
### libsbml dependencies
##message(STATUS "Adding libSBML-dependencies as subdirectory")
##add_subdirectory(libSBML-dependencies)
##set(LIBSBML_DEPS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/include")
##set(LIBSBML_DEPS_INCLUDE_DIR "${LIBSBML_DEPS_INCLUDE_DIR}")
#


# zlib
message(STATUS "Adding zlib as subdirectory")
#if (UNIX)
if (WIN32)
    # on windows zconf.h tries to import unistd.h
    # but we can bypass this with the Z_SOLO flag.
    # We add this globally so all targets on windows
    # are affected by this flag.
#    add_definitions(-DZ_SOLO -D_LARGEFILE64_SOURCE)
endif()
add_subdirectory(zlib-1.2.11)
set(ZLIB_INCLUDE_DIR "${ZLIB_INCLUDE_DIR}" PARENT_SCOPE)

# lzma
message(STATUS "Adding lzma as subdirectory")
add_subdirectory(lzma)
set(LZMA_INCLUDE_DIR "${LZMA_INCLUDE_DIR}" PARENT_SCOPE)

# iconv
message(STATUS "Adding iconv as subdirectory")
add_subdirectory(iconv)
set(ICONV_INCLUDE_DIR "${ICONV_INCLUDE_DIR}" PARENT_SCOPE)
message(STATUS "ICONV_INCLUDE_DIR ${ICONV_INCLUDE_DIR}")

# libxml2
message(STATUS "Adding libxml2 as subdirectory")
add_subdirectory(libxml2)
set(LIBXML2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libxml2")
set(LIBXML2_INCLUDE_DIR "${LIBXML2_INCLUDE_DIR}" PARENT_SCOPE)


# set the variables needed by libsbml here:
message(STATUS "Adding libSBML as subdirectory")
set(LIBSBML_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source)
set(LIBSBML_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libSBML-5.18.1-experimental-Source)
set(LIBXML_LIBRARY "$<TARGET_FILE:libxml2>")
set(LIBZ_LIBRARY "$<TARGET_FILE:zdll>")
set(WITH_CPP_NAMESPACE ON)
set(LIBSBML_SKIP_SHARED_LIBRARY ON)
set(WITH_CHECK OFF)


add_subdirectory(libSBML-5.18.1-experimental-Source)
set(LIBSBML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source/include")
set(LIBSBML_INCLUDE_DIR "${LIBSBML_INCLUDE_DIR}" PARENT_SCOPE)

message(STATUS "Adding rr-libstruct as subdirectory")
add_subdirectory(rr-libstruct)
#set(RR_STRUCT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/rr-libstruct")
set(RR_STRUCT_INCLUDE_DIR "${RR_STRUCT_INCLUDE_DIR}" PARENT_SCOPE)

#include_directories(rr-libstruct)
## poco
message(STATUS "Adding poco as subdirectory")
set(POCO_STATIC TRUE)
add_subdirectory(poco_1.5.3)
set(POCO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/poco_1.5.3/include")
set(POCO_INCLUDE_DIR "${POCO_INCLUDE_DIR}" PARENT_SCOPE)
#include_directories(poco_1.5.3/include)
#include_directories(D:/rrbuild/roadrunner/third_party/poco_1.5.3/include)

## sundials
message(STATUS "Adding sundials as subdirectory")
set(SUNDIALS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sundials")
set(SUNDIALS_SOURCE_DIR "${SUNDIALS_SOURCE_DIR}" PARENT_SCOPE)

add_subdirectory(sundials)
set(SUNDIALS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sundials/include")
set(SUNDIALS_INCLUDE_DIR "${SUNDIALS_INCLUDE_DIR}" PARENT_SCOPE)

message(STATUS "CVODE BUILD_SHARED_LIBS after : ${BUILD_SHARED_LIBS}")
#include_directories(sundials/include)

## unit_test
message(STATUS "Adding unit-test as subdirectory")
add_subdirectory(unit_test)
set(UNITTEST_INCLUDE_DIR "${UNITTEST_INCLUDE_DIR}" PARENT_SCOPE)
#
#
## googletest












