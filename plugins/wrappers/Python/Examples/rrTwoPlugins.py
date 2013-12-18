import matplotlib.pyplot as plot
from numpy import *
from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

# Create some data
sbmlModel ="../../models/sbml_test_0001.xml"
result = rr.load(sbmlModel)
rr.simulate(0, 10, 512)

#Each roadrunner instance has an embedded dataobject
#get a handle to it by the following function  
rrDataHandle = getRoadRunnerDataHandle(rr)

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = loadPlugin(pm, "rrp_add_noise")

#get parameter that controls the amount of noise applied, i.e. sigma
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

#set size of Sigma
setDoubleParameter(sigmaHandle, 3.e-6)

#Assign data to the Noise plugin (Add a parameter to noise plugin and use that instead)
assignPluginInput(noisePlugin, rrDataHandle)

#Execute the noise plugin which will add some noise to the data
executePlugin(noisePlugin)

#Input Data
npData = getNumpyData(rrDataHandle)

x = npData[:,0] #result['time']
y1Input = npData[:,1]
y2Input = npData[:,2]
print x

#Load the Levenberg-Marquardt minimization plugin
lmPlugin = loadPlugin(pm, "rrp_lm")

#Setup the plugin for minimization
#See plugin documentation for available parameters
expDataParameter    = getPluginParameter(lmPlugin, "ExperimentalData");
paraHandle          = getPluginParameter(lmPlugin, "InputParameterList");

#The actual parameter value, as a pointer :TODO: use getListParameter func instead
paraList = getParameterValueHandle(paraHandle);

#Create and add parameters to fit to the plugins parameter list
para1 = createParameter("k1", "double", "A Hint")
setDoubleParameter(para1, 0.2) #Todo: do this in one line.. switch the hint field to be after 'value' field
addParameterToList(paraList, para1)

#Assign data to the experiemtal data parametra: Todo use setParameter function instead
setRoadRunnerDataParameter(expDataParameter, rrDataHandle)

#Setup selection listst
species = "[S1] [S2]"
paraHandle = getPluginParameter(lmPlugin, "ModelDataSelectionList");
setParameterByString(paraHandle, species) #Todo use setParameter

#Get species list in observed data
paraHandle = getPluginParameter(lmPlugin, "ObservedDataSelectionList");
setParameterByString(paraHandle, species)

#set input sbml model
setPluginParameter(lmPlugin, "SBML", rr.getSBML())

#Check plugin status, before any work
print '=========================== Levenberg-Marquardt report before minimization '
print getPluginStatus(lmPlugin)

#Execute lmfit plugin
res = executePluginEx(lmPlugin)

print '=========================== Levenberg-Marquardt report after minimization '
print getPluginStatus(lmPlugin)

# Look at the data
dataPHandle = getPluginParameter(lmPlugin, "ModelData");
dataHandle  = getParameterValueHandle(dataPHandle) #Change to getParameter
npData = getNumpyData(dataHandle)
S1Model = npData[:,1]
S2Model = npData[:,2]

dataPHandle = getPluginParameter(lmPlugin, "Residuals");
dataHandle = getParameterValueHandle(dataPHandle)
npData = getNumpyData(dataHandle)
s1Residual= npData[:,1]
s2Residual= npData[:,2]

plot.plot(x, y1Input,'*r', label="S1 Input")
plot.plot(x, S1Model, '-r', label="S1 Fitted")

plot.plot(x, y2Input,'*b', label="S2 Input")
plot.plot(x, S2Model, '-b', label="S2 Fitted")

plot.plot(x, s1Residual,'xr',  label="S1 Residual")
plot.plot(x, s2Residual, 'xb', label="S2 Residual")

plot.legend(bbox_to_anchor=(1.05, 1), loc=1, borderaxespad=0.)
plot.show()
unLoadPlugins(pm)
print "done"
