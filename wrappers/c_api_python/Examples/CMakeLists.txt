CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
getStoichMatrix.py
threeLineSimulation.py
simulateSelectionList.py
plotData.py
)

install(    
    FILES 
    ${examples} 
    DESTINATION c_api_python 	
    COMPONENT Python
    )

