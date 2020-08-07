set(target roadrunner_c_api)

set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${RR_CAPI_INCLUDE_DIR}")

#The C-api is built as a shared library..
add_library(${target} SHARED
        rrcStringList.cpp
        rrArrayList.cpp
        rrArrayListItem.cpp
        rrArrayListItemBase.cpp
        rrc_api.cpp
        rrc_logging_api.cpp
        rrc_nom_api.cpp
        rrc_libstruct_api.cpp
        rrc_utilities.cpp
        rrc_cpp_support.cpp
        rrcStringList.h
        rrArrayList.h
        rrArrayListItem.h
        rrArrayListItemBase.h
        rrc_api.h
        rrc_logging_api.h
        rrc_nom_api.h
        rrc_libstruct_api.h
        rrc_utilities.h
        rrc_cpp_support.h
        )

target_include_directories(${target} PRIVATE
        ${RR_CAPI_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/source
        ${INCLUDE_DIRECTORIES}
        ${RR_GENERATED_HEADER_PATH}
        )

target_link_libraries(${target} PRIVATE roadrunner-static)
add_dependencies(${target} roadrunner-static)

if (WIN32)
    target_compile_definitions(${target} PRIVATE EXPORT_RRC STATIC_RR)
endif ()


add_subdirectory(Apps)
add_subdirectory(ApiTest)

if (INSTALL_EXAMPLES)
    add_subdirectory(Examples)
endif ()

if (BUILD_TESTS)
    add_subdirectory(Testing)
endif ()

#=============== install =====================
#Glob all header files
FILE(GLOB headers rrc*.h)

install(
        FILES
        ${headers}
        DESTINATION include/rr/C COMPONENT c_api
)

install(
        TARGETS ${target}
        RUNTIME DESTINATION bin COMPONENT c_api
        ARCHIVE DESTINATION lib COMPONENT c_api
        LIBRARY DESTINATION lib COMPONENT c_api
)
