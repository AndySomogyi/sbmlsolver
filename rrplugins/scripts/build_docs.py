import sys
import os
import os.path
import show_status as ss
import subprocess
import datetime
import shutil


import pbs as sc
from pbs import git
from pbs import ls

def cleanFolder(path):
    print('Removing folder ' + path)
    shutil.rmtree(path, ignore_errors=True)


    #==== C DOCS ====
def generateDocs(doxyFile, inputFolder, outputFolder):
    os.chdir(inputFolder)    
    print 'Generating Docs from: ' + inputFolder     
    
    #Clean the folder first
    cleanFolder(outputFolder)
             
    cmd ='(type ' + doxyFile + ' & echo OUTPUT_DIRECTORY='+ outputFolder + ' )| doxygen -'
    print 'Executing: ' + cmd
        
    output = subprocess.check_output(cmd, shell=True)    

from os.path import *
try:
    originalWorkingDirectory = os.getcwd()    
    buildFolder= os.path.dirname(os.path.realpath(__file__))
    repoFolder= join(buildFolder,'telPlugins')
    docsDir  = join(buildFolder,'ghPages', 'docs')
    cppDir= join(repoFolder,'source')
    
    pythonDir= join(repoFolder,'wrappers', 'python')
    
    #Change into repo folder
    os.chdir(repoFolder)    
    
    #Check status    
    print git("status")
    
    #Pull from origin
    #print git("pull")
    os.chdir(buildFolder)
    
    #Update doxygen documentation
    #==== C DOCS ====
    docsInput = join(repoFolder,'wrappers', 'c')
    docsOutput=join(buildFolder, docsDir, 'c')
    generateDocs('docs.doxy', docsInput, docsOutput)           
          
    #==== CPP DOCS ====
    docsInput = join(repoFolder,'source')
    docsOutput=join(buildFolder, docsDir, 'cpp')
    generateDocs('docs.doxy', docsInput, docsOutput)           
    
    #==== Python DOCS ====
    docsInput = join(repoFolder,'wrappers', 'python')
    docsOutput=join(buildFolder, docsDir, 'python')
    generateDocs('docs.doxy', docsInput, docsOutput)           
    
    #==== Low level Python DOCS ====
    docsInput = join(repoFolder,'wrappers', 'python')
    docsOutput=join(buildFolder, docsDir, 'python_ctypes')
    generateDocs('docs_ctypes.doxy', docsInput, docsOutput)           
       
    
    #Git add any new files
    
    
    #git push
    #print git("push")
     
except Exception as e:
   print e
##        


