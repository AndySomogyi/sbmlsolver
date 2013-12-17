##@Module rrPluginsObj

import rrPlugins

class Plugins:

    def __new__(self, pluginName):
        self.pluginManager = rrPlugins.createPluginManager()
        self.pluginName = pluginName
        plugin = rrPlugins.loadPlugin (self.pluginManager, pluginName)
        parameterNames = rrPlugins.getListOfPluginParameterNames (plugin)
        setattr (self, "k1", 3.14)        
        print parameterNames
        
    def loadPlugins():
        rrPlugins.loadPlugins (self.pluginsManager)
        
    #def getPlugin (name):
        