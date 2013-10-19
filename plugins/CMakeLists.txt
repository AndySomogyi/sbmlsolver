cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

OPTION (BUILD_ADD_NOISE_PLUGIN		        "Build add noise plugin"	        ON)
OPTION (BUILD_LEVENBERG_MARQUARDT_PLUGIN	"Build add noise plugin"	        ON)

add_subdirectory(plugin_manager)

if(${BUILD_ADD_NOISE_PLUGIN})
add_subdirectory(add_noise)
endif()


if(${BUILD_LEVENBERG_MARQUARDT_PLUGIN})
add_subdirectory(lmfit)
endif()
