cmake_minimum_required(VERSION 2.8)
set(target plugin_info)

add_executable(${target} 
main.cpp 
)

add_definitions(
)

if(WIN32)
add_definitions(
-DWIN32
)

target_link_libraries (${target} 
telplugins
)
endif()

if(UNIX)
add_definitions(
-DPOCO_STATIC
)
target_link_libraries (${target} 
telplugins
lapack
blas
f2c
poco_foundation-static.a
)
endif()


install (TARGETS ${target} 		
DESTINATION bin 
COMPONENT apps
)
