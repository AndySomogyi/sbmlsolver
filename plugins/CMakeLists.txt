cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

if(${BUILD_PLUGINS} STREQUAL ON)

include_directories(
	../wrappers/C
)

OPTION (RRP_BUILD_ADD_NOISE_PLUGIN		        "Build add noise plugin"	                ON)
OPTION (RRP_BUILD_LEVENBERG_MARQUARDT_PLUGIN	"Build Levenberg-Marquardt plugin"	        ON)
OPTION (RRP_BUILD_EXAMPLES   	                "Build Plugin Examples"                     OFF)

add_subdirectory(source)

if(${RRP_BUILD_ADD_NOISE_PLUGIN} STREQUAL ON)
add_subdirectory(add_noise)
endif()

if(${RRP_BUILD_LEVENBERG_MARQUARDT_PLUGIN} STREQUAL ON)
add_subdirectory(lmfit)
endif()

if(${RRP_BUILD_EXAMPLES} STREQUAL ON)
add_subdirectory(examples)
endif()

endif()

add_subdirectory(wrappers)

install(
    FILES README.txt
    DESTINATION plugins
)
