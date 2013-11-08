import sys
import numpy
import matplotlib.pyplot as plot
import rrPython
import rrPlugins
rr = rrPython
rrp = rrPlugins

sbmlModel ="../models/test_1.xml"
rr.setTempFolder('r:/temp')

print 'RoadRunner C API Version: '  + rr.getAPIVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: '                 + rr.getCopyright()

#Check temporary folder setting
tempFolder=rr.getTempFolder()
print 'TempFolder is: ' + tempFolder
result = rr.loadSBMLFromFile(sbmlModel)
print 'Result of loading sbml: %r' % (result);

timeStart = 0.0
timeEnd = 10.0
numPoints = 50
results = rrPython.simulateEx(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
noisePlugin = rrp.loadPlugin("add_noise")
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

#get a hold of data handle
rrDataHandle = rr.getRoadRunnerData()

#Execute the noise plugin which will add some noise to the (internal) data
rrp.executePluginEx(noisePlugin, rrDataHandle)

#The plugin does it work in a thread, so don't proceed until it is done
while rrp.isPluginWorking(noisePlugin) == True:
    print "Plugin is not done yet";


#Input Data
results = rr.getSimulationResult()

S1 = results[:,2]
#S2 = results[:,3]
#S3 = results[:,4]
x = numpy.arange(timeStart, timeEnd, (timeEnd - timeStart)/numPoints)
plot.plot(x, S1, label="S1")
#plot.plot(x, S2, label="S2")
#plot.plot(x, S3, label="S3")
plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
plot.ylabel('Concentration (moles/L)')
plot.xlabel('time (s)')

plot.show()

print "done"
