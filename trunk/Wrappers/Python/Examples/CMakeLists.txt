CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_PYTHON_EXAMPLES)

set(examples 	
getStoichMatrix.py
threeLineSimulation.py
simulateExSelectionList.py
simulatePlot.py
simulateSelectionList.py
threeLineSimulation.py
)

install(    
            FILES 
            ${examples} 
            DESTINATION Python 	
            COMPONENT Python
       )

