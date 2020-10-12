# M Totte Karlsson
# J Kyle Medley
# Debashish Roy

project(released_plugins)

if (RR_PLUGINS_BUILD_TEST_MODEL_PLUGIN)
    add_subdirectory(test_model)
endif()

if (RR_PLUGINS_BUILD_ADD_NOISE_PLUGIN)
    add_subdirectory(add_noise)
endif()

if (RR_PLUGINS_BUILD_CHISQUARE_PLUGIN)
    add_subdirectory(chisquare)
endif()

if (RR_PLUGINS_BUILD_AUTO2000_PLUGIN)
    add_subdirectory(auto2000)
endif()

if (RR_PLUGINS_BUILD_HELLO_PLUGIN)
	add_subdirectory(helloWorld)
endif()





#if (RR_PLUGINS_BUILD_LEVENBERG_MARQUARDT_PLUGIN)
#add_subdirectory(levenberg_marquardt)
#endif()

#[[
if (RR_PLUGINS_BUILD_NELDER_MEAD_PLUGIN)
    add_subdirectory(nelder_mead)
endif()

if (RR_PLUGINS_BUILD_MONTE_CARLO_BS_PLUGIN)
    add_subdirectory(monte_carlo_bs)
endif()

if (RR_PLUGINS_BUILD_STAT_PLUGIN)
   add_subdirectory(statistics)
endif()
]]