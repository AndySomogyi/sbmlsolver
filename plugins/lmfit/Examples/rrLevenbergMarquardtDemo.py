from numpy import *
from matplotlib.pyplot import *
from rrPlugins import *
sbmlModel ="../models/feedback.xml"

#Create a plugin manager
pm = createPluginManager()

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

result = rr.load(sbmlModel)
print 'Result of loading sbml: %r' % (result);

steps = 50
rr.simulate(0, 10, steps)

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = loadPlugin(pm, "rrp_add_noise")
if not noisePlugin:
    exit()

print getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setDoubleParameter(sigmaHandle, 3.e-2)

rrDataHandle = getRoadRunnerDataHandle(rr)

#Execute the noise plugin which will add some noise to the data
executePluginEx(noisePlugin, rrDataHandle)

rrcData = createRRCData(rrDataHandle)
npData = getNPData(rrcData)
x = npData[:,0]
yInput = npData[:,1]

#The plugin does it work in a thread, so don't proceed until it is done
while isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";

#Load the Levenberg-Marquardt minimization plugin
lmPlugin = loadPlugin(pm, "rrp_lm")
if not lmPlugin:
    print getLastError()
    exit()

print getPluginInfo(lmPlugin)

#Setup the plugin for minimization
#See documentation for available parameters
paraHandle = getPluginParameter(lmPlugin, "InputParameterList");

#The actual parameter value, as a pointer
paraList = getParameterValueHandle(paraHandle);

#Add parameters to fit
para1 = createParameter("J0_VM1", "double")
setDoubleParameter(para1, 0.2)
addParameterToList(paraList, para1)

#Input Data
setPluginInputData(lmPlugin, rrDataHandle)

#set species to fit
species = "[S1] [S2] [S3] [S4]"
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

# Look at the data
dataPHandle = getPluginParameter(lmPlugin, "ModelData");
dataHandle = getParameterValueHandle(dataPHandle)
rrcData = createRRCData(dataHandle)
npData = getNPData(rrcData)
S1 = npData[:,1]
S2 = npData[:,2]
S3 = npData[:,3]
S4 = npData[:,4]


#Observed data should be the same as the input data, see above
##dataPHandle = getPluginParameter(lmPlugin, "ObservedData");
##dataHandle = getParameterValueHandle(dataPHandle)
##rrcData = createRRCData(dataHandle)
##npData = getNPData(rrcData)
##yOutput2 = npData[:,1]
##length = len(yOutput)

dataPHandle = getPluginParameter(lmPlugin, "ResidualsData");
dataHandle = getParameterValueHandle(dataPHandle)
rrcData = createRRCData(dataHandle)
npData = getNPData(rrcData)
yOutput3 = npData[:,1]

#Observe, the plotting of the data is erratic!!
#The data itself however is proper. Find another way of plotting.

plot(x, yInput,'*g')
plot(x, S1, '-r')
#plot(x, S2, '-r')
#plot(x, S3, '-r')
#plot(x, S4, '-r')
#plot(x, yOutput2, '*')
plot(x, yOutput3, 'o')

show()
unLoadPlugins(pm)
print "done"
