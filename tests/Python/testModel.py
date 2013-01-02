
import sys
import rrTester
import rrPython
import numpy

pathToModels = 'C:\\vs\\trunk\\tests\\'

# runTester takes two arguments:
#  1) The path to the results and model file
#  2) The name of the results and model file

#marray = numpy.array([[-0.2, 0, 0.067], [0, -0.64, 0.4], [0.15, 0.09, -0.467 ]])
marray = numpy.array([[3,-2],[4,-1]])
x = rrPython.createRRMatrix (marray)
print "M = ", rrPython.matrixToString (x)
y = rrPython.getEigenvaluesMatrix (marray)
print rrPython.matrixToString(y)

#rrTester.runTester (pathToModels, 'roadRunnerTest99.txt')
