macro(FindDependencies)

    # Root of the dependencies
    set(THIRD_PARTY_DIRECTORY "${CMAKE_SOURCE_DIR}/third_party")
    CheckDirectoryExists("third_party directory" "${THIRD_PARTY_DIRECTORY}")

    # clapack
    set(CLAPACK_SOURCE_DIR ${THIRD_PARTY_DIRECTORY}/clapack3.2.1)
    CheckDirectoryExists("clapack" "${CLAPACK_SOURCE_DIR}")
    set(CLAPACK_INSTALL_PREFIX "${CLAPACK_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

    # Note: Technically these dependencies are in a predictable location, so
    # find_* functions might be overkill here. But, this way allows users
    # produce their own version of the dependency by passing in *_INSTALL_PREFIX.
    find_library(CLAPACK_STATIC_LIBRARY
            NAMES
            liblapack${CMAKE_STATIC_LIBRARY_SUFFIX}
            lapack${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${CLAPACK_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(BLAS_STATIC_LIBRARY
            NAMES
            libblas${CMAKE_STATIC_LIBRARY_SUFFIX}
            blas${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${CLAPACK_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(F2C_STATIC_LIBRARY
            NAMES
            libf2c${CMAKE_STATIC_LIBRARY_SUFFIX}
            f2c${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${CLAPACK_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    set(CLAPACK_STATIC_LIBRARIES
            ${CLAPACK_STATIC_LIBRARY}
            ${BLAS_STATIC_LIBRARY}
            ${F2C_STATIC_LIBRARY}
            )

    find_path(CLAPACK_INCLUDE_DIR
            NAMES clapack/clapack.h
            HINTS ${CLAPACK_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )

#    set(LIBSBML_DEPS_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-dependencies")
#    set(LIBSBML_DEPS_INSTALL_PREFIX "${LIBSBML_DEPS_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
#
#    # libsbml
#    set(LIBSBML_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source")
#    CheckDirectoryExists("libsbml source" "${LIBSBML_SOURCE_DIR}")
#
#    set(LIBSBML_INSTALL_PREFIX "${LIBSBML_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
#
#    find_library(LIBSBML_STATIC_LIBRARY
#            NAMES
#            libsbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
#            sbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
#            HINTS ${LIBSBML_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_path(LIBSBML_INCLUDE_DIR
#            NAMES sbml/SBMLTypes.h
#            HINTS ${LIBSBML_INSTALL_PREFIX}/include
#            NO_DEFAULT_PATH
#            )
#
    # libsbml dependencies - These are not needed here for libsbml itself
    # as the libsbml-dependencies target is built into libsbml inside a directory
    # called dependencies. libsbml knows about this folder and uses it.
#    find_library(EXPAT_STATIC_LIBRARY
#            NAMES
#            expat${CMAKE_STATIC_LIBRARY_SUFFIX}
#            libexpat${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_library(LIBBZ2_STATIC_LIBRARY
#            NAMES
#            libbz2${CMAKE_STATIC_LIBRARY_SUFFIX}
#            bz2${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_library(LIBCHECK_STATIC_LIBRARY
#            NAMES
#            libcheck${CMAKE_STATIC_LIBRARY_SUFFIX}
#            check${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_library(LIBICONV_STATIC_LIBRARY
#            NAMES
#            libiconv${CMAKE_STATIC_LIBRARY_SUFFIX}
#            iconv${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_library(LIBXML2_STATIC_LIBRARY
#            NAMES
#            libxml2${CMAKE_STATIC_LIBRARY_SUFFIX}
#            xml2${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )
#
#    find_library(ZLIB_STATIC_LIBRARY
#            NAMES
#            zdll${CMAKE_STATIC_LIBRARY_SUFFIX}
#            libz${CMAKE_STATIC_LIBRARY_SUFFIX}
#            libzlib${CMAKE_STATIC_LIBRARY_SUFFIX}
#            PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
#            NO_DEFAULT_PATH
#            )

    set(LIBSBML_DEPENDENCIES_LIBRARIES
            ${LIBXML2_STATIC_LIBRARY}
            ${EXPAT_STATIC_LIBRARY}
            ${LIBBZ2_STATIC_LIBRARY}
            ${LIBCHECK_STATIC_LIBRARY}
            ${LIBICONV_STATIC_LIBRARY}
            ${ZLIB_STATIC_LIBRARY}
            )


    # poco
    set(POCO_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/poco_1.5.3")
    CheckDirectoryExists("poco source" "${POCO_SOURCE_DIR}")
    set(POCO_INSTALL_PREFIX "${POCO_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

    find_library(POCO_FOUNDATION_STATIC_LIBRARY
            NAMES
            libPocoFoundation${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoFoundation${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_library(POCO_NET_STATIC_LIBRARY
            NAMES
            libPocoNet${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoNet${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_library(POCO_UTIL_STATIC_LIBRARY
            NAMES
            libPocoUtil${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoUtil${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_library(POCO_XML_STATIC_LIBRARY
            NAMES
            libPocoXML${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoXML${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_library(POCO_ZIP_STATIC_LIBRARY
            NAMES
            libPocoZip${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoZip${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    # Ordering is important for linux builds.
    set(POCO_STATIC_LIBRARIES
            ${POCO_NET_STATIC_LIBRARY}
            ${POCO_UTIL_STATIC_LIBRARY}
            ${POCO_XML_STATIC_LIBRARY}
            ${POCO_ZIP_STATIC_LIBRARY}
            ${POCO_FOUNDATION_STATIC_LIBRARY}
            )

    find_path(POCO_INCLUDE_DIR
            NAMES Poco/Poco.h
            HINTS ${POCO_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )


    # Sundials
    set(SUNDIALS_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/sundials")
    CheckDirectoryExists("sundials source" "${SUNDIALS_SOURCE_DIR}")
    set(SUNDIALS_INSTALL_PREFIX "${SUNDIALS_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

    find_library(SUNDIALS_CVODE_STATIC_LIBRARY
            NAMES
            libsundials_cvode${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_cvode${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(SUNDIALS_CVODES_STATIC_LIBRARY
            NAMES
            libsundials_cvodes${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_cvodes${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(SUNDIALS_IDA_STATIC_LIBRARY
            NAMES
            libsundials_ida${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_ida${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(SUNDIALS_IDAS_STATIC_LIBRARY
            NAMES
            libsundials_idas${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_idas${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(SUNDIALS_KINSOL_STATIC_LIBRARY
            NAMES
            libsundials_kinsol${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_kinsol${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_library(SUNDIALS_NVECSERIAL_STATIC_LIBRARY
            NAMES
            libsundials_nvecserial${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_nvecserial${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )

    find_path(SUNDIALS_INCLUDE_DIR
            NAMES cvode/cvode.h
            HINTS ${SUNDIALS_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )

    set(SUNDIALS_STATIC_LIBRARIES
            ${SUNDIALS_CVODE_STATIC_LIBRARY}
            ${SUNDIALS_CVODES_STATIC_LIBRARY}
            ${SUNDIALS_IDA_STATIC_LIBRARY}
            ${SUNDIALS_IDAS_STATIC_LIBRARY}
            ${SUNDIALS_KINSOL_STATIC_LIBRARY}
            ${SUNDIALS_NVECSERIAL_STATIC_LIBRARY}
            )

    # nleq1
    set(NLEQ1_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/nleq1")
    set(NLEQ1_INSTALL_PREFIX "${NLEQ1_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(NLEQ1_STATIC_LIBRARY
            NAMES
            libnleq1-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            nleq1-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${NLEQ1_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_path(NLEQ1_INCLUDE_DIR
            NAMES nleq1/nleq1.h
            HINTS ${NLEQ1_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )


    # nleq2
    set(NLEQ2_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/nleq2")
    set(NLEQ2_INSTALL_PREFIX "${NLEQ2_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(NLEQ2_STATIC_LIBRARY
            NAMES
            libnleq2-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            nleq2-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${NLEQ2_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_path(NLEQ2_INCLUDE_DIR
            NAMES nleq2/nleq2.h
            HINTS ${NLEQ2_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )

    # rr-libstruct
    set(RR_LIBSTRUCT_DIR "${THIRD_PARTY_DIRECTORY}/rr-libstruct")
    set(RR_LIBSTRUCT_INSTALL_PREFIX "${RR_LIBSTRUCT_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(RR_LIBSTRUCT_STATIC_LIBRARY
            NAMES
            librr-libstruct-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            rr-libstruct-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${RR_LIBSTRUCT_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_path(RR_LIBSTRUCT_INCLUDE_DIR
            NAMES rr-libstruct/lsMatrix.h
            HINTS ${RR_LIBSTRUCT_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )

    # unittest
    set(UNIT_TEST_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/unit_test")
    set(UNIT_TEST_INSTALL_PREFIX "${UNIT_TEST_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(UNIT_TEST_STATIC_LIBRARY
            NAMES
            libunit_test-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            unit_test-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${UNIT_TEST_INSTALL_PREFIX}/lib
            NO_DEFAULT_PATH
            )
    find_path(UNIT_TEST_INCLUDE_DIR
            NAMES unit_test/Test.h
            HINTS ${UNIT_TEST_INSTALL_PREFIX}/include
            NO_DEFAULT_PATH
            )
endmacro()