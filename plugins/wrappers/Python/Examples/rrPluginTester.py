from rrPlugins import *

# Create a plugin manager
pm = createPluginManager()

print 'The plugin manager will look for \
plugins in the following folder: ' + gDefaultPluginsPath

# Load plugins from the plugin folder
result = loadPlugins(pm)
if not result:
    print 'Failed to load any plugins'
    print getLastError()
    exit()

print 'Number of Plugins: ' + `getNumberOfPlugins(pm)`
print 'Plugin Names: ' + `getPluginNames(pm)`

# Get a handle to a plugin
aPlugin = getFirstPlugin(pm)

if(aPlugin == None):
    print 'Failed to get a handle to a plugin'
    exit()

print 'PluginName: ' + `getPluginName(aPlugin)`
print getPluginInfo(aPlugin)
print 'PluginParameters: ' + `getListOfPluginParameterNames(aPlugin)`
print `unLoadPlugins(pm)`
print "done"
