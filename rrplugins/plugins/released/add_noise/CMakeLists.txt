# M Totte Karlsson
# J Kyle Medley

project( add_noise )

set(RRP_ADD_NOISE_SOURCES
  add_noise.cpp
  add_noise_worker.cpp
  add_noise_doc.cpp
  noise.cpp
  add_noise.h
  add_noise_worker.h
  add_noise_doc.h
  noise.h
)

add_definitions(
  -DEXPORT_TEL_PLUGIN
  -DPOCO_STATIC
  -DEXPORT_ADD_NOISE
)

add_library(tel_add_noise MODULE ${RRP_ADD_NOISE_SOURCES})

set_property(TARGET tel_add_noise
  PROPERTY COMPILE_DEFINITIONS
  LIBSBML_STATIC
  LIBLAX_STATIC
  STATIC_PUGI
  STATIC_LIBSTRUCT
)

target_link_libraries ( tel_add_noise
  #telplugins_common
  telpluginsBaseClass
  telplugins_math
  ${RRPLUGINS_COMMON_SHARED_LIBS}
)


if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set_target_properties( tel_add_noise PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
endif ()

install (TARGETS tel_add_noise
  DESTINATION ${RRP_PLUGINS_DIR}
  COMPONENT plugins
)
