import sys; import numpy
import matplotlib.pyplot as plot
import rrPython; import rrPlugins
rr = rrPython; rrp = rrPlugins

def pluginStarted():
    print 'The plugin was started'

def pluginIsProgressing(progress):
    nr = progress[0]
    print '\nPlugin progress:' + `nr` +' %'

def pluginIsFinished():
    print 'The plugin did finish'

sbmlModel ="../models/test_1.xml"

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = rrp.loadPlugin("rrp_add_noise")
if not noisePlugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(noisePlugin)

#get parameter for noise 'size'
sigmaHandle = rrp.getPluginParameter(noisePlugin, "Sigma")

aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#set size of noise
rrp.setParameter(sigmaHandle, '0.000001')
aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

result = rr.loadSBMLFromFile(sbmlModel)
print 'Result of loading sbml: %r' % (result);

rrDataHandle = rrPython.simulateEx(0, 5, 100)

cb_func1 =  rrp.pluginCallBackType(pluginStarted)
rrp.assignPluginStartedCallBack(noisePlugin,  cb_func1)

cb_func2 =  rrp.pluginCallBackType2(pluginIsProgressing)
rrp.assignPluginProgressCallBack(noisePlugin, cb_func2)

cb_func3 =  rrp.pluginCallBackType(pluginIsFinished)
rrp.assignPluginFinishedCallBack(noisePlugin, cb_func3)

#Execute the noise plugin which will add some noise to the (internal) data
rrp.executePluginEx(noisePlugin, rrDataHandle, True)

while rrp.isPluginWorking(noisePlugin) == True:
    print ('.'),

#Input Data
results = rr.getSimulationResult()

S1 = results[:,2]
x = numpy.arange(0, S1.size, 1)
plot.plot(x, S1, label="S1")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')

plot.show()

print `rr.unloadAPI()`
print `rrp.unLoadPlugins()`
print `rrp.unloadAPI()`

print "done"
