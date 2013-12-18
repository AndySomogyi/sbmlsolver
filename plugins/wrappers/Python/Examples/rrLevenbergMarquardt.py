import matplotlib.pyplot as plot
from numpy import *
from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

sbmlModel ="../../models/sbml_test_0001.xml"
result = rr.load(sbmlModel)
print 'Result of loading sbml: %r' % (result);

steps = 1350
rr.simulate(0, 10, steps)

#Each roadrunner instance has an embedded dataobject
#we can get a handle to it by the following function  
rrDataHandle = getRoadRunnerDataHandle(rr)

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = loadPlugin(pm, "rrp_add_noise")
if not noisePlugin:
    print "Failed to load the noise plugin.."
    exit()

print getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setDoubleParameter(sigmaHandle, 3.e-6)


#Assign data to the Noise plugin
assignPluginInput(noisePlugin, rrDataHandle)

#Execute the noise plugin which will add some noise to the data
executePluginEx(noisePlugin, False)

#Input Data
npData = getNumpyData(rrDataHandle)
#print result

x = npData[:,0] #result['time']
y1Input = npData[:,1]
y2Input = npData[:,2]
print x

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
experimentalData    = getPluginParameter(lmPlugin, "ExperimentalData");
paraHandle          = getPluginParameter(lmPlugin, "InputParameterList");

#The actual parameter value, as a pointer
paraList = getParameterValueHandle(paraHandle);

#Add parameters to fit
para1 = createParameter("k1", "double", "A Hint")
setDoubleParameter(para1, 0.2)
addParameterToList(paraList, para1)

#Input Data
setRoadRunnerDataParameter(experimentalData, rrDataHandle)

#set species to fit
species = "[S1] [S2]"
paraHandle = getPluginParameter(lmPlugin, "ModelDataSelectionList");
setParameterByString(paraHandle, species)

#Get species list in observed data
paraHandle = getPluginParameter(lmPlugin, "ObservedDataSelectionList");
setParameterByString(paraHandle, species)

#set input sbml model
setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
print getPluginStatus(lmPlugin)

#Execute lmfit plugin
res = executePluginEx(lmPlugin, False)

if res == False:
    print getLastError()
    exit()
    

print '=========================== Levenberg-Marquardt report after minimization '
print getPluginStatus(lmPlugin)

# Look at the data
dataPHandle = getPluginParameter(lmPlugin, "ModelData");
dataHandle = getParameterValueHandle(dataPHandle)
npData = getNumpyData(dataHandle)
S1Model = npData[:,1]
S2Model = npData[:,2]


dataPHandle = getPluginParameter(lmPlugin, "Residuals");
dataHandle = getParameterValueHandle(dataPHandle)
npData = getNumpyData(dataHandle)
s1Residual= npData[:,1]
s2Residual= npData[:,2]

print "x:" + `len(x)`
print "y1" + `len(y1Input)`

plot.plot(x, y1Input,'*r', label="S1 Input")
plot.plot(x, S1Model, '-r', label="S1 Fitted")

plot.plot(x, y2Input,'*b', label="S2 Input")
plot.plot(x, S2Model, '-b', label="S2 Fitted")

plot.plot(x, s1Residual,'xr',  label="S1 Residual")
plot.plot(x, s2Residual, 'xb', label="S2 Residual")

#plot(x, S2, '-r')
#plot.plot(x, yOutput3, 'o')

plot.legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0.)
plot.show()
unLoadPlugins(pm)
print "done"
