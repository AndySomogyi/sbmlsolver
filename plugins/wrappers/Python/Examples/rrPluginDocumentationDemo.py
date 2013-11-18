import os
from rrPython import *
from rrPlugins import *

pluginName = "rrp_add_noise"
#Load the 'noise' plugin in order to add some noise to the data
plugin = loadPlugin(pluginName)
if not plugin:
    print getLastError()
    exit()

print getPluginInfo(plugin)
print getPluginCapabilities(plugin)
print getPluginParameters(plugin, 'Add Noise')
print getPluginCapabilitiesAsXML(plugin)

ptr = getPluginManualAsPDF(plugin)
nrOfBytes = getPluginManualNrOfBytes(plugin)
manual = cast(ptr, POINTER(c_char * nrOfBytes))[0]

if nrOfBytes == 0:
    print 'This plugin don\'t have a manual'
    exit()

#create a pdf file from the data
outFName = pluginName + '.pdf'

with open(outFName, 'wb') as output:
    output.write(manual)

os.system('start ' + outFName) #a total guess