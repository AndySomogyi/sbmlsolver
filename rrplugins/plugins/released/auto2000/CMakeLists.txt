# M Totte Karlsson
# J Kyle Medley

project(auto2000)
set(target tel_auto2000)

add_subdirectory(libAuto)
add_subdirectory(libAutoTelluriumInterface)

#if (TLP_BUILD_EXAMPLES)
#  add_subdirectory(examples)
#endif()


set(RRP_AUTO2000_SOURCES
        telAutoPlugin.cpp
        telAutoWorker.cpp
        telAutoDataParser.cpp
        auto_utils.cpp
        auto2000_doc.cpp

        telAutoPlugin.h
        telAutoWorker.h
        telAutoDataParser.h
        auto_utils.h
        auto2000_doc.h
        )


add_library(tel_auto2000 MODULE ${RRP_AUTO2000_SOURCES})
target_compile_definitions(${target} PRIVATE
        -DEXPORT_TEL_PLUGIN
        -DEXPORT_ADD_NOISE
        -DSTATIC_AUTO
        -DSTATIC_TEL_AUTO
        -DEXPORT_AUTO2000
        )


target_include_directories(${target}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libAutoTelluriumInterface
        $<TARGET_PROPERTY:roadrunner,INCLUDE_DIRECTORIES>
        )

target_link_libraries(${target}
        telpluginsBaseClass
        libTelluriumAutoInterface-static
        libAuto-static
        rr-libstruct::rr-libstruct-static
        roadrunner
        ${RRPLUGINS_COMMON_SHARED_LIBS}
        )

add_dependencies(${target} libTelluriumAutoInterface-static libAuto-static)
if (UNIX)
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "$ORIGIN/./")
endif ()

install(TARGETS ${target}
        DESTINATION ${RRP_PLUGINS_DIR}
        COMPONENT plugins
        )
