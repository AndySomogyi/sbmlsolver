##@Module rrPlugins
#This module allows access to the rrplugins_api.dll from python"""
import sys
import os
import rrPython
from ctypes import *
rr = rrPython

if len(os.path.dirname(__file__)):
    os.chdir(os.path.dirname(__file__))
sharedLib=''
rrpLib=None
libHandle=None
if sys.platform.startswith('win32'):
    rrInstallFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'bin'))
    os.environ['PATH'] = rrInstallFolder + ';' + "c:\\Python27" + ';' + "c:\\Python27\\Lib\\site-packages" + ';' + os.environ['PATH']
    sharedLib = os.path.join(rrInstallFolder, 'rrplugins_c_api.dll')
    rrpLib=CDLL(sharedLib)
    rrpLibHandle = rrpLib._handle

else:
    rrInstallFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'lib'))
    print(os.path.join(rrInstallFolder, 'librrp_api.dylib'))
    # check for .so (linux)
    if os.path.isfile(os.path.join(rrInstallFolder, 'librrp_api.so')):
        sharedLib = os.path.join(rrInstallFolder, 'librrp_api.so')
    elif os.path.isfile(os.path.join(rrInstallFolder, 'librrp_api.dylib')):
        sharedLib = os.path.join(rrInstallFolder, 'librrp_api.dylib')
    else:
        raise Exception("could not locate RoadRunner shared library")
    rrpLib = cdll.LoadLibrary(sharedLib)

##\mainpage notitle
#\section Introduction
#The plugin manager exposes a lightweight framework for adding functionality to RoadRunner by external plugins
#
#@code
#
#import rrPython
#import rrPluginManager
# fill out...
#@endcode
#
#\section Setup
#In order to import the python module, the python folder within the plugin-manager install folder must be in the system's python path. To make sure it is, do the following in Windows:

#Open the control panel and click on 'System'
#The following will appear; click on 'Advanced System Settings'
#\image html http://i.imgur.com/bvn9c.jpg

#Click on the 'Environment Variables' button highlighted in the image below
#\image html http://i.imgur.com/jBCfn.jpg

#Highlight the python path entry and click edit. The prompt shown below will appear. Enter the location of the python folder within the install folder with a semicolon between any other entries.
#\image html http://i.imgur.com/oLC32.jpg

##\defgroup initialization Library initialization and termination methods
# \brief Initialize library and terminate library instance

# \defgroup freeRoutines Free memory routines
# \brief Routines that should be used to free various data structures generated during the course of using the library


#=======================rrp_api=======================#
charptr = POINTER(c_char)

rrpLib.createPluginManager.restype = c_void_p
rrpLib.createPluginManagerEx.restype = c_void_p
rrpLib.freePluginManager.restype = c_bool

#===== The API allocate an internal global handle to
#===== ONE roadrunner instance and ONE instance of a plugin manager
gPluginManager = rrpLib.createPluginManager(rrPython.gHandle)

##\brief Initialize the plugins library and returns a new PluginManager instance
#\return Returns an instance of the library, returns false if it fails
def createPluginManager():
    return rrpLib.createPluginManager()

##\brief Free the plugin manager instance
#\param rrpLib Free the plugin manager instance given in the argument
def freePluginManager(iHandle):
    return rrpLib.freePluginManager(iHandle)

#Unload dll from python
def unloadAPI():
    return windll.kernel32.FreeLibrary(libHandle)

def loadPlugin(libraryName):
    return rrpLib.loadPlugin(gPluginManager, libraryName)

rrpLib.loadPlugins.restype = c_bool
def loadPlugins():
    return rrpLib.loadPlugins(gPluginManager)

rrpLib.getPluginNames.restype = c_void_p
def getPluginNames():
    return rrpLib.getPluginNames(gPluginManager)

rrpLib.unLoadPlugins.restype = c_bool
def unLoadPlugins():
    return rrpLib.unLoadPlugins(gPluginManager)

rrpLib.getNumberOfPlugins.restype = c_int
def getNumberOfPlugins():
    return rrpLib.getNumberOfPlugins(gPluginManager)

rrpLib.getPluginInfo.restype = c_char_p
def getPluginInfo(pluginHandle):
    return rrpLib.getPluginInfo(pluginHandle)

rrpLib.getPluginCapabilities.restype = c_char_p
def getPluginCapabilities(pluginHandle):
    return rrpLib.getPluginCapabilities(pluginHandle)


rrpLib.getPluginStatus.restype = c_char_p
def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

rrpLib.executePlugin.restype = c_bool
def executePlugin(pluginHandle):
    return rrpLib.executePlugin(pluginHandle)

def executePluginEx(pluginHandle, userData, runInThread=False):
    return rrpLib.executePluginEx(pluginHandle, c_void_p(userData), c_bool(runInThread))

rrpLib.getPluginResult.restype = c_char_p
def getPluginResult(pluginHandle):
    return rrpLib.getPluginResult(pluginHandle)

def getPluginParameter(pluginHandle, parameterName):
    return rrpLib.getPluginParameter(pluginHandle, parameterName, 0)

def isPluginWorking(pluginHandle):
    return rrpLib.isPluginWorking(pluginHandle)

def setPluginInputData(pluginHandle, userData):
    return rrpLib.setPluginInputData(pluginHandle, c_void_p(userData))

def setPluginParameter(pluginHandle, parameterName, paraValue):
    return rrpLib.setPluginParameter(pluginHandle, parameterName, c_char_p(paraValue))

#Parameter functionality
def getParameterValueAsString(parameter):
    value = rrpLib.getParameterValueAsString(parameter)
    return c_char_p(value).value

def getParameterValueAsPointer(parHandle):
    return c_void_p(rrpLib.getParameterValueAsPointer(parHandle))

def setParameter(parameter, value):
    return rrpLib.setParameter(parameter, value)

#Minimization data funcionality
def getMinimizationDataReport(minDataHandle):
    value = rrpLib.getMinimizationDataReport(minDataHandle)
    return c_char_p(value).value

def addDoubleParameter(minDataHandle, parLabel, parValue):
    rrpLib.addDoubleParameter(minDataHandle, parLabel, c_double(parValue))

def setMinimizationModelDataSelectionList(minDataHandle, selectionList):
    rrpLib.setMinimizationModelDataSelectionList(minDataHandle, c_char_p(selectionList))

def setMinimizationObservedDataSelectionList(minDataHandle, selectionList):
    rrpLib.setMinimizationObservedDataSelectionList(minDataHandle, c_char_p(selectionList))


