import sys; import numpy
import matplotlib.pyplot as plot
from rrPython import *
from rrPlugins import *

def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress):
    nr = progress[0]
    print '\nPlugin progress:' + `nr` +' %'


def pluginIsFinished():
    print 'The plugin did finish'

sbmlModel ="../models/bistable.xml"
if not loadSBMLFromFile(sbmlModel):
    print getLastError()
    exit()

rrDataHandle = simulateEx(0, 10, 500)

#Load the 'noise' plugin in order to add some noise to roadrunner data
noisePlugin = loadPlugin("rrp_add_noise")
if not noisePlugin:
    print rr.getLastError()
    exit()

print getPluginInfo(noisePlugin)

#get parameter for the 'size' of the noise
sigmaHandle = getPluginParameter(noisePlugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#Set size of noise
#setParameterByString(sigmaHandle, '0.001')
setDoubleParameter(sigmaHandle, 0.0003)

#Check parameter
aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

cb_func1 =  pluginCallBackType1(pluginStarted)
assignPluginStartedCallBack(noisePlugin,  cb_func1)

cb_func2 =  pluginCallBackType2(pluginIsProgressing)
assignPluginProgressCallBack(noisePlugin, cb_func2)

cb_func3 =  pluginCallBackType1(pluginIsFinished)
assignPluginFinishedCallBack(noisePlugin, cb_func3)

#Execute the noise plugin which will add some noise to the (internal) data
executePluginEx(noisePlugin, rrDataHandle, True)

while isPluginWorking(noisePlugin) == True:
    print ('.'),

#Input Data
results = getSimulationResult()
S1 = results[:,1]
x = numpy.arange(0, len(S1), 1)
plot.plot(x, S1, label="S1")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')
plot.show()
print "done"
