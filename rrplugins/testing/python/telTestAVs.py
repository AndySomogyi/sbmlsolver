from teplugins import *

try:
    pm = createPluginManager()
    
    info = getPluginInfo(12)
    if info == None:
        print getLastError()

    info = getPluginInfo(pm)
    if info == None:
        print getLastError()
        
except Exception as e:
    print `e`    



