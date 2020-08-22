# M Totte Karlsson
# J Kyle Medley

include_directories(
  ${TLP_ROOT}/source/core
  ${TLP_ROOT}/source/common
  ${THIRD_PARTY_INSTALL_FOLDER}/include
  ${THIRD_PARTY_INSTALL_FOLDER}/include/rr # DEPRECATED
  ${THIRD_PARTY_INSTALL_FOLDER}/include/roadrunner
  ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
  ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
  ${THIRD_PARTY_INSTALL_FOLDER}/include/clapack
)

add_subdirectory(plugin_info)
