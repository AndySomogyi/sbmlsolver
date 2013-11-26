##@Module rrPlugins
#This module allows access to the rrplugins_api.dll from python"""
import sys
import os
import roadrunner
from ctypes import *

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
#import rrPlugins
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
#Type of plugin callback, first argument is return type
pluginCallBackType1  = CFUNCTYPE(None)
pluginCallBackType2  = CFUNCTYPE(None, POINTER(c_int), c_void_p)

##\brief Initialize the plugins library and returns a new PluginManager instance
#\return Returns an instance of the library, returns None if it fails
rrpLib.createPluginManager.restype = c_void_p
def createPluginManager():
    return rrpLib.createPluginManager()

#===== The API allocate an internal global handle to =============
#===== ONE instance of a plugin manager using the global rrHandle from rrPython
rrI = roadrunner.RoadRunner()
rrHandle = c_void_p(id(rrI))

gPluginManager = rrpLib.createPluginManager(rrHandle)

##\brief Free the plugin manager instance
#\param rrpLib Free the plugin manager instance given in the argument
rrpLib.freePluginManager.restype = c_bool
def freePluginManager(iHandle):
    return rrpLib.freePluginManager(iHandle)

#Unload the API
def unloadAPI():
    unLoadPlugins()
    windll.kernel32.FreeLibrary(rrpLib._handle)

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

#---------- PLUGIN HANDLING FUNCTIONS ============================================
rrpLib.getPluginInfo.restype = c_char_p
def getPluginInfo(pluginHandle):
    return rrpLib.getPluginInfo(pluginHandle)

rrpLib.getPluginCapabilities.restype = c_char_p
def getPluginCapabilities(pluginHandle):
    return rrpLib.getPluginCapabilities(pluginHandle)

rrpLib.getPluginCapabilitiesAsXML.restype = c_char_p
def getPluginCapabilitiesAsXML(pluginHandle):
    return rrpLib.getPluginCapabilitiesAsXML(pluginHandle)

rrpLib.assignPluginStartedCallBack.args =[c_void_p, pluginCallBackType1]
rrpLib.assignPluginStartedCallBack.restype = None
def assignPluginStartedCallBack(pluginHandle, pluginCallBack):
    return rrpLib.assignPluginStartedCallBack(pluginHandle, pluginCallBack, None)

rrpLib.assignPluginProgressCallBack.restype = None
def assignPluginProgressCallBack(pluginHandle, pluginCallBack):
    return rrpLib.assignPluginProgressCallBack(pluginHandle, pluginCallBack, None)

rrpLib.assignPluginFinishedCallBack.restype = None
def assignPluginFinishedCallBack(pluginHandle, pluginCallBack):
    return rrpLib.assignPluginFinishedCallBack(pluginHandle, pluginCallBack, None)

rrpLib.getPluginStatus.restype = c_char_p
def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

rrpLib.executePlugin.restype = c_bool
def executePlugin(pluginHandle):
    return rrpLib.executePlugin(pluginHandle)

rrpLib.executePlugin.restype = c_bool
def executePluginEx(pluginHandle, userData, runInThread=False):
    return rrpLib.executePluginEx(pluginHandle, c_void_p(userData), c_bool(runInThread))

rrpLib.getPluginResult.restype = c_char_p
def getPluginResult(pluginHandle):
    return rrpLib.getPluginResult(pluginHandle)

def isPluginWorking(pluginHandle):
    return rrpLib.isPluginWorking(pluginHandle)

def setPluginInputData(pluginHandle, userData):
    return rrpLib.setPluginInputData(pluginHandle, c_void_p(userData))

#Plugin Parameter functionality
rrpLib.createParameter.restype = c_void_p
def createParameter(name, the_type):
    return rrpLib.createParameter(name, the_type, None)

rrpLib.addParameterToList.restype = c_bool
def addParameterToList(listHandle, paraHandle):
    return rrpLib.addParameterToList(listHandle, paraHandle)

rrpLib.getPluginParameters.restype = c_char_p
def getPluginParameters(pluginHandle, capabilityName):
    return rrpLib.getPluginParameters(pluginHandle, capabilityName)

def getPluginParameter(pluginHandle, parameterName):
    return rrpLib.getPluginParameter(pluginHandle, parameterName, 0)

def setPluginParameter(pluginHandle, parameterName, paraValue):
    return rrpLib.setPluginParameter(pluginHandle, parameterName, c_char_p(paraValue))

rrpLib.getParameterInfo.restype = c_char_p
def getParameterInfo(paraHandle):
    return rrpLib.getParameterInfo(paraHandle)

rrpLib.getParameterType.restype = c_char_p
def getParameterType(paraHandle):
    return rrpLib.getParameterType(paraHandle)

rrpLib.getParameterValueAsString.restype = c_char_p
def getParameterValueAsString(parameter):
    return rrpLib.getParameterValueAsString(parameter)

rrpLib.getParameterValueAsPointer.restype = c_void_p
def getParameterValueAsPointer(parHandle):
    return rrpLib.getParameterValueAsPointer(parHandle)

def setIntParameter(parameter, value):
    return rrpLib.setIntParameter(parameter, c_int(value))

#rrpLib.setDoubleParameter.args = [c_void_p, c_double]
def setDoubleParameter(parameter, value):
    return rrpLib.setDoubleParameter(parameter, c_double(value))

def setStringParameter(parameter, value):
    return rrpLib.setStringParameter(parameter, c_char_p(value))

def setEnumParameter(parameter, value):
    return rrpLib.setIntParameter(parameter, value)

def setParameterByString(parameter, value):
    return rrpLib.setParameterByString(parameter, value)

def getParameterValue(paraHandle):
    paraType = getParameterType(paraHandle)
    if paraType == 'double':
        paraVoidPtr = getParameterValueAsPointer(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_double))
        return ptr[0]
    if paraType == 'int':
        paraVoidPtr = getParameterValueAsPointer(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_int))
        return ptr[0]
    if paraType == 'string':
        paraVoidPtr = getParameterValueAsPointer(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_char_p))
        return ptr[0]
    if paraType == 'NoiseType':
        paraVoidPtr = getParameterValueAsPointer(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_int))
        return ptr[0]
    else:
        return None

#DOCS
rrpLib.getPluginManualAsPDF.restype =  POINTER(c_ubyte)
def getPluginManualAsPDF(pluginHandle):
    return rrpLib.getPluginManualAsPDF(pluginHandle)

def getPluginManualNrOfBytes(pluginHandle):
    return rrpLib.getPluginManualNrOfBytes(pluginHandle)




