import sys
import rrPython
import rrPlugins
rr = rrPython
rrp = rrPlugins

sbmlModel ="../models/test_1.xml"
rr.setTempFolder('r:/temp/python')

print 'RoadRunner C API Version: '  + rr.getAPIVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: '                 + rr.getCopyright()

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is: ' + tempFolder

rr.enableLogging()
rr.setComputeAndAssignConservationLaws(True)
result = rr.loadSBMLFromFile(sbmlModel)

print 'Result of loading sbml: %r' % (result);


rr.simulate()
rrDataHandle = rr.getRoadRunnerData()

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = rrp.loadPlugin("add_noise")
if not noisePlugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = rrp.getPluginParameter(noisePlugin, "Sigma")

aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
rrp.setParameter(sigmaHandle, '0.00001')
aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Execute the noise plugin which will add some noise to the data
rrp.executePluginEx(noisePlugin, rrDataHandle)

#The plugin does it work in a thread, so don't proceed until it is done
while rrp.isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";

#Load the Levenberg-Marquardt minimization plugin
lmPlugin = rrp.loadPlugin("lm")
if not noisePlugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(lmPlugin)

#Setup the plugin
#The plugin has a parameter, called MinData
minDataParaHandle   = rrp.getPluginParameter(lmPlugin, "MinData");

#The actual parameter value, as a pointer
minDataHandle = rrp.getParameterValueAsPointer(minDataParaHandle)

#Input Data
rrCData = rr.getSimulationResult2()
rrp.setPluginInputData(lmPlugin, rrCData)

#Minimization Parameters, check the actual SBML
rrp.addDoubleParameter(minDataHandle, "k1", 3.23)

#set species to fit
rrp.setMinimizationObservedDataSelectionList(minDataHandle, "S1 S2")

#set species to fit, check the actual SBML
rrp.setMinimizationModelDataSelectionList(minDataHandle, "S1 S2")

#set the plugins tempFolder
rrp.setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
rrp.setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
report = rrp.getPluginStatus(lmPlugin)
print report

#Execute the noise plugin, add some noise to the data
rrp.executePlugin(lmPlugin)

#The plugin does it work in a thread, so don't proceed until it is done
while rrp.isPluginWorking(lmPlugin) == True:
    print "Plugin is not done yet";

print '=========================== Levenberg-Marquardt report after minimization '
report = rrp.getPluginStatus(lmPlugin)
print report

print rrp.getPluginResult(lmPlugin)
print "done"
