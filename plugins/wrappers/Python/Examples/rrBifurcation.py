import sys; import numpy; import rrPython; import rrPlugins;
import matplotlib.pyplot as plot

rr = rrPython; rrp = rrPlugins

sbmlModel ="../models/bistable.xml"

result = rr.loadSBMLFromFile(sbmlModel)
print 'Result of loading sbml: %r' % (result);

#Load Auto plugin
plugin = rrp.loadPlugin("rrp_auto2000")
if not plugin:
    print rr.getLastError()
    exit()

print rrp.getPluginInfo(plugin)

#Set Auto Parameters
rrp.setPluginParameter(plugin, "ScanDirection", "Negative")
rrp.setPluginParameter(plugin, "PrincipalContinuationParameter", "k3")
rrp.setPluginParameter(plugin, "PCPLowerBound", "0.2")
rrp.setPluginParameter(plugin, "PCPUpperBound", "1.2")

#get handle to a parameter
paraHandle = rrp.getPluginParameter(plugin,"PCPLowerBound")
test = rrp.getParameterValueAsString(paraHandle)
print 'Current value is ' + test

#Execute the plugin
rrp.executePlugin(plugin)

biFurcationDiagram = rrp.getPluginParameter(plugin, "BiFurcationDiagram")
if biFurcationDiagram:
    print `rrp.getParameterValueAsString(biFurcationDiagram)`

print "done"
