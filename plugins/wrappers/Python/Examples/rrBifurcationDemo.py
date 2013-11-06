import sys
import numpy
import matplotlib.pyplot as plot
import rrPython
import rrPlugins
rr = rrPython
rrp = rrPlugins

sbmlModel ="../models/bistable.xml"
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
numPoints = 500
results = rrPython.simulateEx(timeStart, timeEnd, numPoints)

#Load the 'noise' plugin in order to add some noise to the data
plugin = rrp.loadPlugin("rrp_auto2000")
if not plugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(plugin)

#get parameter for noise 'size'
rrp.setPluginParameter(plugin, "ScanDirection", "Negative")
rrp.setPluginParameter(plugin, "PrincipalContinuationParameter", "k3")
rrp.setPluginParameter(plugin, "PCPLowerBound", "0.2")
rrp.setPluginParameter(plugin, "PCPUpperBound", "1.2")

paraHandle = rrp.getPluginParameter(plugin,"PCPLowerBound")
test = rrp.getParameterValueAsString(paraHandle)
print 'Current value is ' + test

#Execute the noise plugin which will add some noise to the (internal) data
rrp.executePlugin(plugin)

#The plugin does it work in a thread, so don't proceed until it is done
while rrp.isPluginWorking(plugin) == True:
    print "Plugin is not done yet";


#Input Data
#results = rrp.getResult()

#S1 = results[:,2]
#S2 = results[:,3]
#S3 = results[:,4]
#x = numpy.arange(timeStart, timeEnd, (timeEnd - timeStart)/numPoints)
#plot.plot(x, S1, label="S1")
#plot.plot(x, S2, label="S2")
#plot.plot(x, S3, label="S3")
#plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
#plot.ylabel('Concentration (moles/L)')
#plot.xlabel('time (s)')

#plot.show()

print "done"
