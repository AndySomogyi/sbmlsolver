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

if (WIN32)
    set(HAVE_WCHAR_T "1 /*HAVE_WCHAR_T*/")
    set(USE_MBSTATE_T "0 /*USE_MBSTATE_T*/")
    set(BROKEN_WCHAR_H "0 /*BROKEN_WCHAR_H*/")
    set(HAVE_VISIBILITY "0 /*HAVE_VISIBILITY*/")
    set(ICONV_CONST "")

    configure_file(config.h.in config.h)
    file(READ "${CMAKE_CURRENT_BINARY_DIR}/config.h" _contents)
    string(REPLACE "#undef HAVE_WORKING_O_NOFOLLOW" "#define HAVE_WORKING_O_NOFOLLOW 0" _contents "${_contents}")
    string(REPLACE "#undef HAVE_MBRTOWC" "#define HAVE_MBRTOWC 1" _contents "${_contents}")
    string(REPLACE "#undef HAVE_MBSINIT" "#define HAVE_MBSINIT 1" _contents "${_contents}")
    string(REPLACE "#undef HAVE_WCRTOMB" "#define HAVE_WCRTOMB 1" _contents "${_contents}")
    string(REPLACE "#undef ICONV_CONST" "#define ICONV_CONST ${ICONV_CONST}" _contents "${_contents}")
    string(REPLACE "#undef EILSEQ" "" _contents "${_contents}")
    string(REPLACE "#undef WORDS_LITTLEENDIAN" "#define WORDS_LITTLEENDIAN 1" _contents "${_contents}")
    string(REPLACE "#undef ENABLE_RELOCATABLE" "#define ENABLE_RELOCATABLE 1" _contents "${_contents}")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/config.h" "${_contents}")

    configure_file(libcharset/include/localcharset.h.build.in localcharset.h)
    configure_file(libcharset/include/libcharset.h.in libcharset.h)
    configure_file(include/iconv.h.build.in iconv.h)

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
else ()
    #    if (BUILD_SHARED_LIBS)
    #        set(STATIC_SHARED --enable-shared --disable-static)
    #    else ()
    #        set(STATIC_SHARED --disable-shared --enable-static)
    #    endif ()

    set(STATIC_SHARED --disable-shared --enable-static)

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/autoconf)

    message(STATUS "running ./configure for iconv")
    execute_process(
            COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/configure --enable-relocatable ${STATIC_SHARED} --without-libiconv-prefix --without-libintl-prefix
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/autoconf
    )

    # this was here before
    configure_file(${CMAKE_CURRENT_BINARY_DIR}/autoconf/lib/config.h config.h @ONLY)
    configure_file(${CMAKE_CURRENT_BINARY_DIR}/autoconf/include/iconv.h iconv.h @ONLY)
    configure_file(${CMAKE_CURRENT_BINARY_DIR}/autoconf/libcharset/include/localcharset.h localcharset.h @ONLY)

    # I added these (ciaran welsh)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/lib/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/lib/config.h @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/iconv.h @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/localcharset.h.in ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/localcharset.h @ONLY)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/libcharset.h.in ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include/libcharset.h @ONLY)
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set(LIBICHARSET_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libcharset/include")
set(LIBICONV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(GNULIB_LOCAL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gnulib-local/lib")

set(LIBICHARSET_INCLUDE_DIR ${LIBICHARSET_INCLUDE_DIR} PARENT_SCOPE)
set(LIBICONV_INCLUDE_DIR ${LIBICONV_INCLUDE_DIR} PARENT_SCOPE)

add_library(libcharset STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/lib/localcharset.c
        ${CMAKE_CURRENT_SOURCE_DIR}/libcharset/lib/relocatable-stub.c
        )
target_include_directories(libcharset PUBLIC #$<INSTALL_INTERFACE:include>
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
target_include_directories(libiconv-static PUBLIC #$<INSTALL_INTERFACE:include>
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
#set(ICONV_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/iconv")
#set(ICONV_INCLUDE_DIR "${ICONV_INCLUDE_DIR}" PARENT_SCOPE)
#file(MAKE_DIRECTORY "${ICONV_INCLUDE_DIR}")
#file(COPY
#        ${CMAKE_CURRENT_BINARY_DIR}/iconv.h
#        ${CMAKE_CURRENT_BINARY_DIR}/localcharset.h
#        DESTINATION "${ICONV_INCLUDE_DIR}"
#        )









