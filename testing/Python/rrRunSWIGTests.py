import sys
import rrSWIGTests
import numpy

pathToModels = 'C:\\Python27\\Lib\\roadrunner\\testing\\Python\\'

# runTester takes two arguments:
#  1) The path to the results and model file
#  2) The name of the results and model file

rrSWIGTests.runTester (pathToModels, 'results_roadRunnerTest_1.txt')
