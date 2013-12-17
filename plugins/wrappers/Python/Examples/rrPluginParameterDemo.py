from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pm, "rrp_add_noise")
if not plugin:
    print getLastError()
    exit()

print getPluginInfo(plugin)
#Get the plugins parameters
paras =  getPluginParameters(plugin)

#Get parameters associated with a capability
print getListOfPluginParameterNames(paras)

paraHandle   = getPluginParameter(plugin, "NoiseType")

print 'getting some info about the parameter'
print getParameterInfo(paraHandle)

paraValueString = getParameterValueAsString(paraHandle)
print 'parameter is ' + paraValueString

setParameterByString(paraHandle, "1")
paraValueString = getParameterValueAsString(paraHandle)
print 'Parameter is now: ' + paraValueString

setIntParameter(paraHandle, 0)
test = getIntParameter(paraHandle)
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

#======= Test bools
boolParaHandle = createParameter("bool", "bool", "Hint", True)
print getBoolParameter(boolParaHandle)
setBoolParameter(boolParaHandle, False)
print getBoolParameter(boolParaHandle)

#====Test some strings
stringParaHandle = createParameter("bool", "string", "Hello", "Hello2")
print getStringParameter(stringParaHandle)
setStringParameter(stringParaHandle, "bla bla")
print getStringParameter(stringParaHandle)
