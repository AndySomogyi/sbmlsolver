from teplugins import *

pm = createPluginManager()
print `loadPlugins(pm)`

print getPluginNames(pm)
print getPluginLibraryNames(pm)

pl = getFirstPlugin(pm)
print getPluginInfo(pl)

print getPluginPropertiesAsXML(pl)



