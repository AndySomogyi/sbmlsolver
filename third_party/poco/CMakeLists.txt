cmake_minimum_required(VERSION 2.6)
project(poco)

add_definitions(
    -DPCRE_STATIC
    -DPOCO_STATIC
)

OPTION(BUILD_POCO_NET "Build poco net library" ON)

#if(${MSVC})
if(WIN32)
    add_definitions(
        #-DUSRDLL
        -DWIN32
        -D_WIN32
        )
endif()

add_subdirectory(Foundation)
add_subdirectory(XML)

if (BUILD_POCO_NET)
add_subdirectory(Net)
endif()
