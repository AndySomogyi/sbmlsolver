# M Totte Karlsson
# J Kyle Medley

project( chisquared )

set(RRP_CHISQUARED_SOURCES
  chisquare_doc.cpp
  csChiSquare.cpp
  csChiWorker.cpp
  chisquare_doc.h
  csChiSquare.h
  csChiWorker.h
)

add_definitions(
  -DPOCO_STATIC
  -DLIBSBML_STATIC # FIXME: using shared libsbml
  -DEXPORT_TEL_PLUGIN
  -DSTATIC_LIBSTRUCT
  -DEXPORT_CHI_SQUARE
)

# TODO: change to tel_chisquared
add_library(tel_chisquare MODULE ${RRP_CHISQUARED_SOURCES})

target_link_libraries (tel_chisquare
  telpluginsBaseClass
  telplugins_math
  #telplugins_common
  ${RRPLUGINS_COMMON_SHARED_LIBS}
)

if(UNIX)
  if(APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no_implicit_dylibs -undefined error")
  else()
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")
  endif()

  set_property(TARGET tel_chisquare
    PROPERTY  COMPILE_DEFINITIONS
    LIBSBML_STATIC # FIXME: using shared libsbml
    LIBLAX_STATIC
    STATIC_PUGI
    STATIC_LIBSTRUCT
  )
endif()

install (TARGETS tel_chisquare
  DESTINATION ${RRP_PLUGINS_DIR}
  COMPONENT plugins
)
