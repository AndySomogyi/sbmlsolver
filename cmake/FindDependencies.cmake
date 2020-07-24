macro(FindDependencies)

    # clapack, libsbml, poco, sundials

    # clapack
#    find_library(CLAPACK_STATIC_LIBRARY
#            NAMES clapack.lib clapack.a
#            PATHS ${VCPKG_LIB_DIR}
#            NO_DEFAULT_PATH
#            )
#
#    find_path(CLAPACK_INCLUDE_DIR
#            NAMES libxml/parser.h
#            PATHS ${VCPKG_INCLUDE_DIR}
#            NO_DEFAULT_PATH
#            )

    # libsbml
    message(STATUS "VCPKG_LIB_DIR ${VCPKG_LIB_DIR}")
    find_library(LIBSBML_STATIC_LIBRARY
            NAMES libsbml-static.lib libsbml-static.a
            PATHS "${VCPKG_STATIC_LIB_DIR}"
            NO_DEFAULT_PATH
            )

    find_library(LIBSBML_INCLUDE_DIR
            NAMES sbml/SBMLTypes.h
            PATHS ${VCPKG_INCLUDE_DIR}
            NO_DEFAULT_PATH
            )


endmacro()