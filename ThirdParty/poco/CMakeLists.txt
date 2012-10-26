cmake_minimum_required(VERSION 2.6)
project(poco)

set(target poco)

if(${MSVC})
add_definitions(
-DFoundationEXPORTS 
-DUSRDLL
-D_WINDOWS
-DWIN32
-DPOCO_STATIC
-DPCRE_STATIC
)
endif()

add_subdirectory(Foundation)
