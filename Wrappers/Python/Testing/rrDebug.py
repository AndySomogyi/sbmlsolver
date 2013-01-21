import os
import rrPython
rr = rrPython

print 'RoadRunner Build Date: ' + rr.getCopyright()

rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile("..\\testing\\TestModel_1.xml")

print result;
print rr.getEigenvalues()
print rr.getEigenvalueIds()
print rr.getTimeCourseSelectionList()
print rr.getSteadyStateSelectionList()
print "done"
