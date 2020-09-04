# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")


#todo look into this cmake function. Looks useful
#check_function_exists(fseeko HAVE_FSEEKO)


## nleq1
message(STATUS "Adding nleq1 as subdirectory")
add_subdirectory(nleq1)
set(LIBBZ_INCLUDE_DIR $<TARGET_PROPERTY:nleq1,INCLUDE_DIRECTORIES>) # or libsbml


## nleq2
message(STATUS "Adding nleq2 as subdirectory")
add_subdirectory(nleq2)
set(LIBBZ_INCLUDE_DIR $<TARGET_PROPERTY:nleq2,INCLUDE_DIRECTORIES>)

## bzip
message(STATUS "Adding bzip2 as subdirectory")
add_subdirectory(bzip2)
set(LIBBZ_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bzip2")

## zlib
message(STATUS "Adding zlib as subdirectory")
add_subdirectory(zlib)
set(ZLIB_INCLUDE_DIR $<TARGET_PROPERTY:zlib,INCLUDE_DIRECTORIES>)
set(LIBZ_INCLUDE_DIR "${ZLIB_INCLUDE_DIR}") # an alias, used by libsbml

## iconv
message(STATUS "Adding iconv as subdirectory")
add_subdirectory(iconv)
set(LIBICONV_INCLUDE_DIR ${LIBICONV_INCLUDE_DIR} $<TARGET_PROPERTY:libiconv-static,INCLUDE_DIRECTORIES>)

## clapack
message(STATUS "Adding clapack as subdirectory")
add_subdirectory(CLAPACK)
set(CLAPACK_INCLUDE_DIR $<TARGET_PROPERTY:lapack,INCLUDE_DIRECTORIES>)


## libxml2
message(STATUS "Adding libxml2 as subdirectory")
add_subdirectory(libxml2)
add_dependencies(LibXml2 zlib libiconv-static)

set(
        LIBXML_INCLUDE_DIR ${LIBXML_INCLUDE_DIR}
        $<TARGET_PROPERTY:LibXml2,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libz2,INCLUDE_DIRECTORIES>
        "${CMAKE_CURRENT_SOURCE_DIR}/libxml2/include"
        "${CMAKE_CURRENT_BINARY_DIR}/libxml2"
)

# note the TARGET_LINKER_FILE generator expression gives us the export library on windows
set(LIBXML_LIBRARY $<TARGET_LINKER_FILE:LibXml2>)

## poco
message(STATUS "Adding poco as subdirectory")
set(ENABLE_APACHECONNECTOR OFF)
set(ENABLE_CPPPARSER OFF)
set(ENABLE_CRYPTO OFF)
set(ENABLE_DATA OFF)
set(ENABLE_DATA_MYSQL OFF)
set(ENABLE_DATA_ODBC OFF)
set(ENABLE_DATA_POSTGRESQL OFF)
set(ENABLE_DATA_SQLITE OFF)
set(ENABLE_ENCODINGS OFF)
set(ENABLE_ENCODINGS_COMPILER OFF)
set(ENABLE_FOUNDATION ON)
set(ENABLE_JSON OFF)
set(ENABLE_JWT OFF)
set(ENABLE_MONGODB OFF)
set(ENABLE_PDF OFF)
set(ENABLE_NET ON)
set(ENABLE_NETSSL OFF)
set(ENABLE_NETSSL_WIN OFF)
set(ENABLE_UTIL OFF)
set(ENABLE_XML OFF)
set(ENABLE_ZIP OFF)
set(ENABLE_PAGECOMPILER OFF)
set(ENABLE_PAGECOMPILER_FILE2PAGE OFF)
set(ENABLE_REDIS OFF)
set(ENABLE_POCODOC OFF)
set(ENABLE_TESTS OFF)
add_subdirectory(poco)

## libsbml
message(STATUS "Adding libsbml as subdirectory")
add_subdirectory(libSBML-5.18.1-experimental-Source)
set(LIBSBML_INCLUDE_DIR $<TARGET_PROPERTY:libsbml,INCLUDE_DIRECTORIES>) # used by libstruct

# rr-libstruct
message(STATUS "Adding rr-libstruct as subdirectory")
# poco options
add_subdirectory(rr-libstruct)
set(ZLIB_INCLUDE_DIR $<TARGET_PROPERTY:rr-libstruct-static,INCLUDE_DIRECTORIES>)


## sundials
message(STATUS "Adding sundials as subdirectory")
if (${BUILD_SHARED_LIBS})
    # temporarily turn off build_shared_libs FOR SUNDIALS WE ONLY WANT STATIC
    set(BUILD_SHARED_LIBS OFF)
    set(_TEMP_NO_SHARED_LIBS ON) # marker of shared lib needs turning on
endif ()

add_subdirectory(sundials)
if (_TEMP_NO_SHARED_LIBS)
    # turn BUILD_SHARED_LIBS back on
    set(BUILD_SHARED_LIBS ON)
endif ()

#set(SUNDIALS_INCLUDE_DIR
#    "$<TARGET_PROPERTIES,")
#set(SUNDIALS_INCLUDE_DIR "${SUNDIALS_INCLUDE_DIR}" PARENT_SCOPE)
#
#message(STATUS "CVODE BUILD_SHARED_LIBS after : ${BUILD_SHARED_LIBS}")
##include_directories(sundials/include)

set(
        DEPENDENCY_TARGETS
        libz2
        f2c blas lapack
        libcharset libiconv-static
        LibXml2
        sbml-static libsbml
        nleq1-static
        nleq2-static
        PocoFoundation
        PocoNet
        zlibstatic

)

macro(get_target_include_directories target variable)
    get_target_property(_var ${target} INCLUDE_DIRECTORIES)
    set(variable "${variable}" "${_var}")
    unset(_var)
endmacro()

set(
        DEPENDENCY_INCLUDE_DIRS
        $<TARGET_PROPERTY:libz2,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:f2c,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:blas,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:lapack,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libcharset,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:LibXml2,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libsbml,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PocoFoundation,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PocoNet,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:zlibstatic,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:libiconv-static,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:sbml-static,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:nleq1-static,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:nleq2-static,INCLUDE_DIRECTORIES>
        PARENT_SCOPE
)

add_custom_target(
        printGenex
        COMMENT "Result of generator expression:"
        COMMAND ${CMAKE_COMMAND} -E echo "${DEPENDENCY_INCLUDE_DIRS}"
        VERBATIM
)

set(DEPENDENCY_TARGETS ${DEPENDENCY_TARGETS} PARENT_SCOPE)
if (${BUILD_TESTS})
    add_subdirectory(googletest)
endif ()





