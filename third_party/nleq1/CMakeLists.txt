CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(nleq1)

set(target nleq1)

set(headers nleq1.h f2c_nleq.h)

set(sources
        nleq1.c
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
            STATIC_NLEQ1
            )
else ()
    set_property(TARGET ${target}-static
            PROPERTY COMPILE_DEFINITIONS
            STATIC_NLEQ1
            )

endif ()

target_link_libraries(${target}-static f2c)



#Install required files
install(FILES ${headers}
        DESTINATION include/nleq1)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/nleq1)
file(COPY ${headers} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/nleq1)
install(TARGETS ${target}-static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

