import sys
import numpy
import matplotlib.pyplot as plot
from rrPython import *
from rrPlugins import *

#sbmlModel ="../models/squareWaveModel.xml"
sbmlModel ="../models/bistable.xml"
setTempFolder('r:/temp')

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is: ' + tempFolder

enableLogging()
setComputeAndAssignConservationLaws(True)
result = loadSBMLFromFile(sbmlModel)

print 'Result of loading sbml: %r' % (result);
#setTimeCourseSelectionList("time,S1")
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
setDoubleParameter(sigmaHandle, 1.e-3)
aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Execute the noise plugin which will add some noise to the data
executePluginEx(noisePlugin, rrDataHandle)

#Input Data
results = getSimulationResult()
S1_input = results[:,1]
x = numpy.arange(0, len(S1_input), 1)


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
#rrData = getRoadRunnerData()
setPluginInputData(lmPlugin, rrDataHandle)

#Minimization Parameters, check the actual SBML
addDoubleParameter(minDataHandle, "k1", 1.10)
addDoubleParameter(minDataHandle, "k2", 1.0)
addDoubleParameter(minDataHandle, "k3", 1.0)

#set species to fit
species = "x"
setMinimizationObservedDataSelectionList(minDataHandle, species)

#set species to fit, check the actual SBML
setMinimizationModelDataSelectionList(minDataHandle, species)

#set the plugins tempFolder
setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
report = getPluginStatus(lmPlugin)
print report

#Execute lmfit plugin
executePlugin(lmPlugin)

#The plugin does it work in a thread, so don't proceed until it is done
while isPluginWorking(lmPlugin) == True:
    print "Plugin is not done yet";

print '=========================== Levenberg-Marquardt report after minimization '
report = getPluginStatus(lmPlugin)
print report

print getPluginResult(lmPlugin)

results = getSimulationResult()
S1_output = results[:,1]

plot.plot(x, S1_input, label="S1_in")
plot.plot(x, S1_output, label="S1_out")
plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')
plot.show()

print "done"
