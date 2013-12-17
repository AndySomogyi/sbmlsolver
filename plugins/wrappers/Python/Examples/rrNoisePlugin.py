import sys;
import numpy
import os
import matplotlib.pyplot as plot
import roadrunner
from ctypes import *
from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Create a roadrunner instance
rr = roadrunner.RoadRunner()

#Define callback functions
def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress, dummy):
    nr = progress[0]
    print '\nPlugin progress:' + `nr` +' %'

def pluginIsFinished():
    print 'The plugin did finish'

sbmlModel ="../../models/bistable.xml"
if not rr.load(sbmlModel):
    print 'Failed loading model'
    exit()

rr.simulate(0, 10, 500)
print rr.getInfo()

#The plugin will need a handle to the underlying roadrunner data
rrDataHandle = getRoadRunnerDataHandle(rr)

#Load the 'noise' plugin in order to add some noise to roadrunner data
noisePlugin = loadPlugin(pm, "rrp_add_noise")
if not noisePlugin:
    print getLastError()
    exit()
assignPluginInput(noisePlugin, rrDataHandle)
print getPluginInfo(noisePlugin)

#get parameter for the 'size' of the noise
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Set size of noise
setDoubleParameter(sigmaHandle, 0.03)

#Check parameter
aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

cb_func1 =  NotifyEvent(pluginStarted)
assignOnStartedEvent(noisePlugin,  cb_func1)

cb_func2 =  NotifyIntStrEvent(pluginIsProgressing)
assignOnProgressEvent(noisePlugin, cb_func2)

cb_func3 =  NotifyEvent(pluginIsFinished)
assignOnFinishedEvent(noisePlugin, cb_func3)

#Execute the noise plugin which will add some noise to the (internal) data
executePluginEx(noisePlugin, False)

while isPluginWorking(noisePlugin) == True:
    print ('.'),

#Input Data
result = rr.getSimulationResult()
x = result['time']
y = result['[x]']

plot.plot(x, y, label="[x]")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')
plot.show()

unLoadPlugins(pm)
print "done"
