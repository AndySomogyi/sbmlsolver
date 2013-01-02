
import sys
import rrTester
import rrPython
import numpy

pathToModels = 'C:\\vs\\trunk\\tests\\'

# runTester takes two arguments:
#  1) The path to the results and model file
#  2) The name of the results and model file

marray = numpy.array([[1,2],[3,4]])
x = rrPython.createRRMatrix (marray)
print "M = ", rrPython.matrixToString (x)
y = rrPython.getEigenvaluesMatrix (marray)
print y

#rrTester.runTester (pathToModels, 'roadRunnerTest99.txt')
