import sys
import rrPlugins
rrp = rrPlugins

#Load plugins from the plugin folder
result = rrp.loadPlugins()
if not result:
    print rrp.getLastError()
    exit()

print 'Number of Plugins: ' + `rrp.getNumberOfPlugins()`
namesHandle = rrp.getPluginNames()
names = rrp.stringArrayToString(namesHandle)

print names

print `rrp.unLoadPlugins()`
print "done"
