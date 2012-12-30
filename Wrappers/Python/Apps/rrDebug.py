import os
import rrPython
rr = rrPython

print 'RoadRunner Build Date: ' + rr.getCopyright()

startTime = 0
endTime = 5
numPoints = 50
selList="time,S1,S2"

#rr.setTempFolder(".")
rr.enableLogging()
rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile("..\\tests\\TestModel_1.xml")

print result;
print rr.getEigenvalues()
print rr.getEigenvalueIds()
print rr.getTimeCourseSelectionList()
print rr.getSteadyStateSelectionList()
print "done"