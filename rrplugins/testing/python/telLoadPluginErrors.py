import roadrunner
import teplugins as tel

try:
    pm = tel.createPluginManager()
    
    if tel.loadPlugins(pm) == False:        
        print tel.getPluginLoadErrors(pm)   
         
except Exception as e:
    print 'Problem: ' + `e`