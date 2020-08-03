# Root of the dependencies
set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")

## nleq1
add_subdirectory(nleq1)

## nleq2
add_subdirectory(nleq2)

## clapack3.2.1
add_subdirectory(clapack3.2.1)

## libsbml dependencies
add_subdirectory(libSBML-dependencies)

# set the variables needed by libsbml here:
set(LIBSBML_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source)
set(LIBSBML_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/libSBML-5.18.1-experimental-Source)
set(LIBXML_LIBRARY "$<TARGET_FILE:libxml2>")
set(LIBZ_LIBRARY "$<TARGET_FILE:zdll>")
set(WITH_CPP_NAMESPACE ON)
set(LIBSBML_SKIP_SHARED_LIBRARY ON)
set(WITH_CHECK OFF)

add_subdirectory(libSBML-5.18.1-experimental-Source)

add_subdirectory(rr-libstruct)

## poco
add_subdirectory(poco_1.5.3)

## sundials
add_subdirectory(sundials)

## unit_test
add_subdirectory(unit_test)

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



















