# Macro to determine whether we should build llvm6.0.1 or download the binaries.
# LLVM takes a long time to build, so our preference is to download the binaries
# when possible. This is only possible in a select few cases:
#   - When doing a Release build
#   - When using MSVC 2019 (19.26.28806.0) compiler or newer
#   - When using GNU compilers 10.1.0 or newer
# This macro defines two variables:
#   - BUILD_LLVM
#   - DOWNLOAD_LLVM_BINARIES
# which are mutually exclusive flags to determine how to ascertain LLVM.
macro(DownloadOrBuildLLVM)
    set(BUILD_LLVM OFF)
    set(DOWNLOAD_LLVM_BINARIES ON)

    # Check compiler versions
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(LLVM_BINARY_URL "https://github.com/CiaranWelsh/LLVM6.0.1/releases/download/v0.0.1/llvm6.0.1-ubuntu.zip")

        # Make sure we are using the same compiler that LLVM was compiled with.
        # If not, we must build from source.
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "10.1.0")
            message(WARNING "Detecting GNU compiler version ${CMAKE_CXX_COMPILER_VERSION}
but prebuilt binaries were built with version 10.1.0. Switching CMake to build LLVM from source.")
            set(BUILD_LLVM ON)
            set(DOWNLOAD_LLVM_BINARIES OFF)
        endif ()

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(LLVM_BINARY_URL "https://github.com/CiaranWelsh/LLVM6.0.1/releases/download/v0.0.1/llvm6.0.1-msvc.zip")
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.26.28806.0")
            message(WARNING "Detecting MSVC compiler version ${CMAKE_CXX_COMPILER_VERSION}
but prebuilt binaries were built with version 19.26.28806.0. Switching CMake to build LLVM from source.")
            set(BUILD_LLVM ON)
            set(DOWNLOAD_LLVM_BINARIES OFF)
        endif ()
    else ()
        message(FATAL_ERROR "Macos not yet supported in the new rr build system.
This is coming as soon as possible.")
    endif ()

    # Additionally, since only the Release builds are uploaded, we can only
    # download binaries in Release mode. This is true on any system
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(BUILD_LLVM ON)
        set(DOWNLOAD_LLVM_BINARIES OFF)
    endif ()


    # Sanity check, BUILD_LLVM and DOWNLOAD_LLVM_BINARIES are mutually exclusive
    if (${BUILD_LLVM} AND ${DOWNLOAD_LLVM_BINARIES})
        message(FATAL_ERROR "Both variables  (\"${Build_LLVM}\" and \"${DOWNLOAD_LLVM_BINARIES}\"  \
are set to ON, but these variables are mutually exclusive. If you are seeing this error, then
this is a build problem. Please kindly report the error as a GitHub Issue. ")
    endif ()

endmacro()