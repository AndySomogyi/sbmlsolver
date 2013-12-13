from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pm, "rrp_add_noise")
if not plugin:
    print rr.getLastError()
    exit()

print getPluginInfo(plugin)
print getPluginCapabilities(plugin)
print getPluginCapabilitiesAsXML(plugin)

paraHandle   = getPluginParameter(plugin, "NoiseType")

print 'getting some info about the parameter'
print getParameterInfo(paraHandle)

paraValueString = getParameterValueAsString(paraHandle)
print 'parameter is ' + paraValueString

setParameterByString(paraHandle, "1")
paraValueString = getParameterValueAsString(paraHandle)
print 'Parameter is now: ' + paraValueString

setIntParameter(paraHandle, 0)
test = getParameterValue(paraHandle)
print test

paraValueString = getParameterValueAsString(paraHandle)
print 'Parameter is now: ' + paraValueString

## Retrieve a parameter handle
paraHandle = getPluginParameter(plugin, "Sigma")

print 'getting some info about the parameter'
print getParameterInfo(paraHandle)

paraValueString = getParameterValueAsString(paraHandle)
print 'parameter is ' + paraValueString

setParameterByString(paraHandle, "1.23")
paraValueString = getParameterValueAsString(paraHandle)
print 'Parameter is now: ' + paraValueString

var = 2.23
setDoubleParameter(paraHandle, var)
test = getParameterValue(paraHandle)
print test

paraValueString = getParameterValueAsString(paraHandle)
print 'Parameter is now: ' + paraValueString
