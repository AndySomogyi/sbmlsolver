import sys
import rrPython
rr = rrPython

sbmlModel ="../models/test_1.xml"
rr.setTempFolder('r:/temp/python')

print 'RoadRunner C API Version: '  + rr.getAPIVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: '                 + rr.getCopyright()

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is :' + tempFolder

rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile(sbmlModel)

print 'Result of loading sbml: %r' % (result);


rr.simulate()
rrDataHandle = rr.getRoadRunnerData()

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = rr.loadPlugin("add_noise")
print rr.getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = rr.getPluginParameter(noisePlugin, "Sigma")

aSigma = rr.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
rr.setParameter(sigmaHandle, '0.00001')
aSigma = rr.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Execute the noise plugin which will add some noise to the data
rr.executePluginEx(noisePlugin, rrDataHandle)

#The plugin does it work in a thread, so don't proceed until it is done
while rr.isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";

#Load the Levenberg-Marquardt minimization plugin
lmPlugin = rr.loadPlugin("lm")
print rr.getPluginInfo(lmPlugin)

#Setup the plugin
#The plugin has a parameter, called MinData
minDataParaHandle   = rr.getPluginParameter(lmPlugin, "MinData");

#The actual parameter value, as a pointer
minDataHandle = rr.getParameterValueAsPointer(minDataParaHandle)

#Input Data
rrCData = rr.getSimulationResult2() 
rr.setPluginInputData(lmPlugin, rrCData)

#Minimization Parameters, check the actual SBML
rr.addDoubleParameter(minDataHandle, "k1", 3.23)

#set species to fit
rr.setMinimizationObservedDataSelectionList(minDataHandle, "S1 S2")

#set species to fit, check the actual SBML
rr.setMinimizationModelDataSelectionList(minDataHandle, "S1 S2")

#set the plugins tempFolder
rr.setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
rr.setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
report = rr.getPluginStatus(lmPlugin)
print report

#Execute the noise plugin, add some noise to the data
rr.executePlugin(lmPlugin)

#The plugin does it work in a thread, so don't proceed until it is done
while rr.isPluginWorking(lmPlugin) == True:
    print "Plugin is not done yet";

print '=========================== Levenberg-Marquardt report after minimization '
report = rr.getPluginStatus(lmPlugin)
print report

print rr.getPluginResult(lmPlugin)
print "done"
