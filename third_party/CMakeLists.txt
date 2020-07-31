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

ExternalProject_Add(
        nleq1
        SOURCE_DIR ${NLEQ1_SOURCE_DIR}
        LOG_DIR ${NLEQ1_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${NLEQ1_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

ExternalProject_Add(
        nleq2
        SOURCE_DIR ${NLEQ2_SOURCE_DIR}
        LOG_DIR ${NLEQ2_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${NLEQ2_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

# clapack3.2.1
ExternalProject_Add(
        clapack
        SOURCE_DIR ${CLAPACK_SOURCE_DIR}
        LOG_DIR ${CLAPACK_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${CLAPACK_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DCLAPACK_INCLUDE_DIR=${CLAPACK_INCLUDE_DIR}
        -DLIBSBML_INCLUDE_DIR=${LIBSBML_INCLUDE_DIR}
)

# libsbml dependencies
ExternalProject_Add(
        libsbml_dependencies
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies
        LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-dependencies/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${LIBSBML_DEPS_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)


# libsbml - looks for its dependencies in a folder called `dependencies` at libsbml root.
ExternalProject_Add(
        libsbml
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source
        LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libSBML-5.18.1-experimental-Source/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DLIBSBML_DEPENDENCY_DIR=${LIBSBML_DEPS_INSTALL_PREFIX}
        -DCMAKE_INSTALL_PREFIX=${LIBSBML_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DWITH_CPP_NAMESPACE=ON
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        DEPENDS libsbml_dependencies
)


# add rr-libstruct subdirectory. Depends on libsbml
#add_subdirectory(rr-libstruct)
ExternalProject_Add(
        rr-libstruct
        SOURCE_DIR ${RR_LIBSTRUCT_DIR}
        LOG_DIR ${RR_LIBSTRUCT_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${RR_LIBSTRUCT_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        # These should default to the correct location inside rr-libstruct anyway
        -DLIBSBML_INSTALL_PREFIX=${LIBSBML_INSTALL_PREFIX}
        -DCLAPACK_INSTALL_PREFIX=${CLAPACK_INSTALL_PREFIX}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)
add_dependencies(rr-libstruct libsbml clapack)

# poco
ExternalProject_Add(
        poco
        SOURCE_DIR ${POCO_SOURCE_DIR}
        LOG_DIR ${POCO_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${POCO_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DFoundation_EXPORTS=ON

)

# sundials
ExternalProject_Add(
        sundials
        SOURCE_DIR ${SUNDIALS_SOURCE_DIR}
        LOG_DIR ${SUNDIALS_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${SUNDIALS_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

# unit_test
ExternalProject_Add(
        unittest
        SOURCE_DIR ${UNIT_TEST_SOURCE_DIR}
        LOG_DIR ${UNIT_TEST_SOURCE_DIR}/logs-${CMAKE_CXX_COMPILER_ID}
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${UNIT_TEST_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)

add_custom_target(all-dependencies)

add_dependencies(all-dependencies
        libsbml
        poco
        sundials
        nleq1
        nleq2
        clapack
        libsbml_dependencies
        libsbml
        poco
        sundials
        rr-libstruct
        unittest
        )




















