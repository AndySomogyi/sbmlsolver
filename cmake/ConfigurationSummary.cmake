# A simple configuration summary for roadrunner.
macro(ConfigurationSummary)
    message(STATUS "
########################################################
# Build Summary for libroadrunner ${ROADRUNNER_VERSION}
########################################################

For up-to-date news and releases visit libroadrunner.org

Configured on host $ENV{COMPUTERNAME} ${HOSTNAME}

-------------------------------------------------------------------------------
Options
-------------------------------------------------------------------------------

    Host OS                             ${CMAKE_SYSTEM_NAME}
    Host architecture                   ${CMAKE_SYSTEM_PROCESSOR}
    With LLVM?                          ${BUILD_LLVM}
    With legacy C? (DEP)                ${BUILD_LEGACY_C}
    Enable Python interface?            ${BUILD_PYTHON}
    Enable Java interface?              ${BUILD_JAVA_INTERFACE}
    Enable test suite?                  ${BUILD_TESTS}

-------------------------------------------------------------------------------
CMake variables and options
-------------------------------------------------------------------------------
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

-------------------------------------------------------------------------------
Dependencies
-------------------------------------------------------------------------------
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

        RR_DEPENDENCIES_INSTALL_PREFIX  ${RR_DEPENDENCIES_INSTALL_PREFIX}
        LLVM_INSTALL_PREFIX             ${LLVM_INSTALL_PREFIX}
        BUILD_TEST_TOOLS                ${BUILD_TEST_TOOLS}
        BUILD_LLVM                      ${BUILD_LLVM}
        BUILD_LEGACY_C                  ${BUILD_LEGACY_C}
        BUILD_PYTHON                    ${BUILD_PYTHON}
        PYTHON_SITE_DIR                 ${PYTHON_SITE_DIR}
        BUILD_PYTHON                    ${BUILD_PYTHON}
        BUILD_JAVA_INTERFACE            ${BUILD_JAVA_INTERFACE}
        INSTALL_CXX_API                 ${INSTALL_CXX_API}
        INSTALL_C_API                   ${INSTALL_C_API}
        INSTALL_C_API_PYTHON            ${INSTALL_C_API_PYTHON}
        INSTALL_APPS                    ${INSTALL_APPS}
        INSTALL_EXAMPLES                ${INSTALL_EXAMPLES}
        INSTALL_STATIC_LIB              ${INSTALL_STATIC_LIB}
        RR_BUILD_SHARED_CORE            ${RR_BUILD_SHARED_CORE}
        BUILD_TESTS                     ${BUILD_TESTS}
        INSTALL_SBML_MODELS             ${INSTALL_SBML_MODELS}
        WITH_CONDA_BUILDER              ${WITH_CONDA_BUILDER}
        USE_POCO_NET                    ${USE_POCO_NET}
        VERBOSE_CMAKE                   ${VERBOSE_CMAKE}
        CMAKE_VERBOSE_MAKEFILE          ${CMAKE_VERBOSE_MAKEFILE}
        BUILD_PACKAGING                 ${BUILD_PACKAGING}
        WITH_ADDRESS_SANITIZER          ${WITH_ADDRESS_SANITIZER}
        CXX_STANDARD                    ${CXX_STANDARD}
        CMAKE_CXX_VISIBILITY_PRESET     ${CMAKE_CXX_VISIBILITY_PRESET}
        CMAKE_VISIBILITY_INLINES_HIDDEN ${CMAKE_VISIBILITY_INLINES_HIDDEN}

-------------------------------------------------------------------------------
rrplugins
-------------------------------------------------------------------------------

    Features:
      Enable shared libs?       ${TLP_BUILD_SHARED_LIB}
      Enable static libs?       ${TLP_BUILD_STATIC_LIB}

    Language bindings:
      Enable C bindings?        ${TLP_BUILD_PLUGINS_C_API}
      Enable python bindings?   ${TLP_BUILD_PLUGINS_PYTHON_API}

    Released plugins:
      Test Model                ${TLP_BUILD_TEST_MODEL_PLUGIN}
      Add Noise                 ${TLP_BUILD_ADD_NOISE_PLUGIN}
      Chi-squared               ${TLP_BUILD_CHISQUARE_PLUGIN}
      Bifurcation Auto2k        ${TLP_BUILD_AUTO2000_PLUGIN}
      Demo Plugin               ${TLP_BUILD_HELLO_PLUGIN}

    Dependencies (packages marked with *** are required for rrplugins):


End Summary
########################################################################################
")
endmacro()
