set(target roadrunner_c_api)

#The C-api is built as a shared library..
add_library(${target} SHARED
        rrcStringList.cpp
        rrArrayList.cpp
        rrArrayListItem.cpp
        rrArrayListItemBase.cpp
        rrc_api.cpp
        rrc_logging_api.cpp
        rrc_nom_api.cpp
        rrc_libstruct_api.cpp
        rrc_utilities.cpp
        rrc_cpp_support.cpp
        rrcStringList.h
        rrArrayList.h
        rrArrayListItem.h
        rrArrayListItemBase.h
        rrc_api.h
        rrc_logging_api.h
        rrc_nom_api.h
        rrc_libstruct_api.h
        rrc_utilities.h
        rrc_cpp_support.h
        )

target_include_directories(${target} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/C
        ${CMAKE_SOURCE_DIR}/source
        ${INCLUDE_DIRECTORIES}
        ${RR_GENERATED_HEADER_PATH}
        )

target_link_libraries(${target} PRIVATE
        $<TARGET_FILE:roadrunner-static>
        D:/rrbuild/roadrunner/third_party/rr-libstruct/install-MSVC/lib/rr-libstruct-static.lib
        D:/rrbuild/roadrunner/third_party/libSBML-5.18.1-experimental-Source/install-MSVC/lib/libsbml-static.lib
        ${LINK_LIBRARIES}
        )
add_dependencies(${target} roadrunner-static)

if (WIN32)
    set_property(TARGET ${target}
            PROPERTY COMPILE_DEFINITIONS
            EXPORT_RRC
            LIBSBML_STATIC
            LIBLAX_STATIC
            POCO_STATIC
            STATIC_LIBSTRUCT
            STATIC_RR
            )
    # set msvc runtime
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    else ()
        message(FATAL_ERROR "Only Debug and Release configurations are supported")
    endif ()

    target_link_libraries(${target} PRIVATE Ws2_32)
endif ()
#add_subdirectory(Apps)
#add_subdirectory(ApiTest)
#if (INSTALL_EXAMPLES)
#    add_subdirectory(Examples)
#endif ()
#
#if (BUILD_TEST_TOOLS)
#    add_subdirectory(Testing)
#endif ()

#=============== install =====================
#Glob all header files
FILE(GLOB headers rrc*.h)

install(
        FILES
        ${headers}
        DESTINATION include/rr/C COMPONENT c_api
)

install(
        TARGETS ${target}
        RUNTIME DESTINATION bin COMPONENT c_api
        ARCHIVE DESTINATION lib COMPONENT c_api
        LIBRARY DESTINATION lib COMPONENT c_api
)
