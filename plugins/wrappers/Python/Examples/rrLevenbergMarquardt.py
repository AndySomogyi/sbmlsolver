import matplotlib.pyplot as plot
from numpy import *
from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Load the LevenbergMarquardt plugin
lmPlugin = loadPlugin(pm, "rrp_lm")

if not lmPlugin:
    print getLastError()
    exit()

#Read in the data to fit
rrDataHandle = createRoadRunnerDataFromFile("testData.dat")

#==== Setup the plugin for minimization ==========

#set input sbml model
sbml = getText("../../models/sbml_test_0001.xml")
setPluginParameter(lmPlugin, "SBML", sbml)

#See documentation for available parameters
experimentalData    = getPluginParameter(lmPlugin, "ExperimentalData");
paraList            = getPluginParameter(lmPlugin, "InputParameterList");

#Add parameters to fit
para1 = createParameter("k1", "double", "", 0.2)
addParameterToList(paraList, para1)

#Input Data
setRoadRunnerDataParameter(experimentalData, rrDataHandle)

#set species to fit
species = "[S1] [S2]"
paraHandle = getPluginParameter(lmPlugin, "FittedDataSelectionList");
setParameterByString(paraHandle, species)

#Get species list in observed data
paraHandle = getPluginParameter(lmPlugin, "ExperimentalDataSelectionList");
setParameterByString(paraHandle, species)

#Check plugin status, input
print '=========================== Levenberg-Marquardt report before minimization '
print getPluginStatus(lmPlugin)

#Execute lmfit plugin
res = executePluginEx(lmPlugin)
   
print '=========================== Levenberg-Marquardt report after minimization '
print getPluginStatus(lmPlugin)

#Input Data
npData = getNumpyData(rrDataHandle)

x = npData[:,0] #result['time']
y1Input = npData[:,1]
y2Input = npData[:,2]

# Look at the data
dataPHandle = getPluginParameter(lmPlugin, "FittedData");
dataHandle = getParameterValue(dataPHandle)
npData = getNumpyData(dataHandle)
S1Model = npData[:,1]
S2Model = npData[:,2]

dataPHandle = getPluginParameter(lmPlugin, "Residuals");
dataHandle = getParameterValue(dataPHandle)
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
