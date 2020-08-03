# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")

## nleq1
message(STATUS "Adding nleq1 as subdirectory")
add_subdirectory(nleq1)
include_directories(nleq1)

## nleq2
message(STATUS "Adding nleq2 as subdirectory")
add_subdirectory(nleq2)
include_directories(nleq2)

## clapack3.2.1
message(STATUS "Adding clapack3.2.1 as subdirectory")
add_subdirectory(clapack3.2.1)
include_directories(clapack3.2.1/include)

## libsbml dependencies
message(STATUS "Adding libSBML-dependencies as subdirectory")
add_subdirectory(libSBML-dependencies)
include_directories(libSBML-dependencies/libiconv/source/include)
include_directories(libSBML-dependencies/libxml2/include)
include_directories(libSBML-dependencies/zlib/source)

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
include_directories(libSBML-5.18.1-experimental-Source/src)
include_directories(libSBML-5.18.1-experimental-Source/src/sbml)

message(STATUS "Adding rr-libstruct as subdirectory")
add_subdirectory(rr-libstruct)
include_directories(rr-libstruct)
## poco
message(STATUS "Adding poco as subdirectory")
add_subdirectory(poco_1.5.3)
include_directories(poco_1.5.3/Foundation/include)
include_directories(poco_1.5.3/Net/include)
include_directories(poco_1.5.3/Util/include)
include_directories(poco_1.5.3/XML/include)
include_directories(poco_1.5.3/Zip/include)

## sundials
message(STATUS "Adding sundials as subdirectory")
add_subdirectory(sundials)
include_directories(sundials/include)

## unit_test
message(STATUS "Adding unit-test as subdirectory")
add_subdirectory(unit_test)
include_directories(unit_test/src)

add_custom_target(all-dependencies)

set(ALL_DEPENDENCY_TARGETS
        all-dependencies
        blas
        arithchk
        f2c
        lapack
        libsbml-static
        libiconv
        libxml2
        zdll
        nleq1-static
        nleq2-static
        PocoFoundation
        PocoNet
        PocoUtil
        PocoXML
        PocoZip
        rr-libstruct-static
        sundials_cvode_static
        sundials_cvodes_static
        sundials_ida_static
        sundials_idas_static
        sundials_kinsol_static
        sundials_nvecserial_static
        unit_test-static)

# ensure available from whole project
set(ALL_DEPENDENCY_TARGETS ${ALL_DEPENDENCY_TARGETS} PARENT_SCOPE)

add_dependencies(all-dependencies
        blas
        arithchk
        f2c
        lapack
        libsbml-static
        libiconv
        libxml2
        zdll
        nleq1-static
        nleq2-static
        PocoFoundation
        PocoNet
        PocoUtil
        PocoXML
        PocoZip
        rr-libstruct-static
        sundials_cvode_static
        sundials_cvodes_static
        sundials_ida_static
        sundials_idas_static
        sundials_kinsol_static
        sundials_nvecserial_static
        unit_test-static
        )



















