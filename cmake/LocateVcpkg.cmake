# Try and find vcpkg root. If it exists in:
# on Windows:
#   - C:\vcpkg
#   - D:\vcpkg
# on Linux:
#   - ~/vcpkg
# it will be found automatically. If not, then you will need
# to give cmake the following definition:
#   -DVCPKG_ROOT=/path/to/vcpkg/root

# defines the variable "${VCPKG_ROOT}"
macro(LocateVcpkg)

    set(VCPKG_VERBOSE ON)

    # defines a utility macro for setting variables in a cross platform way
    include(SetCrossPlatform)

    # Try and find the vcpkg root directory
    find_path(VCPKG_ROOT
            NAMES vcpkg vcpkg.exe bootstrap-vcpkg.bat bootstrap-vcpkg.sh
            HINTS C:/vcpkg D:/vcpkg ~/vcpkg
            DOC "Full path to the vcpkg root"
            )

    if (NOT EXISTS ${VCPKG_ROOT})
        message(FATAL_ERROR "vcpkg root not found (${VCPKG_ROOT}). Please\
        give libroadunner the absolute path to your vcpkg root directory using (for instance)\
        -DVCPKG_ROOT=/home/ciaran/vcpkg")
    endif ()

    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Path to vcpkg toolchain file")

    # ensure the toolchain file exists and user has provided it.
    if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE})
        message(FATAL_ERROR "No vcpkg toolchain installed, got \"${CMAKE_TOOLCHAIN_FILE}\".\
Have you passed in the root to vcpkg? i.e. -DVCPKG_ROOT=\"D:/vcpkg\"")
    endif ()

    # When user doesn't give VCPKG_ARCH set some defaults.
    if (NOT DEFINED VCPKG_ARCH)
        SetCrossPlatform(VCPKG_ARCH "x64-windows" "x64-linux" "x64-osx")
    endif()

    # location of installed libraries in vcpkg
    set(VCPKG_INSTALLED_PACKAGES "${VCPKG_ROOT}/installed/${VCPKG_ARCH}")

    # location of installed static packages in vcpkg
    SetCrossPlatform(VCPKG_STATIC_INSTALLED_PACKAGES
            "${VCPKG_ROOT}/installed/x64-windows-static"
            "${VCPKG_ROOT}/installed/x64-linux" # note: on linux, this is same for shared and static
            "${VCPKG_ROOT}/installed/x64-osx"
            )

    # Path to static libraries
    set(VCPKG_STATIC_LIB_DIR "${VCPKG_STATIC_INSTALLED_PACKAGES}/lib")
    # Path to shared libraries
    set(VCPKG_LIB_DIR "${VCPKG_INSTALLED_PACKAGES}/lib")
    # Path to binaries
    SetCrossPlatform(VCPKG_BIN_DIR
            "${VCPKG_INSTALLED_PACKAGES}/bin" # windows
            "${VCPKG_INSTALLED_PACKAGES}/lib" # linux
            "${VCPKG_INSTALLED_PACKAGES}/lib" # mac
            )
    # include directories (should be the same for static and dynamic)
    set(VCPKG_INCLUDE_DIR "${VCPKG_INSTALLED_PACKAGES}/include")


    set(_VCPKG_DIRECTORIES
            ${VCPKG_STATIC_LIB_DIR}
            ${VCPKG_LIB_DIR}
            ${VCPKG_BIN_DIR}
            ${VCPKG_INCLUDE_DIR}
            )

    # ensure the directories we've located exist
    foreach (d ${_VCPKG_DIRECTORIES})
        if (NOT EXISTS ${d})
            message(FATAL_ERROR "VCPKG directory \"${d}\" does not exist")
        endif ()
    endforeach ()

    # The libsbml port does not yet exist on the official vcpkg
    # git repository. It is only available from:
    #   https://github.com/CiaranWelsh/vcpkg
    # Therefore, we do a check and fail if user is using the official vcpkg


    #Need to check to see whether the user is using my version
    # of the official version, because I have the additional ports
    # that will be needed
endmacro()