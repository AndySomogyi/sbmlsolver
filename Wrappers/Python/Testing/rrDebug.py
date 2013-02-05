import rrPython
rr = rrPython

print 'RoadRunner Version: ' + rr.getVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: ' + rr.getCopyright()

#rr.setTempFolder('/usr/local/lib')
#rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile("/usr/local/models/feedback.xml")

print 'Result of loading sbml: '
print result;
print rr.getEigenvalues()
print rr.getEigenvalueIds()
print rr.getTimeCourseSelectionList()
rr.setNumPoints(50)
rr.setTimeEnd(5)
print 'Unloading shared library'
#print rr.Unload(rr.handle)

#rr.loadPlugins()
#print rr.getPluginInfo("TestPlugin")

simResult = rr.simulate()
print simResult

print "done"
