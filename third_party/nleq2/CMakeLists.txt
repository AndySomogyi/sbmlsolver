CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(nleq2)

set(target nleq2)

set(headers nleq2.h f2c_nleq.h)

set(sources
        nleq2.c
        linalg_nleq2.c
        wnorm.c
        zibconst.c
        zibmon.c
        zibsec.c
        )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(${target}-static STATIC ${sources})

if (WIN32)
    set_property(TARGET ${target}-static
            PROPERTY COMPILE_DEFINITIONS
            WIN32
            STATIC_NLEQ2
            )
else ()
    set_property(TARGET ${target}-static
            PROPERTY COMPILE_DEFINITIONS
            STATIC_NLEQ2
            )

endif ()

target_link_libraries(${target}-static f2c)



#Install required files
install(FILES ${headers}
        DESTINATION include/nleq2)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/nleq2)
file(COPY ${headers} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/nleq2)


install(TARGETS ${target}-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

