# Find LLVM
#
# It defines the following variables
#  LLVM_FOUND        - True if llvm found.
#  LLVM_INCLUDE_DIRS  - where to find llvm include files
#  LLVM_LIBRARY_DIRS  - where to find llvm libs
#  LLVM_LDFLAGS       - llvm linker flags
#  LLVM_LIBRARIES          - list of llvm libs for working with modules.
#                      This is a list of absolute path file names, which is
#                      evidently the cmake convention.
#  LLVM_FLAGS     - flags to add to the CXX compiler for llvm support
#  LLVM_VERSION_MAJOR
#  LLVM_VERSION_MINOR
#  LLVM_VERSION_PATCH
#################################################################################

# this function borrowed from PlPlot, Copyright (C) 2006  Alan W. Irwin
function(TRANSFORM_VERSION numerical_result version)
    # internal_version ignores everything in version after any character that
    # is not 0-9 or ".".  This should take care of the case when there is
    # some non-numerical data in the patch version.
    #message(STATUS "DEBUG: version = ${version}")
    string(REGEX REPLACE "^([0-9.]+).*$" "\\1" internal_version ${version})

    # internal_version is normally a period-delimited triplet string of the form
    # "major.minor.patch", but patch and/or minor could be missing.
    # Transform internal_version into a numerical result that can be compared.
    string(REGEX REPLACE "^([0-9]*).+$" "\\1" major ${internal_version})
    string(REGEX REPLACE "^[0-9]*\\.([0-9]*).*$" "\\1" minor ${internal_version})
    string(REGEX REPLACE "^[0-9]*\\.[0-9]*\\.([0-9]*)$" "\\1" patch ${internal_version})

    if (NOT patch MATCHES "^[0-9]+$")
        set(patch 0)
    endif (NOT patch MATCHES "^[0-9]+$")

    if (NOT minor MATCHES "[0-9]+")
        set(minor 0)
    endif (NOT minor MATCHES "[0-9]+")

    if (NOT major MATCHES "[0-9]+")
        set(major 0)
    endif (NOT major MATCHES "[0-9]+")
    #message(STATUS "DEBUG: internal_version = ${internal_version}")
    #message(STATUS "DEBUG: major = ${major}")
    #message(STATUS "DEBUG: minor= ${minor}")
    #message(STATUS "DEBUG: patch = ${patch}")
    math(EXPR internal_numerical_result
            "${major}*1000000 + ${minor}*1000 + ${patch}"
            #"${major}*1000000 + ${minor}*1000"
            )
    #message(STATUS "DEBUG: ${numerical_result} = ${internal_numerical_result}")
    set(${numerical_result} ${internal_numerical_result} PARENT_SCOPE)
endfunction(TRANSFORM_VERSION)

# first try to find llvm-config
find_program(LLVM_CONFIG_EXECUTABLE
        NAMES llvm-config-${LLVM_MIN_VERSION_TEXT} llvm-config
        PATHS /opt/local/bin /usr/local/bin
        HINTS
        ${LLVM_INSTALL_PREFIX}/bin
        $ENV{LLVM_DIR}/bin
        C:/LLVM/bin
        C:/LLVM6.0.1/bin
        C:/llvm/bin
        C:/llvm6.0.1/bin
        D:/LLVM/bin
        D:/LLVM6.0.1/bin
        D:/llvm/bin
        D:/llvm6.0.1/bin
        ~/LLVM/bin
        ~/LLVM6.0.1/bin
        ~/llvm/bin
        ~/llvm6.0.1/bin
        DOC "llvm-config executable"
        )


if (LLVM_CONFIG_EXECUTABLE)
    message(STATUS "LLVM llvm-config found at: ${LLVM_CONFIG_EXECUTABLE}")

    #    execute_process(
    #            COMMAND chmod -x ${LLVM_CONFIG_EXECUTABLE}
    #            #            OUTPUT_VARIABLE LLVM_VERSION
    #            #            OUTPUT_STRIP_TRAILING_WHITESPACE
    #    )

    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --version
            OUTPUT_VARIABLE LLVM_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    #    message(STATUS "LLVM_VERSION ${LLVM_VERSION}")

    # Version Info
    execute_process(COMMAND ${LLVM_CONFIG_EXECUTABLE} --version OUTPUT_VARIABLE LLVM_STRING_VERSION)
    string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)([^\n\r\t ]*)" "\\1" LLVM_VERSION_MAJOR "${LLVM_VERSION}")
    string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)([^\n\r\t ]*)" "\\2" LLVM_VERSION_MINOR "${LLVM_VERSION}")
    string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)([^\n\r\t ]*)" "\\3" LLVM_VERSION_PATCH "${LLVM_VERSION}")
#    message(STATUS "LLVM_VERSION: ${LLVM_VERSION_MAJOR}.${LLVM_VERSION_MINOR}.${LLVM_VERSION_PATCH}")

    execute_process(COMMAND ${LLVM_CONFIG_EXECUTABLE} --bindir OUTPUT_VARIABLE LLVM_BIN_DIR)
    execute_process(COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir OUTPUT_VARIABLE LLVM_LIB_DIR)
    #    MESSAGE(STATUS "LLVM_BIN_DIR: " ${LLVM_BIN_DIR})
    #    MESSAGE(STATUS "LLVM_LIB_DIR: " ${LLVM_LIB_DIR})

    # Include Dir
    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --includedir
            OUTPUT_VARIABLE LLVM_INCLUDE_DIRS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    #    message(STATUS "LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}")

    # Lib Dir
    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --libdir
            OUTPUT_VARIABLE LLVM_LIBRARY_DIRS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    #    message(STATUS "LLVM_LIBRARY_DIRS:  ${LLVM_LIBRARY_DIRS}")

    # System libs
    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --system-libs
            OUTPUT_VARIABLE LLVM_SYSTEM_LIBS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
#    message(STATUS "LLVM_SYSTEM_LIBS:  ${LLVM_SYSTEM_LIBS}")
    # For some reason, the return value for system-libs for macos is of the
    # form '-llib1 -llib2 ...", but the return value for system-libs for windows
    # is of the form "lib1.lib lib2.lib ..." which doesn't work the same way.
    # The only system it was failing for was macos, so we'll just set it for that.
    if (APPLE)
        set(LLVM_SYSTEM_LIBS_THISOS ${LLVM_SYSTEM_LIBS})
    endif ()
#    message(STATUS "LLVM_SYSTEM_LIBS_THISOS:  ${LLVM_SYSTEM_LIBS_THISOS}")

    # C++ Flags, strip out stuff that CMake build adds
    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --cxxflags
            OUTPUT_VARIABLE LLVM_FLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT LLVM_FLAGS)
        message(STATUS "LLVM_INSTALL_PREFIX ${LLVM_INSTALL_PREFIX}")
        message(FATAL_ERROR "The LLVM_FLAGS variable does not exist. This means\
that the llvm-config binary could not be executed by cmake to query for\
LLVM flags. Sometimes this happens because you do not have execution permissions\
on llvm-config. Other times this happens when LLVM_INSTALL_PREFIX does not point to the\
correct location. ")
    endif ()
#    message(STATUS "LLVM_FLAGS ${LLVM_FLAGS}")
    # strip this from llvm's version, we should add this ourselves in
    # production mode to main CFLAGS
    STRING(REPLACE "-DNDEBUG" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-D_DEBUG" "" LLVM_FLAGS ${LLVM_FLAGS})
    # remove optimization from flags, our cmake build will decide what optimization to use
    STRING(REGEX REPLACE "-O[0-9]" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-fno-rtti" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-fno-exceptions" "" LLVM_FLAGS ${LLVM_FLAGS})
    # this causes problems as most other libs compiled without this.
    # should be OK linking to LLVM as this just results in a slightly larger lib (I think, I hope...)
    string(REPLACE "-fvisibility-inlines-hidden" "" LLVM_FLAGS ${LLVM_FLAGS})

    # remove extra warnings that the llvm-config added
    STRING(REPLACE "-Wcast-qual" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Woverloaded-virtual" "" LLVM_FLAGS ${LLVM_FLAGS})

    # some LLVMs add these options
    # -Wall -W -Wno-unused-parameter -Wwrite-strings -Wno-missing-field-initializers -pedantic
    # -Wno-long-long -Wno-uninitialized -Wnon-virtual-dtor


    STRING(REPLACE "-Wall" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-W " "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wno-unused-parameter" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wwrite-strings" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wno-missing-field-initializers" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-pedantic" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wno-long-long" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wno-uninitialized" "" LLVM_FLAGS ${LLVM_FLAGS})
    STRING(REPLACE "-Wnon-virtual-dtor" "" LLVM_FLAGS ${LLVM_FLAGS})


    #    MESSAGE(STATUS "LLVM_FLAGS: " ${LLVM_FLAGS})


    # link libraries, currently only need core, jit and native.
    # TODO: in future, replace this with something like LLVM_CORE_LIBS, LLVM_JIT_LIBS...
    # Replaced this with manually set variable LLVM_LIBRARIES
    #   The advantage is that we can pass these in as targets to
    # the llvm build and not have to build everything.
    execute_process(
            COMMAND ${LLVM_CONFIG_EXECUTABLE} --libfiles core mcjit native Core OrcJIT Support native
            OUTPUT_VARIABLE LLVM_LIBRARIES
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (WIN32)
        # llvm-config returns .a library names, even on Windows, so manually convert .a -> .lib and remove lib prefix.
        STRING(REGEX REPLACE "lib([^/\\]*)\\.a" "\\1.lib" LLVM_LIBRARIES ${LLVM_LIBRARIES})
    endif ()
    # we get a space sep list from llvm-config, make it a cmake ; separated list.
    STRING(REGEX REPLACE "[\n\t\r ]+" ";" LLVM_LIBRARIES ${LLVM_LIBRARIES})
    #    message(STATUS "LLVM_LIBRARIES: ${LLVM_LIBRARIES}")

    # starting with LLVM 3.4 (at least on Ubuntu) it requres functions in
    # ncurses for console IO formatting. So, we find ncurses here.
    if (UNIX)
        #message("UNIX true")
        #        message("LLVM VERSION >= 3.4, looking for curses library")
        # sudo apt-get install libncurses5-dev libncursesw5-dev
        find_package(Curses REQUIRED)
        #        message(STATUS "curses: ${CURSES_FOUND}")
        #        message(STATUS "curdir: ${CURSES_INCLUDE_DIR}")
        #        message(STATUS "curlib: ${CURSES_LIBRARIES}")
        #        message(STATUS "LLVM_LIBRARIES: ${LLVM_LIBRARIES}")

        set(LLVM_LIBRARIES "${LLVM_LIBRARIES};${CURSES_LIBRARIES}")


        # LLVM 3.5 seems to require zlib, at least on OSX 10.9.
        # no big deal to just add it on UNIX in general as it already there.
        if (LLVM_VERSION_MINOR GREATER 4)
            message("LLVM > 3.4, looking for zlib")
            find_package(ZLIB REQUIRED)
            set(LLVM_LIBRARIES "${LLVM_LIBRARIES};${ZLIB_LIBRARY}")
        endif ()

    endif ()


    if (LLVM_INCLUDE_DIRS)
        set(LLVM_FOUND TRUE)
        message(STATUS "Found LLVM: ${LLVM_INCLUDE_DIRS}")
    endif (LLVM_INCLUDE_DIRS)


else (LLVM_CONFIG_EXECUTABLE)
    message(STATUS "Could NOT find LLVM executable")
endif (LLVM_CONFIG_EXECUTABLE)

if (NOT LLVM_FOUND AND LLVM_FIND_REQUIRED)
    message(FATAL_ERROR "Could NOT find LLVM")
endif (NOT LLVM_FOUND AND LLVM_FIND_REQUIRED)
