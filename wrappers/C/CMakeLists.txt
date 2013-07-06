set(target rrc_api)

set(C_API_INCLUDE_ROOT "${RR_ROOT}/wrappers/C")

include_directories(${RR_GENERATED_HEADER_PATH}) #we have to do this to get the generated header

include_directories(   
	${C_API_INCLUDE_ROOT}
	${RR_ROOT}/source
	${THIRD_PARTY_INSTALL_FOLDER}/include
	)

#The C-api is built as a shared library..
add_library(${target} SHARED
	rrc_api
	rrc_plugin_api
	rrc_jobs_api
	rrc_parameter_api
	rrc_logging_api
	rrc_nom_api
	rrc_libstruct_api
	rrc_utilities
	rrc_cpp_support
	)

if(${MSVC})
    set (CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} \"/DEF:${CMAKE_CURRENT_SOURCE_DIR}/rrc_api.def\"")
endif()

#Link C_api dll statically to roadrunner and libsbml
if(WIN32)
    set_property(TARGET ${target} 
        PROPERTY  COMPILE_DEFINITIONS 
        EXPORT_RRC
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        POCO_STATIC
        STATIC_LIBSTRUCT
        STATIC_RR             
        )
    
    target_link_libraries ( ${target}
        roadrunner-static
        )
    
else() #Unix
	
    # OSX seems to link fine without this flag
    if(NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
    endif()
    
    set_property(TARGET ${target} 
        PROPERTY  COMPILE_DEFINITIONS 
        LIBSBML_STATIC
        LIBLAX_STATIC
        STATIC_PUGI
        STATIC_LIBSTRUCT
        )
    
    target_link_libraries ( ${target} 
        roadrunner-static
        rr-libstruct-static
        nleq-static        
        sbml-static
        lapack
        blas
        f2c
		pthread
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
install(
	FILES 
    ${target}.h 
    rrc_utilities.h 
    rrc_types.h 
    rrc_exporter.h				 					
	
	DESTINATION include/c_api COMPONENT c_api
	)

install(   
	TARGETS ${target}
	RUNTIME DESTINATION bin COMPONENT c_api
	ARCHIVE DESTINATION lib COMPONENT c_api
	LIBRARY DESTINATION lib COMPONENT c_api
	)

