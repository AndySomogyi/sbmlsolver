cmake_minimum_required(VERSION 2.8)

PROJECT(rrModels)

set(models
feedback.xml
test_1.xml 				
ss_threestep.xml 	
ss_TurnOnConservationAnalysis.xml 	
ss_threeSpecies.xml
squareWaveModel.xml
)

if(BUILD_TESTS)
add_subdirectory(test_models)
endif()

install(    FILES ${models}
            DESTINATION models 	
            COMPONENT sbml_models)
