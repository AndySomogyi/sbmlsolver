set(target rr_c_api)

set(C_API_INCLUDE_ROOT "${RR_ROOT}/Wrappers/C")

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(   
${C_API_INCLUDE_ROOT}
${RR_ROOT}/source
${THIRD_PARTY_INSTALL_FOLDER}/include
)

#The C-api is built as a shared library..
add_library(${target} SHARED
rr_c_api.cpp
rr_dll_entry_point.cpp
rr_c_api_support.cpp
)

set_property(   TARGET ${target} 
                PROPERTY  COMPILE_DEFINITIONS 
                          EXPORT_RR_C_API
                          STATIC_RR             #when linking with static lib, no declspec shuld be present..
                          LIBSBML_STATIC
					      LIBLAX_STATIC
                          STATIC_PUGI
                          STATIC_LIBSTRUCT
)

if(${MSVC})
set (CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/c_api_vs.def\"")
endif()

#Link C_api dll statically to roadrunner and libsbml
if(WIN32)
 target_link_libraries ( ${target}
 roadrunner-static
 )
else() #On linux, link with shared version of roadrunner
target_link_libraries ( ${target}
roadrunner
)
endif()

add_subdirectory(Apps)
if(INSTALL_EXAMPLES)
    add_subdirectory(Examples)
endif()

if(BUILD_TESTS)
    add_subdirectory(Testing)
endif()

#=============== INSTALL =====================
install(   FILES ${target}.h rr_c_types.h rr_c_api_exporter.h				 					
           DESTINATION include/c_api  	
           COMPONENT c_api)

install(   
           TARGETS ${target}
           RUNTIME DESTINATION bin COMPONENT c_api
           ARCHIVE DESTINATION lib COMPONENT c_api
           LIBRARY DESTINATION lib COMPONENT c_api
)

