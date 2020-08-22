""" 
Usage:
   python install_telplugins.py
   This file will install files needed to 
   use Tellurium plugins from Python. Specifically, it will install a module named
   telplugins in the systems Python sitepackage folder and needed dlls and plugin dlls as well
"""
import site
import shutil
import os.path
import os
import glob

DEBUG = 1

def getSitePackagesFolder():
    splist = site.getsitepackages()
    for s in splist:
        if s.find('site-packages') > 0:
            return s

def cleanFolder(path):
    print('Removing folder ' + path)
    shutil.rmtree(path, ignore_errors=True)
   

try:
    #============================= 
    #First get sitepackages folder
    telPackageName = 'teplugins'
    spFolder = os.path.join(getSitePackagesFolder(), telPackageName)
    if DEBUG: 
        print "spFolder " + spFolder
    
    print('Installing Tellurium plugins to folder: ' + spFolder)   
    #clean destination folder   
    
    print('Cleaning Tellurium plugins folder: ' + spFolder)
    cleanFolder(spFolder)

    #Figure what to copy        
    currentFolder = os.path.dirname(os.path.abspath(__file__))   
    if DEBUG: 
        print "currentFolder " + currentFolder
    
    pyFilesFolder = os.path.join(currentFolder, 'site-packages', telPackageName)    
    if DEBUG: 
        print "pyFilesFolder " + pyFilesFolder
        
    pyFiles = glob.glob(os.path.join(pyFilesFolder, '*.py'))
    if DEBUG: 
        print "pyFiles " + str(pyFiles)
        
    #Plugins 
    pluginsFolder = os.path.join(currentFolder,  'plugins')   
    plugins = glob.glob(os.path.join(pluginsFolder, '*.dll'))
                       
    #Copy the dlls that are in ..\bin folder
    #Binaries       
    binFolder = os.path.join(currentFolder, 'bin')   
    if DEBUG: 
        print "binFolder " + binFolder
        
    bins = glob.glob(os.path.join(binFolder, '*.dll'))
    
    if DEBUG: 
        print "bins " + str(bins)
                 
    rootFiles = pyFiles + bins
   
    if not os.path.exists(spFolder):
        os.makedirs(spFolder)
                    
    #Copy root files
    for file in rootFiles:
        print 'Copying file: ' + file + ' -> ' + spFolder 
        shutil.copy(file, spFolder)
    
    #Copy plugins
    destPluginFolder = spFolder #Copy plugins into the same folder as the other dll's 
    if DEBUG: 
        print "destPluginFolder " + destPluginFolder
    
    if not os.path.exists(destPluginFolder):
        os.makedirs(destPluginFolder)
            
    for file in plugins:
        print 'Copying file: ' + file + ' -> ' + destPluginFolder
        shutil.copy(file, destPluginFolder)
    
    #Copy any files necessary for plugins to work properly
    pluginSupportFiles =['README.txt']    
    for file in pluginSupportFiles:
        theFile = os.path.join(pluginsFolder, file)
        print 'Copying file: ' + theFile + ' -> ' + destPluginFolder
        shutil.copy(theFile, destPluginFolder)

except Exception as e:
    print 'There was a problem installing Tellurium plugins: ' + `e`

