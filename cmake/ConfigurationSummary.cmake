# A simple configuration summary for roadrunner.
macro(ConfigurationSummary)
    message(STATUS "
########################################################
# Build Summary for libroadrunner ${ROADRUNNER_VERSION}#
########################################################

CMake variables and options
---------------------------
    CMAKE_SYSTEM_VERSION               ${CMAKE_SYSTEM_VERSION}
    ROADRUNNER_VERSION                 ${ROADRUNNER_VERSION}
    CMAKE_CXX_COMPILER_ID              ${CMAKE_CXX_COMPILER_ID}
    CMAKE_CXX_COMPILER_VERSION         ${CMAKE_CXX_COMPILER_VERSION}
    CMAKE_CXX_STANDARD                 ${CMAKE_CXX_STANDARD}
    CMAKE_POSITION_INDEPENDENT_CODE    ${CMAKE_POSITION_INDEPENDENT_CODE}
    CMAKE_INSTALL_RPATH                ${CMAKE_INSTALL_RPATH}
    CMAKE_CXX_FLAGS                    ${CMAKE_CXX_FLAGS}
    CMAKE_C_FLAGS                      ${CMAKE_C_FLAGS}
    CMAKE_CXX_FLAGS_RELEASE            ${CMAKE_CXX_FLAGS_RELEASE}
    CMAKE_C_FLAGS_RELEASE              ${CMAKE_C_FLAGS_RELEASE}
    CMAKE_CXX_FLAGS_DEBUG              ${CMAKE_CXX_FLAGS_DEBUG}
    CMAKE_C_FLAGS_DEBUG                ${CMAKE_C_FLAGS_DEBUG}
    CMAKE_INSTALL_PREFIX               ${CMAKE_INSTALL_PREFIX}
    CMAKE_VERBOSE_MAKEFILE             ${CMAKE_VERBOSE_MAKEFILE}
    CMAKE_BUILD_TYPE                   ${CMAKE_BUILD_TYPE}
    CMAKE_RUNTIME_OUTPUT_DIRECTORY     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    CMAKE_LIBRARY_OUTPUT_DIRECTORY     ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    CMAKE_ARCHIVE_OUTPUT_DIRECTORY     ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
    MSVC_RUNTIME_LIBRARY               ${MSVC_RUNTIME_LIBRARY}

    WITH_SANITIZER                     ${WITH_SANITIZER}
    BUILD_SHARED_LIBS                  ${BUILD_SHARED_LIBS}
    BUILD_DOCS                         ${BUILD_DOCS}
    BUILD_TESTS                        ${BUILD_TESTS}

VCPKG config
------------
    VCPKG_ROOT                          ${VCPKG_ROOT}
    CMAKE_TOOLCHAIN_FILE                ${CMAKE_TOOLCHAIN_FILE}
    VCPKG_ARCH                          ${VCPKG_ARCH}
    VCPKG_INSTALLED_PACKAGES            ${VCPKG_INSTALLED_PACKAGES}
    VCPKG_STATIC_INSTALLED_PACKAGES     ${VCPKG_STATIC_INSTALLED_PACKAGES}
    VCPKG_STATIC_LIB_DIR                ${VCPKG_STATIC_LIB_DIR}
    VCPKG_LIB_DIR                       ${VCPKG_LIB_DIR}
    VCPKG_BIN_DIR                       ${VCPKG_BIN_DIR}
    VCPKG_INCLUDE_DIR                   ${VCPKG_INCLUDE_DIR}

Dependencies
------------
    GOOGLETEST_SOURCE                   ${GOOGLETEST_SOURCE}

    clapack:
        CLAPACK_SOURCE_DIR              ${CLAPACK_SOURCE_DIR}
        CLAPACK_INCLUDE_DIR             ${CLAPACK_INCLUDE_DIR}
        CLAPACK_STATIC_LIBRARY          ${CLAPACK_STATIC_LIBRARY}
    libsbml:
        LIBSBML_SOURCE_DIR              ${LIBSBML_SOURCE_DIR}
        LIBSBML_INSTALL_PREFIX          ${LIBSBML_INSTALL_PREFIX}
        LIBSBML_INCLUDE_DIR             ${LIBSBML_INCLUDE_DIR}
        LIBSBML_STATIC_LIBRARY          ${LIBSBML_STATIC_LIBRARY}
        LIBSBML_LIBRARY                 ${LIBSBML_LIBRARY}
    poco:
        POCO_SOURCE_DIR                 ${POCO_SOURCE_DIR}
        POCO_STATIC_LIBRARY             ${POCO_STATIC_LIBRARY}
        POCO_INCLUDE_DIR                ${POCO_INCLUDE_DIR}
    sundials:
        SUNDIALS_SOURCE_DIR             ${SUNDIALS_SOURCE_DIR}
        SUNDIALS_STATIC_LIBRARY         ${SUNDIALS_STATIC_LIBRARY}
        SUNDIALS_INCLUDE_DIR            ${SUNDIALS_INCLUDE_DIR}
    LLVM
        BUILD_LLVM                      ${BUILD_LLVM}
        DOWNLOAD_LLVM_BINARIES          ${DOWNLOAD_LLVM_BINARIES}
        LLVM_ROOT                       ${LLVM_ROOT}
        LLVM_INSTALL_PREFIX             ${LLVM_INSTALL_PREFIX}

Build variables
---------------

End Summary
########################################################################################
")
endmacro()
