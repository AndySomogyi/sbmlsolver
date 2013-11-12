CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
rrNoisePluginDemo.py
rrLevenbergMarquardtDemo.py
rrPluginTester.py
rrBifurcationDemo.py
rrCallBackFunctionDemo.py
)

install(    
FILES ${examples} 
DESTINATION python/PluginExamples 	
COMPONENT Python
)

