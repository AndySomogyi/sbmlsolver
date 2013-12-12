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

sbmlModel ="../models/bistable.xml"
model = open(sbmlModel, 'r').read()
result = rr.load(model)
print 'Result of loading sbml: %r' % (result);

timeStart = 0
timeEnd = 10
numPoints = 500
rr.simulate(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pm, "rrp_add_noise")
if not plugin:
    print rr.getLastError()
    exit()

print getPluginInfo(plugin)
print getPluginCapabilities(plugin)

#get parameter for noise 'size'
sigmaHandle = getPluginParameter(plugin, "Sigma")

aSigma = getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
setParameterByString(sigmaHandle, '0.01')

cb_func1 =  pluginCallBackType1(pluginStarted)
assignPluginStartedCallBack(plugin,  cb_func1)

cb_func2 =  pluginCallBackType2(pluginIsProgressing)
assignPluginProgressCallBack(plugin, cb_func2)

cb_func3 =  pluginCallBackType1(pluginIsFinished)
assignPluginFinishedCallBack(plugin, cb_func3)

rrDataHandle = getRoadRunnerDataHandleFromInstance(rr)
#Execute the noise plugin which will add some noise to the (internal) data
executePluginEx(plugin, rrDataHandle, True)

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
unloadAPI()

print "done"
