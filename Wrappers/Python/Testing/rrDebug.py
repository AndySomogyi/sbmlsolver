import rrPython
rr = rrPython

print 'RoadRunner Version: ' + rr.getVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: ' + rr.getCopyright()

rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile("..\\testing\\Test_1.xml")

print result;
print rr.getEigenvalues()
print rr.getEigenvalueIds()
print rr.getTimeCourseSelectionList()
print rr.getSteadyStateSelectionList()

print 'Unloading shared library'
#print rr.Unload(rr.handle)

rr.loadPlugins()
print rr.getPluginInfo("TestPlugin")
print "done"
