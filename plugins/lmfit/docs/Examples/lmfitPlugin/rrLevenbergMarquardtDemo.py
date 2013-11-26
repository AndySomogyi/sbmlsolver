from numpy import *
from matplotlib import *
from matplotlib.pyplot import *
from rrPython import *
from rrPlugins import *

sbmlModel ="../models/squareWaveModel.xml"
setTempFolder('r:/temp')

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is: ' + tempFolder

enableLogging()
result = loadSBMLFromFile(sbmlModel)

print 'Result of loading sbml: %r' % (result);
#setTimeCourseSelectionList("time,S1")
rrDataHandle = simulateEx(0, 100, 1000)

rrcData = createRRCData(rrDataHandle)
npData = getNPData(rrcData)
print npData

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
setDoubleParameter(sigmaHandle, 1.e-1)

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

#Setup the plugin for minimization
#See documentation for available parameters
paraHandle = getPluginParameter(lmPlugin, "InputParameterList");

#The actual parameter value, as a pointer
paraList = getParameterValueAsPointer(paraHandle);

#Add parameters to fit
para1 = createParameter("k1", "double")
setDoubleParameter(para1, 0.2)
addParameterToList(paraList, para1)

#Input Data
#rrData = getRoadRunnerData()
setPluginInputData(lmPlugin, rrDataHandle)

#set species to fit
species = "[S1]"
paraHandle = getPluginParameter(lmPlugin, "ModelDataSelectionList");
setParameterByString(paraHandle, species)

#Get species list in observed data
paraHandle = getPluginParameter(lmPlugin, "ObservedDataSelectionList");
setParameterByString(paraHandle, species)

#set the plugins tempFolder
setPluginParameter(lmPlugin, "TempFolder", "r:/temp/")

#set input sbml model
setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
print getPluginStatus(lmPlugin)

#Execute lmfit plugin
executePluginEx(lmPlugin, None, False)

print '=========================== Levenberg-Marquardt report after minimization '
print getPluginStatus(lmPlugin)

# Lok at the data
#dataFile = tempFolder +
dataPHandle = getPluginParameter(lmPlugin, "ModelData");
dataHandle = getParameterValueAsPointer(dataPHandle)
writeRRData(dataHandle, tempFolder + "/ModelData.dat")

dataPHandle = getPluginParameter(lmPlugin, "ObservedData");
dataHandle = getParameterValueAsPointer(dataPHandle)
writeRRData(dataHandle, tempFolder + "/Observed.dat")

unloadAPI()
rr.unloadAPI()
t = arange(0.0, 2.0, 0.01)
s = sin(2*pi*t)
plot(t, s)

show()
print "done"
