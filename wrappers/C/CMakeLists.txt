include(GNUInstallDirs)
set(target roadrunner_c_api)

set(
        HEADERS
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

set(
        SOURCES
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
)

#The C-api is built as a shared library..
add_library(${target} SHARED ${HEADERS} ${SOURCES})

# link statically so users of the c library will not also need the shared.
target_link_libraries(${target} PRIVATE roadrunner-static ${LLVM_LIBRARIES} )
MESSAGE(STATUS "C LLVM_INCLUDE_DIRS ${LLVM_INCLUDE_DIRS}")
#target_include_directories(
#        ${target} PUBLIC
#        "D:/LLVM6.0.1/LLVM6.0.1-windows10-msvc2019/include/llvm"
#        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
#        "${LLVM_INCLUDE_DIRS}"
#        "${RR_INCLUDE_DIRECTORIES}"
#)

add_dependencies(${target} roadrunner-static)

target_compile_definitions(${target} PRIVATE EXPORT_RRC STATIC_RR)

#
#target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
##        ${CMAKE_SOURCE_DIR}/source
##        ${RR_CAPI_INCLUDE_DIR}
##        ${INCLUDE_DIRECTORIES}
##        ${RR_GENERATED_HEADER_PATH}
##        )
#
#target_link_libraries(${target} PRIVATE roadrunner-static)
#
#
#
#add_subdirectory(Apps)
#add_subdirectory(ApiTest)
#
#if (INSTALL_EXAMPLES)
#    add_subdirectory(Examples)
#endif ()
#
#if (BUILD_TESTS)
#    add_subdirectory(Testing)
#endif ()
#
##=============== install =====================
##Glob all header files
#FILE(GLOB headers rrc*.h)
#
#install(
#        FILES
#        ${headers}
#        DESTINATION include/rr/C COMPONENT c_api
#)
#
#install(
#        TARGETS ${target}
#        RUNTIME DESTINATION bin COMPONENT c_api
#        ARCHIVE DESTINATION lib COMPONENT c_api
#        LIBRARY DESTINATION lib COMPONENT c_api
#)
