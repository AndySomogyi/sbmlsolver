# M Totte Karlsson
# J Kyle Medley

project(plugins)

#Give all plugins access to headers in the following folders
include_directories(
  ${RR_PLUGINS_ROOT}/rrplugins/core
  ${RR_PLUGINS_ROOT}/rrplugins/common
  ${RR_PLUGINS_ROOT}/rrplugins/math
  ${RR_PLUGINS_ROOT}/wrappers/C
)

add_subdirectory(released)

#[[
if (RR_PLUGINS_BUILD_BETA_PLUGINS)
  add_subdirectory(beta)
endif()
]]

install(
  FILES README.txt
  DESTINATION plugins
)
