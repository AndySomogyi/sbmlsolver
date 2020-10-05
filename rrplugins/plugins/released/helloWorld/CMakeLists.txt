# Debashish Roy

project( Hello )

set(RRP_HELLO_SOURCES
  hello.cpp
  hello.h
)

add_definitions(
  -DEXPORT_TEL_PLUGIN
  -DPOCO_STATIC
  -DEXPORT_HELLO
)

add_library(tel_hello MODULE ${RRP_HELLO_SOURCES})

set_property(TARGET tel_hello
  PROPERTY COMPILE_DEFINITIONS
  LIBSBML_STATIC
  LIBLAX_STATIC
  STATIC_PUGI
  STATIC_LIBSTRUCT
)

target_link_libraries ( tel_hello
  telpluginsBaseClass
  ${RRPLUGINS_COMMON_SHARED_LIBS}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set_target_properties( tel_hello PROPERTIES INSTALL_RPATH "$ORIGIN/./" )
endif ()

install (TARGETS tel_hello
  DESTINATION ${RRP_PLUGINS_DIR}
  COMPONENT plugins
)
