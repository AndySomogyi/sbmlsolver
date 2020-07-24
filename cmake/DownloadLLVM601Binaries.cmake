# A macro for downloading LLVM6.0.1 binaries directly from
# a github release.
macro(DownloadLLVM601Binaries)
    # https://www.scivision.dev/cmake-download-verify-file/

    # Check to see if the LLVM_ROOT variable is defined
    if (NOT DEFINED LLVM_ROOT)
        message(FATAL_ERROR "The LLVM_ROOT variable is not defined")
    endif ()

    # if the LLVM directory does not exist, create it.
    if (NOT EXISTS "${LLVM_ROOT}")
        file(MAKE_DIRECTORY "${LLVM_ROOT}")
    endif ()

    # Set a variable for where to put the downloaded zip file.
    set(LLVM_DOWNLOAD_ZIP "${LLVM_ROOT}/llvm6.0.1.zip")

    # Download from github or locate existing download of zip
    if (NOT EXISTS ${LLVM_DOWNLOAD_ZIP})
        message(STATUS "Downloading llvm binaries from \"${LLVM_BINARY_URL}\" to \"${LLVM_DOWNLOAD_ZIP}\"")
        file(DOWNLOAD ${LLVM_BINARY_URL} ${LLVM_DOWNLOAD_ZIP}
                #       EXPECTED_HASH ${hash_type}=${hash}
                TLS_VERIFY ON)
    else ()
        message(STATUS "Existing download of LLVM6.0.1 found \"${LLVM_DOWNLOAD_ZIP}\".")
    endif ()

    if (NOT DEFINED LLVM_INSTALL_DIR)
        message(FATAL_ERROR "LLVM_INSTALL_DIR variable is not defined")
    endif ()

    # if we haven't already extracted the zip, do it now
    if (NOT EXISTS ${LLVM_INSTALL_DIR}/bin/llvm-ar)
        # Extract zip file into install location
        file(ARCHIVE_EXTRACT INPUT "${LLVM_DOWNLOAD_ZIP}" DESTINATION "${LLVM_ROOT}" VERBOSE)
    endif ()

endmacro()