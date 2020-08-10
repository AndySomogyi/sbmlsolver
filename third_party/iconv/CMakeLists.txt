cmake_minimum_required(VERSION 3.0.0)
project(libiconv C)

#if (BUILD_SHARED_LIBS)
#    set(LIBICONV_DYNAMIC "1 /*LIBICONV_DYNAMIC*/")
#else ()
#    set(LIBICONV_DYNAMIC "0 /*LIBICONV_DYNAMIC*/")
#endif ()
set(LIBICONV_DYNAMIC "0 /*LIBICONV_DYNAMIC*/")

set(CMAKE_STATIC_LIBRARY_PREFIX)
set(CMAKE_SHARED_LIBRARY_PREFIX)

set(CONFIG_H_IN "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in")
set(CONFIG_H "${CMAKE_CURRENT_BINARY_DIR}/config.h")

if (WIN32)
    set(HAVE_WCHAR_T "1 /*HAVE_WCHAR_T*/")
    set(USE_MBSTATE_T "0 /*USE_MBSTATE_T*/")
    set(BROKEN_WCHAR_H "0 /*BROKEN_WCHAR_H*/")
    set(HAVE_VISIBILITY "0 /*HAVE_VISIBILITY*/")
    set(ICONV_CONST "")


    configure_file(${CONFIG_H_IN} ${CONFIG_H})
    file(READ "${CONFIG_H}" _contents)
    string(REPLACE "#undef HAVE_WORKING_O_NOFOLLOW" "#define HAVE_WORKING_O_NOFOLLOW 0" _contents "${_contents}")
    string(REPLACE "#undef HAVE_MBRTOWC" "#define HAVE_MBRTOWC 1" _contents "${_contents}")
    string(REPLACE "#undef HAVE_MBSINIT" "#define HAVE_MBSINIT 1" _contents "${_contents}")
    string(REPLACE "#undef HAVE_WCRTOMB" "#define HAVE_WCRTOMB 1" _contents "${_contents}")
    string(REPLACE "#undef ICONV_CONST" "#define ICONV_CONST ${ICONV_CONST}" _contents "${_contents}")
    string(REPLACE "#undef EILSEQ" "" _contents "${_contents}")
    string(REPLACE "#undef WORDS_LITTLEENDIAN" "#define WORDS_LITTLEENDIAN 1" _contents "${_contents}")
    string(REPLACE "#undef ENABLE_RELOCATABLE" "#define ENABLE_RELOCATABLE 1" _contents "${_contents}")
    file(WRITE "${CONFIG_H}" "${_contents}")


    #    configure_file(
    #            ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/localcharset.h.build.in
    #            ${CMAKE_CURRENT_BINARY_DIR}/libcharset/include/localcharset.h
    #    )
    #    configure_file(
    #            ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/libcharset.h.in
    #            ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/libcharset.h
    #    )
    #    configure_file(
    #            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.build.in
    #            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.build
    #    )

    #    configure_file(
    #            ${CMAKE_CURRENT_BINARY_DIR}/include/iconv.h
    #            iconv.h
    #    )
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.build.in
            ${CMAKE_CURRENT_BINARY_DIR}/include/iconv.h
    )
    if (MSVC)
        add_compile_options(/wd4018)
    endif ()

    add_definitions(
            -D_CRT_SECURE_NO_WARNINGS
            -DLIBDIR=${LIBDIR}
            -DINSTALLDIR=${INSTALLDIR}
            -DNO_XMALLOC=1
            -DENABLE_RELOCATABLE=1
            -DIN_LIBRARY=1
    )
    # We want static - so no flags for roadrunner (ciaran welsh)
    #    if (BUILD_SHARED_LIBS)
    #        add_definitions(
    #                -DBUILDING_LIBCHARSET=1
    #                -DBUILDING_LIBICONV=1
    #                -DBUILDING_DLL=1)
    #    endif ()
    #
else (WIN32)
    #    if (BUILD_SHARED_LIBS)
    #        set(STATIC_SHARED --enable-shared --disable-static)
    #    else ()
    #        set(STATIC_SHARED --disable-shared --enable-static)
    #    endif ()

    set(STATIC_SHARED --disable-shared --enable-static)

    ##########################################################################
    # Note from ciaran welsh
    # ---------------------------
    # It turned out to be quite difficult to get iconv working on both
    # windows and linux. I took the sources from vcpkg which worked
    # straight away on windows but not with linux. Their strategy for
    # linux was the below commented out code, which I've left to keep a record
    # of it. On linux, they call out to autotools and use ./configure to create
    # the config.h. This became a nuicence when building for linux and windows
    # using the same source tree but different build tree (clion supports wsl toolchain
    # making this viable), since configure_file always wrote to the source tree and overwrite
    # essential configurations for each tool chain.
    #
    # My first strategy was to move the configured files into
    # the build tree instead. This clearly would work if we get it right,
    # however it proved very fidly and ./configure actually takes a very long time
    # for iconv. My alternative strategy was to essentially copy what vcokg authors
    # have done for msvc builds above: hard code the desired variables that
    # are to be passed to iconv's config.h and do some post processing until it works.
    # This strategy worked almost straight away, but has the caveat of reduced flexibility.
    # I'm writing this note because future maintainers of roadrunner should be aware that
    # we may need some conditionals on the variables defined below in order to get
    # iconv to compile on other systems.
    ##########################################################################

    #    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/autoconf)
    #    message(STATUS "running ./configure for iconv")
    #    set(_bindir_autoconf_iconv_h "${CMAKE_CURRENT_BINARY_DIR}/autoconf/config.h")
    #    if (NOT EXISTS ${_bindir_autoconf_iconv_h})
    #        # we only need to run this once, and it takes ages.
    #        # so check for existance of ${_bindir_autoconf_iconv_h}
    #        # if it exists then we assume configure has been run by a previous
    #        # execution of cmake.
    #        execute_process(
    #                COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/configure --enable-relocatable ${STATIC_SHARED} --without-libiconv-prefix --without-libintl-prefix
    #                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/autoconf
    #        )
    #    endif ()

    #    set(DLL_VARIABLE       "0        /*DLL_VARIABLE*/")
    set(HAVE_VISIBILITY "1        /*HAVE_VISIBILITY*/")
    set(ICONV_CONST "const    /*ICONV_CONST*/")
    set(USE_MBSTATE_T "1        /*USE_MBSTATE_T*/")
    set(BROKEN_WCHAR_H "0        /*BROKEN_WCHAR_H*/")
    set(HAVE_WCHAR_T "0        /*HAVE_WCHAR_T*/")

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
            ${CMAKE_CURRENT_BINARY_DIR}/config.h
    )

    configure_file(${CONFIG_H_IN} ${CONFIG_H})
    file(READ "${CONFIG_H}" _contents)
    string(REPLACE "#undef ICONV_CONST" "#define ICONV_CONST ${ICONV_CONST}" _contents "${_contents}")
    string(REPLACE "#undef EILSEQ" "" _contents "${_contents}")
    file(WRITE "${CONFIG_H}" "${_contents}")

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/export.h.in
            ${CMAKE_CURRENT_BINARY_DIR}/libcharset/include/export.h
    )

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.build.in
            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.in
    )

    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.in
            ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h
    )


    #    configure_file(
    #            ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    #            ${CMAKE_CURRENT_BINARY_DIR}/config.h
    #    )

endif ()

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

# We use binary directory as it will work for multiple tool chains.
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/localcharset.h.build.in
        ${CMAKE_CURRENT_BINARY_DIR}/libcharset/include/localcharset.h
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/libcharset.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/libcharset/include/libcharset.h
)


set(LIBICHARSET_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/libcharset/include")
set(LIBICONV_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${LIBICHARSET_INCLUDE_DIR}
        ${LIBICONV_INCLUDE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/autoconf # for config.h
)
set(GNULIB_LOCAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gnulib-local/lib")

set(LIBICONV_INCLUDE_DIR ${LIBICONV_INCLUDE_DIR} PARENT_SCOPE)

add_library(libcharset STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/lib/localcharset.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/lib/relocatable-stub.c
        )
target_include_directories(libcharset PUBLIC
        ${LIBICHARSET_INCLUDE_DIR}
        ${LIBICONV_INCLUDE_DIR}
        )
add_library(libiconv-static STATIC
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/iconv.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib/relocatable.c")

if (WIN32)
    target_compile_definitions(libiconv-static PRIVATE
            -Dset_relocation_prefix=libiconv_set_relocation_prefix
            -Drelocate=libiconv_relocate -Drelocate2=libiconv_relocate2
            )
endif ()
target_link_libraries(libiconv-static PRIVATE libcharset)
add_dependencies(libiconv-static libcharset)
target_include_directories(libiconv-static PUBLIC
        ${LIBICHARSET_INCLUDE_DIR}
        ${LIBICONV_INCLUDE_DIR}
        ${GNULIB_LOCAL_INCLUDE_DIR}
        )


#install(TARGETS libcharset libiconv-static EXPORT unofficial-iconv-targets
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        )
#
#if (NOT DISABLE_INSTALL_HEADERS)
#    install(FILES
#            ${CMAKE_CURRENT_BINARY_DIR}/iconv.h
#            ${CMAKE_CURRENT_BINARY_DIR}/localcharset.h
#            DESTINATION include
#            )
#
#endif ()

#install(
#        EXPORT unofficial-iconv-targets
#        FILE unofficial-iconv-config.cmake
#        NAMESPACE unofficial::iconv::
#        DESTINATION share/unofficial-iconv
#)










