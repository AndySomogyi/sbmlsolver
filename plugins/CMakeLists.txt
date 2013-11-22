cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

if(${BUILD_PLUGINS} STREQUAL ON)

include_directories(
	../wrappers/C
)

OPTION (BUILD_ADD_NOISE_PLUGIN		        "Build add noise plugin"	        ON)
OPTION (BUILD_LEVENBERG_MARQUARDT_PLUGIN	"Build add noise plugin"	        ON)

add_subdirectory(source)

if(${BUILD_ADD_NOISE_PLUGIN} STREQUAL ON)
add_subdirectory(add_noise)
endif()


if(${BUILD_LEVENBERG_MARQUARDT_PLUGIN} STREQUAL ON)
add_subdirectory(lmfit)
endif()

endif()

add_subdirectory(wrappers)

