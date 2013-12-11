##@Module rrPlugins
#This module allows access to the rrplugins_api.dll from python"""
import sys
import os
import numpy as np
import roadrunner
from ctypes import *

if len(os.path.dirname(__file__)):
    os.chdir(os.path.dirname(__file__))
sharedLib=''
rrpLib=None
if sys.platform.startswith('win32'):
    rrInstallFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..\\..\\', 'bin'))
    os.chdir(rrInstallFolder)
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


#=======================rrp_api=======================#
#Type of plugin callback, first argument is return type
pluginCallBackType1  = CFUNCTYPE(None)
pluginCallBackType2  = CFUNCTYPE(None, POINTER(c_int), c_void_p)

##\brief Initialize the plugins library and returns a new PluginManager instance
#\return Returns an instance of the library, returns None if it fails
rrpLib.createPluginManager.restype = c_void_p
def createPluginManager():
    return rrpLib.createPluginManager(None)

##\brief Free the plugin manager instance
#\param rrpLib Free the plugin manager instance given in the argument
rrpLib.freePluginManager.restype = c_bool
def freePluginManager(iHandle):
    return rrpLib.freePluginManager(iHandle)

#Unload the API
def unloadAPI():
    windll.kernel32.FreeLibrary(rrpLib._handle)

rrpLib.loadPlugins.restype = c_bool
def loadPlugins(pmHandle):
    return rrpLib.loadPlugins(pmHandle)

def loadPlugin(pm, libraryName):
    return rrpLib.loadPlugin(pm, libraryName)

rrpLib.getPluginNames.restype = c_void_p
def getPluginNames(pm):
    return rrpLib.getPluginNames(pm)

rrpLib.unLoadPlugins.restype = c_bool
def unLoadPlugins(pm):
    return rrpLib.unLoadPlugins(pm)

rrpLib.getNumberOfPlugins.restype = c_int
def getNumberOfPlugins(pm):
    return rrpLib.getNumberOfPlugins(pm)

rrpLib.getFirstPlugin.restype = c_void_p
def getFirstPlugin(pm):
    return rrpLib.getFirstPlugin(pm)

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

rrpLib.getParameterValueHandle.restype = c_void_p
def getParameterValueHandle(parHandle):
    return rrpLib.getParameterValueHandle(parHandle)

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
        paraVoidPtr = getParameterValueHandle(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_double))
        return ptr[0]
    if paraType == 'int':
        paraVoidPtr = getParameterValueHandle(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_int))
        return ptr[0]
    if paraType == 'string':
        paraVoidPtr = getParameterValueHandle(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_char_p))
        return ptr[0]
    if paraType == 'NoiseType':
        paraVoidPtr = getParameterValueHandle(paraHandle)
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

#rrpLib.getRRHandle.restype = c_void_p
def getRoadRunnerHandle(aRRFromswig):
    return cast(int(aRRFromswig.this), c_void_p)

rrpLib.getRoadRunnerDataHandle.restype = c_void_p
def getRoadRunnerDataHandle(aRRFromswig):
    handle = getRoadRunnerHandle(aRRFromswig)
    return rrpLib.getRoadRunnerDataHandle(handle)

rrpLib.createRRCData.restype = c_void_p
def createRRCData(rrDataHandle):
    return rrpLib.createRRCData(rrDataHandle)

##\brief Returns a string list in string form.
rrpLib.stringArrayToStringFWD.restype = c_char_p
def stringArrayToString(aList):
    return rrpLib.stringArrayToStringFWD(aList)

def getNPData(rrcDataHandle):
    rowCount = rrpLib.getRRDataNumRows(rrcDataHandle)
    colCount = rrpLib.getRRDataNumCols(rrcDataHandle)
    resultArray = np.zeros([rowCount, colCount])
    for row in range(rowCount):
        for col in range(colCount):
                value = c_double()
                if rrpLib.getRRCDataElementF(rrcDataHandle, row, col, pointer(value)) == True:
                    resultArray[row, col] = value.value
    return resultArray

##\mainpage notitle
#\section Introduction
#Roadrunners plugin library exposes a lightweight framework for adding functionality to RoadRunner core, by external plugins.
#The code fragment below shows briefly how to load plugins, check for plugins and get an manipulate an individual plugin.
#
#@code
#
#import sys
#import rrPlugins
#rrp = rrPlugins
#
# #Load plugins from the plugin folder
#result = rrp.loadPlugins()
#if not result:
#    print rrp.getLastError()
#    exit()
#
#print 'Number of Plugins: ' + `rrp.getNumberOfPlugins()`
#namesHandle = rrp.getPluginNames()
#names = rrp.stringArrayToString(namesHandle)
#
#print names
#
#print `rrp.unLoadPlugins()`
#print "done"
#@endcode
#
#    \section plugins_overview Overview
#    The libRoadRunner Plugin API is centered around three important concepts:
#    - A Plugin Manager (RRPluginManagerHandle)
#    - A Plugin (RRPlugin)
#    - A Plugin Parameter (RRParameter)
#
#    \section plugins_usage How to use plugins
#    A typical use case of the Plugin API may be as follows:
#
#    Plugin usage scenario
#    -# Client creates a PluginManager.
#    -# Client load plugins using the Plugin Manager.
#    -# Client get a handle to a plugin.
#    -# Client get a handle to a specific parameter in the plugin.
#    -# Client set the value of the parameter.
#    -# Client excutes the plugin.
#    -# Client retrieve the value of a plugins parameter, e.g. a "result" parameter.
#    .
#
#    \todo Fill out this section
#
#    \section plugins_writing How to write plugins
#    The current plugin framework is a 'minimalistic' API. It is designed to give a plugin developer
#    maximum amount of room for interacting with the RoadRunner core, and at the same time,
#    a minimum of requirements and abstract concepts getting in the way of designing a plugin.
#
#    This section will show you how to get on the way with your first plugin.
#    \todo Fill out this section
#   \section main_section Using rrPlugins.py
#   In order to use this wrapper (rrPlugins.py), the Python path need to inlcude the folder where the wrapper script is located, e.g.
#   "c:\\roadrunner-1.0.0\\plugins\\python"
#
# \defgroup plugin_manager Plugin Manager
# \brief Plugin Manager Library Functions
#
# \defgroup plugins Plugin Functions
# \brief Functions operating on Plugin Handles.
#
# \defgroup plugin_parameters Plugin Parameters
# \brief Plugins Parameter related functions
#
# \defgroup utilities Utility Functions
# \brief Functions to help and assist in the use of the Plugins framework


