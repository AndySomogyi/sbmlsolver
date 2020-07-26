macro(FindDependencies)

    # clapack3.2.1, libsbml, poco, sundials

    # clapack3.2.1
#    find_library(CLAPACK_STATIC_LIBRARY
#            NAMES clapack3.2.1.lib clapack3.2.1.a
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