CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
rrPluginTester.py
#rrBifurcationDemo.py
rrEvents.py
rrPluginDocumentation.py
rrPluginParameters.py
rrPluginParameterLists.py
rrLevenbergMarquardt.py
rrNoisePlugin.py
rrTwoPlugins.py
rrCreateNoisyData.py
rrReadDataFromFile.py
)

install(    
FILES ${examples} README.txt testData.dat
DESTINATION plugins/Examples 	
COMPONENT plugins_api
)


if(BUILD_TEST_TOOLS)
install(    
FILES runAllPythonExamples.sh 
DESTINATION plugins/Examples 	
COMPONENT plugins_api
)
endif()

