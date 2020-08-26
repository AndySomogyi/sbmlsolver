# M Totte Karlsson
# J Kyle Medley

project(plugins)

#Give all plugins access to headers in the following folders
include_directories(
  ${TLP_ROOT}/rrplugins/core
  ${TLP_ROOT}/rrplugins/common
  ${TLP_ROOT}/rrplugins/math
  ${TLP_ROOT}/wrappers/C
)

add_subdirectory(released)

#[[
if (TLP_BUILD_BETA_PLUGINS)
  add_subdirectory(beta)
endif()
]]

install(
  FILES README.txt
  DESTINATION plugins
)
