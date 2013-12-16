from rrPlugins import *

pm = createPluginManager()
print `loadPlugins(pm)`

print getPluginNames(pm)

pl = getFirstPlugin(pm)
print getPluginInfo(pl)

print getPluginParametersAsXML(pl)

parasHandle = getPluginParameters(pl)
paraNames = getListOfParameterNames(parasHandle)

print paraNames 

para = getPluginParameter(pl, "Sigma")

print getParameterInfo(para)
setParameterDescription(para, "An indepth description")
setParameterHint(para, "An indepth Hint")
print getParameterInfo(para)

