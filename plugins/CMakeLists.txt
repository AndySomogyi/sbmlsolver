cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

if(${BUILD_PLUGINS} STREQUAL ON)

set(PLUGIN_MANAGER_INCLUDE_ROOT "${RR_ROOT}/plugins")

include_directories(
	../wrappers/C
	${PLUGIN_MANAGER_INCLUDE_ROOT}
)

OPTION (BUILD_ADD_NOISE_PLUGIN		        "Build add noise plugin"	        ON)
OPTION (BUILD_LEVENBERG_MARQUARDT_PLUGIN	"Build add noise plugin"	        ON)

add_subdirectory(source)

if(${BUILD_ADD_NOISE_PLUGIN})
add_subdirectory(add_noise)
endif()


if(${BUILD_LEVENBERG_MARQUARDT_PLUGIN})
add_subdirectory(lmfit)
endif()

endif()
