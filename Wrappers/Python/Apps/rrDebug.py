import os
import rrPython

print 'RoadRunner Build Date: ' + rrPython.getCopyright()

startTime = 0
endTime = 5
numPoints = 50
selList="time,S1,S2"

#rrPython.setTempFolder(".")
rrPython.enableLogging()
rrPython.setComputeAndAssignConservationLaws(True)
result = rrPython.loadSBMLFromFile("..\\tests\\TestModel_1.xml")

print result;
#rrPython.getEigenvalues()
print "done"
rrPython.getEigenvalueIds()