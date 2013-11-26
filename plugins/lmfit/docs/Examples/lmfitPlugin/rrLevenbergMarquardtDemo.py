from numpy import *
from matplotlib import *
from matplotlib.pyplot import *
from rrPlugins import *
sbmlModel ="../models/bistable.xml"

rr = roadrunner.RoadRunner()
#Check temporary folder setting
#tempFolder=rr.getTempFolder()
#print 'TempFolder is: ' + tempFolder

result = rr.load(sbmlModel)

print 'Result of loading sbml: %r' % (result);

rr.simulate(0,15,128)
rrDataHandle = getRoadRunnerDataHandle(rr)

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
setDoubleParameter(sigmaHandle, 1.e-2)

#Execute the noise plugin which will add some noise to the data
executePluginEx(noisePlugin, rrDataHandle)

#Input Data
result = rr.getSimulationResult()
yInput = result['[x]']
x = result['time']

print len(x)
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
species = "[x]"
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
rrcData = createRRCData(dataHandle)
npData = getNPData(rrcData)
yOutput = npData[:,1]
length = len(yOutput)
#print npData


dataPHandle = getPluginParameter(lmPlugin, "ObservedData");
dataHandle = getParameterValueAsPointer(dataPHandle)
rrcData = createRRCData(dataHandle)
npData = getNPData(rrcData)
yOutput2 = npData[:,1]
length = len(yOutput)

#writeRRData(dataHandle, tempFolder + "/Observed.dat")


#plot(x, yInput,'*')
plot(x, yOutput)
plot(x, yOutput2, '*')

show()
print "done"
