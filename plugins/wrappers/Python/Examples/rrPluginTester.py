import sys
import rrPython
import rrPlugins
rrp = rrPlugins
rr = rrPython
#Load plugins from the plugin folder
result = rrp.loadPlugins()
if not result:
    print rrp.getLastError()
    exit()

print 'Number of Plugins: ' + `rrp.getNumberOfPlugins()`
names = rrp.getPluginNames()
print rr.stringArrayToString(names)
rr.freeStringArray(names)
print `rrp.unLoadPlugins()`
print "done"
