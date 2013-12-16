CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
rrPluginTester.py
#rrBifurcationDemo.py
rrCallBackFunctionDemo.py
rrPluginDocumentationDemo.py
rrPluginParameterDemo.py
rrLevenbergMarquardtDemo.py
rrNoisePluginDemo.py
)

install(    
FILES ${examples} README.txt 
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

