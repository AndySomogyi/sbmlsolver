cmake_minimum_required(VERSION 2.8)
PROJECT(tests)


set(python_testing
rrTester.py
testModel.py
)

install(FILES ${python_testing}
DESTINATION tests/Python 	
COMPONENT testing
)

