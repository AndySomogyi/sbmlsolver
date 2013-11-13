cmake_minimum_required(VERSION 2.8)
project(rr_plugins)

if(${BUILD_PLUGINS} STREQUAL ON)

include_directories(
	../wrappers/C
)

OPTION (RRP_BUILD_ADD_NOISE_PLUGIN		        "Build add noise plugin"	                ON)
OPTION (RRP_BUILD_LEVENBERG_MARQUARDT_PLUGIN	"Build Levenberg-Marquardt plugin"	        ON)
#OPTION (RRP_BUILD_C_PLUGIN_DEMO	                "Build demo of a plugin written in C"       ON)
OPTION (RRP_BUILD_CPP_PLUGIN_DEMO	            "Build demo of a plugin written in C++"     ON)

add_subdirectory(source)

if(${RRP_BUILD_ADD_NOISE_PLUGIN} STREQUAL ON)
add_subdirectory(add_noise)
endif()

if(${RRP_BUILD_LEVENBERG_MARQUARDT_PLUGIN} STREQUAL ON)
add_subdirectory(lmfit)
endif()

#if(${RRP_BUILD_C_PLUGIN_DEMO} STREQUAL ON)
#add_subdirectory(c_plugin_demo)
#endif()

if(${RRP_BUILD_CPP_PLUGIN_DEMO} STREQUAL ON)
add_subdirectory(cpp_plugin_demo)
endif()

endif()

add_subdirectory(wrappers)

