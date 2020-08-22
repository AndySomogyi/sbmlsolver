""" 
Usage:
   This scripts copies tellurium plugin files installed in python site packages
   to repository 
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
  
#============================= 
#Get sitepackages folder
try:
    repoFolder=os.path.join('r:\\','tlp', 'wrappers', 'python')    
    #Where is python site packages?
    telPackageName = 'teplugins'
    spFolder = os.path.join(getSitePackagesFolder(), telPackageName)
    
    if DEBUG: 
        print "spFolder " + spFolder
    
    #Figure what to copy        
        
    pyFiles = glob.glob(os.path.join(spFolder, '*.py'))
    if DEBUG: 
        print "pyFiles " + str(pyFiles)
         
    #Copy files
    for file in pyFiles:
        print 'Copying file: ' + file + ' -> ' + repoFolder 
        shutil.copy(file, repoFolder)
        
except Exception as e:
    print 'There was a problem copying changed python files: ' + `e`

