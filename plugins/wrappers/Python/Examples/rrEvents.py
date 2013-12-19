import os
import roadrunner
import matplotlib.pyplot as plot
import numpy
from rrPlugins import *

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

#Create a plugin manager
pm = createPluginManager()

def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress, dummy):
    nr = progress[0]
    print '\nPlugin progress:' + `nr` +' %'

def pluginIsFinished():
    print 'The plugin did finish'

#Check if model file exists
sbmlModel ="../../models/bistable.xml"
if os.path.exists(sbmlModel):
    model = open(sbmlModel, 'r').read()
    rr.load(model)
else:
    print "The sbml model file: " + sbmlModel +" can't be found"
    exit()

timeStart = 0
timeEnd = 10
numPoints = 500
rr.simulate(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pm, "rrp_add_noise")

print getPluginInfo(plugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(plugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setParameterByString(sigmaHandle, '0.01')

cb_func1 =  NotifyEvent(pluginStarted)
assignOnStartedEvent(plugin,  cb_func1)

cb_func2 =  NotifyIntStrEvent(pluginIsProgressing)
assignOnProgressEvent(plugin, cb_func2)

cb_func3 =  NotifyEvent(pluginIsFinished)
assignOnFinishedEvent(plugin, cb_func3)

#Assign data to the plugin
setPluginParameter(plugin,"InputData", getRoadRunnerDataHandle(rr))

#Execute the noise plugin which will add some noise to the (internal) data
executePluginEx(plugin, True)

while isPluginWorking(plugin) == True:
    print ('.'),

result = rr.getSimulationResult()
x = result['time']
y = result['[x]']

plot.plot(x, y, label="[x]")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')
plot.show()

unLoadPlugins(pm)
unLoadAPI()

print "done"
