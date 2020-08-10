# A simple configuration summary for roadrunner.
macro(ConfigurationSummary)
    message(STATUS "
########################################################
# Build Summary for libroadrunner ${ROADRUNNER_VERSION}
########################################################

For up-to-date news and releases visit libroadrunner.org

Configured on host $ENV{COMPUTERNAME} ${HOSTNAME}

Options
-------
    Host OS                             ${CMAKE_SYSTEM_NAME}
    Host architecture                   ${CMAKE_SYSTEM_PROCESSOR}
    With LLVM?                          ${BUILD_LLVM}
    With legacy C? (DEP)                ${BUILD_LEGACY_C}
    Enable Python interface?            ${BUILD_PYTHON}
    Enable Java interface?              ${BUILD_JAVA_INTERFACE}
    Enable test suite?                  ${BUILD_TESTS}

CMake variables and options
---------------------------
    CMAKE_C_COMPILER                   ${CMAKE_C_COMPILER}
    CMAKE_CXX_COMPILER                 ${CMAKE_CXX_COMPILER}
    BUILD_DEFINITIONS                  ${BUILD_DEFINITIONS}
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

Dependencies
------------
    THIRD_PARTY_DIRECTORY               ${THIRD_PARTY_DIRECTORY}
    GOOGLETEST_SOURCE                   ${GOOGLETEST_SOURCE}

    LLVM:
        LLVM_FOUND                      ${LLVM_FOUND}
        LLVM_INCLUDE_DIRS               ${LLVM_INCLUDE_DIRS}
        LLVM_LIBRARY_DIRS               ${LLVM_LIBRARY_DIRS}
        LLVM_LDFLAGS                    ${LLVM_LDFLAGS}
        LLVM_LIBRARIES                  ${LLVM_LIBRARIES}
        LLVM_FLAGS                      ${LLVM_FLAGS}
        LLVM_VERSION_MAJOR              ${LLVM_VERSION_MAJOR}
        LLVM_VERSION_MINOR              ${LLVM_VERSION_MINOR}
        LLVM_VERSION_PATCH              ${LLVM_VERSION_PATCH}

        BUILD_LLVM                      ${BUILD_LLVM}
        DOWNLOAD_LLVM_BINARIES          ${DOWNLOAD_LLVM_BINARIES}
        LLVM_ROOT                       ${LLVM_ROOT}
        LLVM_INSTALL_PREFIX             ${LLVM_INSTALL_PREFIX}

    clapack:
        CLAPACK_INCLUDE_DIR             ${CLAPACK_INCLUDE_DIR}

    iconv:
        LIBICONV_INCLUDE_DIR            ${LIBICONV_INCLUDE_DIR}

    libsbml:
        LIBSBML_INCLUDE_DIR             ${LIBSBML_INCLUDE_DIR}

    libxml2:
        LIBXML2_INCLUDE_DIR             ${LIBXML2_INCLUDE_DIR}

    lzma:
        LZMA_INCLUDE_DIR                ${LZMA_INCLUDE_DIR}

    nleq1:
        NLEQ1_INCLUDE_DIR               ${NLEQ1_INCLUDE_DIR}

    nleq1:
        NLEQ2_INCLUDE_DIR               ${NLEQ2_INCLUDE_DIR}

    poco:
        POCO_INCLUDE_DIR                ${POCO_INCLUDE_DIR}

    rr-libstruct:
        RR_STRUCT_INCLUDE_DIR           ${RR_STRUCT_INCLUDE_DIR}

    sundials:
        SUNDIALS_INCLUDE_DIR            ${SUNDIALS_INCLUDE_DIR}

    unit_test:
        UNITTEST_INCLUDE_DIR            ${UNITTEST_INCLUDE_DIR}

    zlib:
        ZLIB_INCLUDE_DIR                ${ZLIB_INCLUDE_DIR}

    All:
        LINK_LIBRARIES                  ${LINK_LIBRARIES}
        INCLUDE_DIRECTORIES             ${INCLUDE_DIRECTORIES}

End Summary
########################################################################################
")
endmacro()
