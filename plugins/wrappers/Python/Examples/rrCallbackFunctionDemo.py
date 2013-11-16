import rrPython as rr
import rrPlugins as rrp
import matplotlib.pyplot as plot
import numpy

def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress):
    nr = progress[0]
    print '\nPlugin progress:' + `nr` +' %'

def pluginIsFinished():
    print 'The plugin did finish'

sbmlModel ="../models/bistable.xml"
model = open(sbmlModel, 'r').read()
result = rr.loadSBML(model)
print 'Result of loading sbml: %r' % (result);

timeStart = 0.0
timeEnd = 10.0
numPoints = 500
rrDataHandle = rr.simulateEx(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
plugin = rrp.loadPlugin("rrp_add_noise")
if not plugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(plugin)
print rrp.getPluginCapabilities(plugin)

#get parameter for noise 'size'
sigmaHandle = rrp.getPluginParameter(plugin, "Sigma")

aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
rrp.setParameter(sigmaHandle, '0.01')

cb_func1 =  rrp.pluginCallBackType(pluginStarted)
rrp.assignPluginStartedCallBack(plugin,  cb_func1)

cb_func2 =  rrp.pluginCallBackType2(pluginIsProgressing)
rrp.assignPluginProgressCallBack(plugin, cb_func2)

cb_func3 =  rrp.pluginCallBackType(pluginIsFinished)
rrp.assignPluginFinishedCallBack(plugin, cb_func3)

#Execute the noise plugin which will add some noise to the (internal) data
rrp.executePluginEx(plugin, rrDataHandle, True)

while rrp.isPluginWorking(plugin) == True:
    print ('.'),

results = rr.getSimulationResult()
S1 = results[:,1]
x = numpy.arange(0, S1.size, 1)
plot.plot(x, S1, label="S1")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')

plot.show()

rr.unloadAPI()
rrp.unLoadPlugins()
rrp.unloadAPI()

print "done"
