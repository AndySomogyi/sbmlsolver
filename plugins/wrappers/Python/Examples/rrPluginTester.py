import sys
from rrPlugins import *

## Create a plugin manager
pm = createPluginManager()

## Load plugins from the plugin folder
result = loadPlugins(pm)
if not result:
    print 'Failed to load any plugins'
    exit()

print 'Number of Plugins: ' + `getNumberOfPlugins(pm)`
namesHandle = getPluginNames(pm)
names = stringArrayToString(namesHandle)
print names

## Get a plugin handle
aPlugin = getFirstPlugin(pm)

if(aPlugin == None):
    print 'Failed to get a handle to a plugin'
    exit()

info = getPluginInfo(aPlugin)

print info
print `unLoadPlugins(pm)`
print "done"


