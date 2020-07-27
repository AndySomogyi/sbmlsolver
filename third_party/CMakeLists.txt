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

# add nleq1 subdirectory. No dependencies.
add_subdirectory(nleq1)

# add nleq2 subdirectory. No dependencies.
add_subdirectory(nleq2)

# clapack3.2.1
add_subdirectory(clapack3.2.1)

# libsbml dependencies
ExternalProject_Add(
        libsbml_dependencies
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies
        LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source/dependencies
)


# libsbml - looks for its dependencies in a folder called `dependencies` at libsbml root.
ExternalProject_Add(
        libsbml
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source
        LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LIBSBML_INSTALL_PREFIX} -DLIBSBML_USE_CPP_NAMESPACE=ON
        DEPENDS libsbml_dependencies
)


# add rr-libstruct subdirectory. Depends on libsbml
add_subdirectory(rr-libstruct)
add_dependencies(rr-libstruct-static libsbml lapack)


# poco
ExternalProject_Add(
        poco
        SOURCE_DIR ${POCO_SOURCE_DIR}
        LOG_DIR ${POCO_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${POCO_INSTALL_PREFIX}
)

# sundials
ExternalProject_Add(
        sundials
        SOURCE_DIR ${SUNDIALS_SOURCE_DIR}
        LOG_DIR ${SUNDIALS_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${SUNDIALS_INSTALL_PREFIX}
)

add_custom_target(all-dependencies)
add_dependencies(all-dependencies
        libsbml
        poco
        blas
        arithchk
        sundials
        f2c
        lapack
        nleq1-static
        nleq2-static
        rr-libstruct-static
        )




















