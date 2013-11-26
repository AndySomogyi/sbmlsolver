import os
from rrPython import *
from rrPlugins import *

pluginName = "rrp_add_noise"
plugin = loadPlugin(pluginName)
if not plugin:
    print getLastError()
    exit()

#Get some general info about the plugin
print getPluginInfo(plugin)

#Get the plugins capabilities
print getPluginCapabilities(plugin)

#Get parameters associated with a capability
print getPluginParameters(plugin, 'Add Noise')

#Get xml
print getPluginCapabilitiesAsXML(plugin)

#Get the manual for the plugin (bundled as embedded PDF)
ptr = getPluginManualAsPDF(plugin)
nrOfBytes = getPluginManualNrOfBytes(plugin)
manual = cast(ptr, POINTER(c_char * nrOfBytes))[0]

#Check if there is a PDF available
if nrOfBytes == 0:
    print 'This plugin don\'t have a manual'
    exit()

#create a (temporary) pdf file from the data
outFName = pluginName + '.pdf'

with open(outFName, 'wb') as output:
    output.write(manual)

os.system('start ' + outFName) #a total guess