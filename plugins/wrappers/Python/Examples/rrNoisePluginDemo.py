import sys
import numpy
import matplotlib.pyplot as plot

import rrPython
import rrPlugins

rr = rrPython; rrp = rrPlugins

#Load the 'noise' plugin in order to add some noise to roadrunner data
noisePlugin = rrp.loadPlugin("add_noise")
if not noisePlugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(noisePlugin)

#get parameter for the 'size' of the noise
sigmaHandle = rrp.getPluginParameter(noisePlugin, "Sigma")

aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

#aSigmaDoublePtr = rrp.getParameterValueAsPointer(sigmaHandle)
#print ctypes.c_float(aSigmaDoublePtr).value

#set size of noise
rrp.setParameter(sigmaHandle, '0.000001')
aSigma = rrp.getParameterValueAsString(sigmaHandle)
print 'Current sigma is ' + aSigma

sbmlModel ="../models/test_1.xml"
if not rr.loadSBMLFromFile(sbmlModel):
    print rr.getLastError()
    exit()

results = rrPython.simulate()

#get a hold of data handle
rrDataHandle = rr.getRoadRunnerData()

#Execute the noise plugin which will add some noise to the (internal) data
rrp.executePluginEx(noisePlugin, rrDataHandle)

#Input Data
results = rr.getSimulationResult()
S1 = results[:,2]
x = numpy.arange(0, len(S1), 1)
plot.plot(x, S1, label="S1")

plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')

plot.show()
print "done"
