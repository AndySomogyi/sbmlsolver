# M Totte Karlsson
# J Kyle Medley

include_directories(
  ${RR_ROOT}/rrplugins/rrplugins/core
  ${RR_ROOT}/rrplugins/rrplugins/common
  ${THIRD_PARTY_INSTALL_FOLDER}/include
  ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
  
  ${Poco_INCLUDE_DIRS}
  ${LIBXML2_INCLUDE_DIR}
)

add_subdirectory(common)

add_subdirectory(math)

add_subdirectory(pluginBaseClass)

add_subdirectory(core)

