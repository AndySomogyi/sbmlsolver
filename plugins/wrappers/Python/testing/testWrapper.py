from rrPlugins import *

pm = createPluginManager()
print `loadPlugins(pm)`

print getPluginNames(pm)
print getPluginLibraryNames(pm)

pl = getFirstPlugin(pm)
print getPluginInfo(pl)

print getPluginPropertiesAsXML(pl)

parasHandle = getPluginParameters(pl)
paraNames = getNamesFromParameterList(parasHandle)
print paraNames 

print getListOfPluginParameterNames(pl)
para = getPluginParameter(pl, "Sigma")

print getParameterInfo(para)
setParameterDescription(para, "An indepth description")
setParameterHint(para, "An indepth Hint")
print getParameterInfo(para)

