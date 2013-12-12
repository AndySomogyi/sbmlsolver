CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
rrPluginTester.py
#rrBifurcationDemo.py
rrCallBackFunctionDemo.py
rrPluginDocumentationDemo.py
rrPluginParameterDemo.py
)

install(    
FILES ${examples} Readme.txt 
DESTINATION plugins/python/Examples 	
COMPONENT plugins_api
)

