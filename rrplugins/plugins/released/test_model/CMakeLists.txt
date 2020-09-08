# M Totte Karlsson
# J Kyle Medley

project( test_model )

set(RRP_TEST_MODEL_SOURCES
  TestModel.h
  test_model_doc.h
  TestModel.cpp
  test_model_doc.cpp
  
  ../add_noise/add_noise.cpp
  ../add_noise/add_noise_worker.cpp
  ../add_noise/noise.cpp
  ../add_noise/add_noise.h
  ../add_noise/add_noise_worker.h
  ../add_noise/noise.h
)

add_definitions(
  -DEXPORT_TEL_PLUGIN
  -DEXPORT_TEST_MODEL
  -DPOCO_STATIC
)

add_library(tel_test_model MODULE ${RRP_TEST_MODEL_SOURCES})

target_link_libraries (tel_test_model telpluginsBaseClass 
    telplugins_math ${RRPLUGINS_COMMON_SHARED_LIBS})

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set_target_properties( tel_test_model PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
endif ()

# if(NOT WIN32)
#   if(APPLE)
#     set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
#   else()
# #     set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
#   endif()
# endif()

install (TARGETS tel_test_model DESTINATION ${RRP_PLUGINS_DIR} COMPONENT plugins)
