import sys
from rrPython import *
from rrPlugins import *

sbmlModel ="../models/test_1.xml"
setTempFolder('r:/temp/python')

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is: ' + tempFolder

enableLogging()
setComputeAndAssignConservationLaws(True)
result = loadSBMLFromFile(sbmlModel)

print 'Result of loading sbml: %r' % (result);

rrDataHandle = simulate()

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = loadPlugin("rrp_add_noise")
if not noisePlugin:
    print getLastError()
    exit()

print getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setDoubleParameter(sigmaHandle, 0.000001)
aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Execute the noise plugin which will add some noise to the data
executePluginEx(noisePlugin, rrDataHandle)

#The plugin does it work in a thread, so don't proceed until it is done
while isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";

#Load the Levenberg-Marquardt minimization plugin
lmPlugin = loadPlugin("rrp_lm")
if not lmPlugin:
    print getLastError()
    exit()

print getPluginInfo(lmPlugin)

#Setup the plugin
#The plugin has a parameter, called MinData
minDataParaHandle   = getPluginParameter(lmPlugin, "MinData");

#The actual parameter value, as a pointer
minDataHandle = getParameterValueAsPointer(minDataParaHandle)

#Input Data
rrCData = getSimulationResult2()
setPluginInputData(lmPlugin, rrCData)

#Minimization Parameters, check the actual SBML
addDoubleParameter(minDataHandle, "k1", 3.23)

#set species to fit
setMinimizationObservedDataSelectionList(minDataHandle, "S1 S2")

#set species to fit, check the actual SBML
setMinimizationModelDataSelectionList(minDataHandle, "S1 S2")

#set the plugins tempFolder
setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
report = getPluginStatus(lmPlugin)
print report

#Execute the noise plugin, add some noise to the data
executePlugin(lmPlugin)

#The plugin does it work in a thread, so don't proceed until it is done
while isPluginWorking(lmPlugin) == True:
    print "Plugin is not done yet";

print '=========================== Levenberg-Marquardt report after minimization '
report = getPluginStatus(lmPlugin)
print report

print getPluginResult(lmPlugin)
print "done"
