# M Totte Karlsson
# J Kyle Medley
set(target libTelluriumAutoInterface-static)

set(RRP_LIB_AUTO_INTERFACE_SRC
        telAutoConstants.cpp
        telAutoTelluriumInterface.cpp
        telAutoUtils.cpp
        telAutoConstants.h
        telAutoTelluriumInterface.h
        telAutoUtils.h
        )

add_library(${target} STATIC ${RRP_LIB_AUTO_INTERFACE_SRC})

if (WIN32)
    target_compile_definitions(${target}
            -DSTATIC_AUTO
            -DSTATIC_TEL_AUTO
            )
endif ()

target_link_libraries(${target} PUBLIC
        telplugins_common
        telplugins_core
        rr-libstruct::rr-libstruct-static
        )
#target_include_directories(${target} PUBLIC $<TARGET_PROPERTY:roadrunner-static,INCLUDE_DIRECTORIES>)
install(
        TARGETS libTelluriumAutoInterface-static
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)

