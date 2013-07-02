import sys
import rrPython
rr = rrPython

modelFile ="r:/models/test_1.xml"
rr.setTempFolder('r:/temp/python')

print 'RoadRunner C API Version: ' + rr.getAPIVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: ' + rr.getCopyright()
tempFolder=rr.getTempFolder()

print 'TempFolder is :' + tempFolder

rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile(modelFile)

print 'Result of loading sbml: %r' % (result);
noisePlugin = rr.loadPlugin("add_noise")
print rr.getPluginInfo(noisePlugin)

rr.simulate()
rrDataHandle = rr.getRoadRunnerData()

sigmaHandle = rr.getPluginParameter(noisePlugin, "Sigma")

aSigma = rr.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

rr.setParameter(sigmaHandle, '0.00001')
aSigma = rr.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

rrCData = rr.getSimulationResult()
print rrCData

#Execute the noise plugin, add some noise to the data
rr.executePluginEx(noisePlugin, rrDataHandle)

while rr.isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";

rrCData = rr.getSimulationResult2()

lmPlugin = rr.loadPlugin("lm")

print rr.getPluginInfo(lmPlugin)

#Setup the plugin
minDataParaHandle   = rr.getPluginParameter(lmPlugin, "MinData");

#The actual value, as a pointer
minDataHandle       = rr.getParameterValueAsPointer(minDataParaHandle)

#Input Data
rr.setPluginInputData(lmPlugin, rrCData)

#Minimization Parameters
rr.addDoubleParameter(minDataHandle, "k1", 1.23)

#set species to fit
rr.setMinimizationModelDataSelectionList(minDataHandle, "S1 S2")

#set species to fit
rr.setMinimizationObservedDataSelectionList(minDataHandle, "S1 S2")

#set tempFolder
rr.setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
rr.setPluginParameter(lmPlugin, "SBML", rr.getSBML())


#Check plugin status, input
report = rr.getPluginStatus(lmPlugin)

print '=========================== Levenberg-Marquardt report before minimization '
print report

#Execute the noise plugin, add some noise to the data
rr.executePlugin(lmPlugin)

while rr.isPluginWorking(lmPlugin) == True:
    print "Plugin is not done yet";


report = rr.getPluginStatus(lmPlugin)

print '=========================== Levenberg-Marquardt report after minimization '
print report

print rr.getPluginResult(lmPlugin)
print "done"
