import sys
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
names= getPluginNames(pm)
print names

# Get a plugin handle
aPlugin = getFirstPlugin(pm)

if(aPlugin == None):
    print 'Failed to get a handle to a plugin'
    exit()

info = getPluginInfo(aPlugin)

print info
print `unLoadPlugins(pm)`
print "done"
