import roadrunner
import teplugins

try:    
    for i in range(1):
        noisePlugin = telplugins.Plugin ("tel_add_noise")    
        
        #This is not great semantics
        print noisePlugin.listOfPlugins()

    if telplugins.tpc.hasLoadPluginErrors(telplugins.telplugins._pluginManager):
        print telplugins.tpc.getPluginLoadErrors(telplugins._pluginManager)
    else:
        print 'No Errors'        
            
except Exception as e:
    print 'Problem: ' + `e`