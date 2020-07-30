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
            )
    find_library(BLAS_STATIC_LIBRARY
            NAMES
            libblas${CMAKE_STATIC_LIBRARY_SUFFIX}
            blas${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${CLAPACK_INSTALL_PREFIX}/lib
            )
    find_library(F2C_STATIC_LIBRARY
            NAMES
            libf2c${CMAKE_STATIC_LIBRARY_SUFFIX}
            f2c${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${CLAPACK_INSTALL_PREFIX}/lib
            )

    set(CLAPACK_STATIC_LIBRARIES
            ${CLAPACK_STATIC_LIBRARY}
            ${BLAS_STATIC_LIBRARY}
            ${F2C_STATIC_LIBRARY})

    find_path(CLAPACK_INCLUDE_DIR
            NAMES clapack/clapack.h
            HINTS ${CLAPACK_INSTALL_PREFIX}/include
            )

    set(LIBSBML_DEPS_INSTALL_PREFIX "${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source/dependencies")

    # libsbml
    set(LIBSBML_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/libSBML-5.18.1-experimental-Source")
    CheckDirectoryExists("libsbml source" "${LIBSBML_SOURCE_DIR}")

    set(LIBSBML_INSTALL_PREFIX "${LIBSBML_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

    find_library(LIBSBML_STATIC_LIBRARY
            NAMES
            libsbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            sbml-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${LIBSBML_INSTALL_PREFIX}/lib
            )

    find_path(LIBSBML_INCLUDE_DIR
            NAMES sbml/SBMLTypes.h
            HINTS ${LIBSBML_INSTALL_PREFIX}/include
            )

        # libsbml dependencies - These are not needed here for libsbml itself
    # as the libsbml-dependencies target is built into libsbml inside a directory
    # called dependencies. libsbml knows about this folder and uses it.
    # However, it seems we need libsbml dependencies for creating a DLL on windows.
    if (WIN32)
        find_library(EXPAT_STATIC_LIBRARY
                NAMES expat.lib
                PATHS ${LIBSBML_INSTALL_PREFIX}/lib
                )

        find_library(LIBBZ2_STATIC_LIBRARY
                NAMES libbz2.lib
                PATHS ${LIBSBML_INSTALL_PREFIX}/lib
                )

        find_library(LIBCHECK_STATIC_LIBRARY
                NAMES libcheck.lib
                PATHS ${LIBSBML_INSTALL_PREFIX}/lib
                )

        find_library(LIBICONV_STATIC_LIBRARY
                NAMES libiconv.lib
                PATHS ${LIBSBML_INSTALL_PREFIX}/lib
                )

        find_library(LIBXML2_STATIC_LIBRARY
                NAMES libxml2.lib
                PATHS ${LIBSBML_INSTALL_PREFIX}/lib
                )

        find_library(ZDLL_STATIC_LIBRARY
                NAMES zdll.lib
                PATHS ${LIBSBML_DEPS_INSTALL_PREFIX}/lib
                )

        set(LIBSBML_DEPENDENCIES_LIBRARIES
                ${EXPAT_STATIC_LIBRARY}
                ${LIBBZ2_STATIC_LIBRARY}
                ${LIBCHECK_STATIC_LIBRARY}
                ${LIBICONV_STATIC_LIBRARY}
                ${LIBXML2_STATIC_LIBRARY}
                ${ZDLL_STATIC_LIBRARY}
                )

    endif ()


    # poco
    set(POCO_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/poco_1.5.3")
    CheckDirectoryExists("poco source" "${POCO_SOURCE_DIR}")
    set(POCO_INSTALL_PREFIX "${POCO_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")

    find_library(POCO_FOUNDATION_STATIC_LIBRARY
            NAMES
            libPocoFoundation${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoFoundation${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            )
    find_library(POCO_NET_STATIC_LIBRARY
            NAMES
            libPocoNet${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoNet${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            )
    find_library(POCO_UTIL_STATIC_LIBRARY
            NAMES
            libPocoUtil${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoUtil${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            )
    find_library(POCO_XML_STATIC_LIBRARY
            NAMES
            libPocoXML${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoXML${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            )
    find_library(POCO_ZIP_STATIC_LIBRARY
            NAMES
            libPocoZip${CMAKE_STATIC_LIBRARY_SUFFIX}
            PocoZip${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${POCO_INSTALL_PREFIX}/lib
            )

    set(POCO_STATIC_LIBRARIES
            ${POCO_FOUNDATION_STATIC_LIBRARY}
            ${POCO_NET_STATIC_LIBRARY}
            ${POCO_UTIL_STATIC_LIBRARY}
            ${POCO_XML_STATIC_LIBRARY}
            ${POCO_ZIP_STATIC_LIBRARY}
            )

    find_path(POCO_INCLUDE_DIR
            NAMES Poco/Poco.h
            HINTS ${POCO_INSTALL_PREFIX}/include
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
            )

    find_library(SUNDIALS_CVODES_STATIC_LIBRARY
            NAMES
            libsundials_cvodes${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_cvodes${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            )

    find_library(SUNDIALS_IDA_STATIC_LIBRARY
            NAMES
            libsundials_ida${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_ida${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            )

    find_library(SUNDIALS_IDAS_STATIC_LIBRARY
            NAMES
            libsundials_idas${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_idas${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            )

    find_library(SUNDIALS_KINSOL_STATIC_LIBRARY
            NAMES
            libsundials_kinsol${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_kinsol${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            )

    find_library(SUNDIALS_NVECSERIAL_STATIC_LIBRARY
            NAMES
            libsundials_nvecserial${CMAKE_STATIC_LIBRARY_SUFFIX}
            sundials_nvecserial${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${SUNDIALS_INSTALL_PREFIX}/lib
            )

    find_path(SUNDIALS_INCLUDE_DIR
            NAMES cvode/cvode.h
            HINTS ${SUNDIALS_INSTALL_PREFIX}/include
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
            )
    find_path(NLEQ1_INCLUDE_DIR
            NAMES nleq1/nleq1.h
            HINTS ${NLEQ1_INSTALL_PREFIX}/include
            )


    # nleq2
    set(NLEQ2_SOURCE_DIR "${THIRD_PARTY_DIRECTORY}/nleq2")
    set(NLEQ2_INSTALL_PREFIX "${NLEQ2_SOURCE_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(NLEQ2_STATIC_LIBRARY
            NAMES
            libnleq2-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            nleq2-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${NLEQ2_INSTALL_PREFIX}/lib
            )
    find_path(NLEQ2_INCLUDE_DIR
            NAMES nleq2/nleq2.h
            HINTS ${NLEQ2_INSTALL_PREFIX}/include
            )

    # rr-libstruct
    set(RR_LIBSTRUCT_DIR "${THIRD_PARTY_DIRECTORY}/rr-libstruct")
    set(RR_LIBSTRUCT_INSTALL_PREFIX "${RR_LIBSTRUCT_DIR}/install-${CMAKE_CXX_COMPILER_ID}")
    find_library(RR_LIBSTRUCT_STATIC_LIBRARY
            NAMES
            librr-libstruct-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            rr-libstruct-static${CMAKE_STATIC_LIBRARY_SUFFIX}
            HINTS ${RR_LIBSTRUCT_INSTALL_PREFIX}/lib
            )
    find_path(RR_LIBSTRUCT_INCLUDE_DIR
            NAMES rr-libstruct/lsMatrix.h
            HINTS ${RR_LIBSTRUCT_INSTALL_PREFIX}/include
            )
endmacro()