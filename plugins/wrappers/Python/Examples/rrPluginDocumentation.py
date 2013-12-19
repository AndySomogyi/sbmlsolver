from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

pluginName = "rrp_add_noise"
plugin = loadPlugin(pm, pluginName)
if not plugin:
    print getLastError()
    exit()

#Get some general info about the plugin
print getPluginInfo(plugin)


#Get parameters associated with a capability
print getListOfPluginParameterNames(plugin)

#Get the manual for the plugin (bundled as embedded PDF)
ptr = getPluginManualAsPDF(plugin)
nrOfBytes = getPluginManualNrOfBytes(plugin)
manual = cast(ptr, POINTER(c_char * nrOfBytes))[0]

#Check if there is a PDF available
if nrOfBytes == 0:
    print 'This plugin does not have a manual.'
    exit()

#create a (temporary) pdf file from the data
outFName = pluginName + '.pdf'

with open(outFName, 'wb') as output:
    output.write(manual)

os.system('start ' + outFName) #a total guess
