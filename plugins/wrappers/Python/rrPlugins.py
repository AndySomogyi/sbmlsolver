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

## \brief Create a new instance of a plugin manager.
## \brief A PluginManager manages a collection of plugins, loaded and unloaded by 
##  the load and unload API functions respectively.
## \param pluginDir Full path to folder containing plugins. If None, uses default folder.
## \return On success, a handle to a Plugin manager, on failure, None.
## \ingroup plugin_manager
rrpLib.createPluginManager.restype = c_void_p
def createPluginManager(pluginDir = None):
    return rrpLib.createPluginManager(pluginDir)

## \brief Free the plugin manager. A call to this function will also unload any loaded plugins.
## \param pm Handle to a plugin manager. 
## \return true if success, false otherwise. 
## \ingroup plugin_manager
rrpLib.freePluginManager.restype = c_bool
def freePluginManager(pm):
    return rrpLib.freePluginManager(pm)

##
## \brief Load plugins. The function will look in the default plugin folder for plugins, and load them.
## \param pm Handle to a PluginManager instance
## \return Returns true if Plugins are loaded, false otherwise
## \ingroup plugin_manager
##
rrpLib.loadPlugins.restype = c_bool
def loadPlugins(pm):
    return rrpLib.loadPlugins(pm)

##
## \brief Unload plugins
## \param pm Handle to a PluginManager instance
## \return Returns true if Plugins are unloaded succesfully, false otherwise
## \ingroup plugin_manager
##
rrpLib.unLoadPlugins.restype = c_bool
def unLoadPlugins(pm):
    return rrpLib.unLoadPlugins(pm)

##
## \brief Load a particular plugin
## \param pm Handle to a PluginManager instance
## \param pluginName Name of the plugin to load. The plugin name is the plugins shared library name, without path and extension.
## \return Returns a handle to a plugin, None if unsuccesfull
## \ingroup plugin_manager 
##
def loadPlugin(pm, pluginName):
    return rrpLib.loadPlugin(pm, pluginName)

##
## \brief unload a particular plugin
## \param pm Handle to a PluginManager instance
## \param pHandle Handle to a Plugin instance
## \return Returns true if the Plugin are unloaded succesfully, false otherwise
## \ingroup plugin_manager
##
def unLoadPlugin(pm, pHandle):
    return rrpLib.unLoadPlugin(pm, pHandle)

## \brief Get Number of loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns the number of loaded plugins, -1 if a problem is encountered
## \ingroup plugin_manager
rrpLib.getNumberOfPlugins.restype = c_int
def getNumberOfPlugins(pm):
    return rrpLib.getNumberOfPlugins(pm)

## \brief Function to retrieve the names of currently loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns names for loaded plugins as a RRStringArrayPtr, None otherwise
## \ingroup plugin_manager
rrpLib.getPluginNames.restype = c_void_p
def getPluginNames(pm):
    return rrpLib.getPluginNames(pm)

## \brief getFirstPlugin retrieves the "first" plugin in the plugin managers internal list of plugins.
## This function is typically used together with the getNextPlugin and the getPreviousPlugin functions.
## \param pm Handle to a PluginManager instance
## \return Returns a handle to a plugin. Returns None if the plugin is not found
## \ingroup plugin_manager
rrpLib.getFirstPlugin.restype = c_void_p
def getFirstPlugin(pm):
    return rrpLib.getFirstPlugin(pm)

## \brief getNextPlugin retrieves the "next" plugin in the plugin managers internal list of plugins. This function
##    is typically used together with the getFirstPlugin and getPreviousPlugin functions.
## \param pm Handle to a PluginManager instance
## \return Returns a handle to a plugin. Returns None if the plugin is not found
## \ingroup plugin_manager
rrpLib.getNextPlugin.restype = c_void_p
def getNextPlugin(pm):
    return rrpLib.getNextPlugin(pm)

## \brief getPreviousPlugin retrieves the "previous" plugin in the plugin managers internal list of plugins. This function
##    is typically used together with the getFirstPlugin and getNextPlugin functions.
## \param pm Handle to a PluginManager instance
## \return Returns a handle to a plugin. Returns None if the plugin is not found
## \ingroup plugin_manager
rrpLib.getPreviousPlugin.restype = c_void_p
def getPreviousPlugin(pm):
    return rrpLib.getPreviousPlugin(pm)

## \brief getCurrentPlugin retrieves the "current" plugin in the plugin managers internal list of plugins. This function
##    is typically used together with the getFirst, Next and getPreviousPlugin functions.
## \param pm Handle to a PluginManager instance
## \return Returns a handle to a plugin. Returns None if the plugin is not found
## \ingroup plugin_manager
rrpLib.getCurrentPlugin.restype = c_void_p
def getCurrentPlugin(pm):
    return rrpLib.getCurrentPlugin(pm)


## \brief GetPluginHandle
## \param pm Handle to a PluginManager instance
## \param pluginName Pointer to string holding the name of a plugin
## \return Returns a handle to a plugin, with name as supplied in the parameter pluginName.
## Returns None if the plugin is not found
## \ingroup plugin_manager
rrpLib.getPlugin.restype = c_void_p
def getPlugin(pm, pluginName):
    return rrpLib.getPlugin(pm, c_char_p(pluginName))


#---------- PLUGIN HANDLING FUNCTIONS ============================================
## \brief Get the name of a Plugin
## \param pluginHandle Handle to a plugin
## \return Returns the plugins full name, as a string, None otherwise
## \ingroup plugins
rrpLib.getPluginName.restype = c_char_p
def getPluginName(pluginHandle):
    return rrpLib.getPluginName(pluginHandle)

## \brief Return some information about a Plugin. 
## \param pluginHandle Handle to a plugin
## \return Returns info, as a string, for the plugin, None otherwise
## \ingroup plugins
rrpLib.getPluginInfo.restype = c_char_p
def getPluginInfo(pluginHandle):
    return rrpLib.getPluginInfo(pluginHandle)

## \brief Get Plugin manual as PDF. A plugin may embedd a help manual as a PDF. This function return such as a pointer to a string. 
## Use the function getPluginManualNrOfBytes to get the exact length of this string.
## \param pluginHandle Handle to a plugin
## \return Returns the plugins manuals pdf file as a unsigned char*. If not available, returns None.
## \ingroup plugins
rrpLib.getPluginManualAsPDF.restype =  POINTER(c_ubyte)
def getPluginManualAsPDF(pluginHandle):
    return rrpLib.getPluginManualAsPDF(pluginHandle)

## \brief Get the byte size for the PDF manual.
## \param pluginHandle Handle to a plugin
## \return Returns the nr of bytes in the plugins manuals pdf file as an unsigned int.
## \ingroup plugins
def getPluginManualNrOfBytes(pluginHandle):
    return rrpLib.getPluginManualNrOfBytes(pluginHandle)

## \brief Assign a roadrunner instance handle for the plugin to use.
##    A plugin may use an externally created roadrunner instance for its internal work.
##  \param pluginHandle Handle to a plugin
##  \param rrHandle Handle to a roadrunner instance
##  \return Returns true or false indicating success/failure
## \ingroup plugins
def assignRoadRunnerInstance(pluginHandle, rrHandle):
    return rrpLib.assignRoadRunnerInstance(pluginHandle, rrHandle)

## \brief The executePlugin function is the function designated to fire of a Plugins "worker".
## What is done when this function is entered is plugin dependent.
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating success/failure
## \note The execute function is blocking. If the plugin is todo long work, consider using
## the executePluginEx function that have the option to execute the plugin code in a thread.
## \ingroup plugins
rrpLib.executePlugin.restype = c_bool
def executePlugin(pluginHandle):
    return rrpLib.executePlugin(pluginHandle)

## \brief The executePluginEx is similar to the executePlugin function, except it takes two extra arguments.
## \param pluginHandle Handle to a plugin
## \param userData void* pointer to user data. Plugin dependent. See specific plugin documentation for what to pass as argument.
## \param inAThread bool indicating if the plugin should be executed in a thread.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.executePlugin.restype = c_bool
def executePluginEx(pluginHandle, userData, inAThread=False):
    return rrpLib.executePluginEx(pluginHandle, c_void_p(userData), c_bool(inAThread))

## \brief Get some status of a plugin. See the plugins documentation on what to expect. 
## \param pluginHandle Handle to a plugin
## \return Returns plugin status if available, as a string. None otherwise
## \ingroup plugins
rrpLib.getPluginStatus.restype = c_char_p
def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

## \brief Returns a plugins result, as a string. See the plugins documentation on what to expect. 
## \param pluginHandle Handle to a plugin
## \return Returns a plugins result if available. None otherwise
## \ingroup plugins
rrpLib.getPluginResult.restype = c_char_p
def getPluginResult(pluginHandle):
    return rrpLib.getPluginResult(pluginHandle)

## \brief Reset a Plugin. Plugin dependent. A reset function should bring the internal state of a plugin to a known state
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating success/failure
## \ingroup plugins
def resetPlugin(pluginHandle):
    return rrpLib.resetPlugin(pluginHandle)

## \brief check if plugin is actively working
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating i the plugin is busy or not
## \ingroup plugins
def isPluginWorking(pluginHandle):
    return rrpLib.isPluginWorking(pluginHandle)

## \brief Terminate any work that is in progress in a plugin. If the plugins worker is executed in a thread, this function
## will signal the internals of the plugin to terminate. 
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating success/failure
## \ingroup plugins
def terminateWork(pluginHandle):
    return rrpLib.terminateWork(pluginHandle)

## \brief Check if the work of a plugin is currently being terminated
## \param pluginHandle Handle to the plugin
## \return Returns true or false indicating if the work within the plugin is in the process of being terminated
## \ingroup plugins
def isBeingTerminated(pluginHandle):
    return rrpLib.isBeingTerminated(pluginHandle)

## \brief wasTerminated. query a  plugin if work was terminated before completion
## \param pluginHandle Handle to the plugin
## \return Returns true or false indicating if the work in the plugin was terminated or not
## \ingroup plugins
def wasTerminated(pluginHandle):
    return rrpLib.wasTerminated(pluginHandle)

## \brief Assign callback function fired when a plugin starts its work
## \param pluginHandle Handle to a plugin
## \param pluginCallBack Function pointer to callback routine
## \param userData1 void* pointer to user data.
## \param userData2 void* pointer to user data.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.assignPluginStartedCallBack.args =[c_void_p, pluginCallBackType1, c_void_p]
def assignPluginStartedCallBack(pluginHandle, pluginCallBack, userData1 = None, userData2 = None):
    return rrpLib.assignPluginStartedCallBack(pluginHandle, pluginCallBack, userData1, userData2)

## \brief Assign callback function fired as a plugin progresses
## \param pluginHandle Handle to a plugin
## \param pluginCallBack Function pointer to callback routine
## \param userData1 void* pointer to user data.
## \param userData2 void* pointer to user data.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.assignPluginProgressCallBack.args =[c_void_p, pluginCallBackType1, c_void_p]
def assignPluginProgressCallBack(pluginHandle, pluginCallBack, userData1 = None, userData2 = None):
    return rrpLib.assignPluginProgressCallBack(pluginHandle, pluginCallBack, userData1, userData2)

## \brief Assign callback function fired when a plugin finishes its work
## \param pluginHandle Handle to a plugin
## \param pluginCallBack Function pointer to callback routine
## \param userData1 void* pointer to user data.
## \param userData2 void* pointer to user data.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.assignPluginFinishedCallBack.args =[c_void_p, pluginCallBackType1, c_void_p]
def assignPluginFinishedCallBack(pluginHandle, pluginCallBack, userData1 = None, userData2 = None):
    return rrpLib.assignPluginFinishedCallBack(pluginHandle, pluginCallBack, userData1, userData2)

## \brief Hand external data to a plugin
## \param pluginHandle Handle to a plugin
## \param userData void* pointer to user data. Plugin dependent.
## \return Returns true or false indicating success/failure
## \ingroup plugins
def setPluginInputData(pluginHandle, userData):
    return rrpLib.setPluginInputData(pluginHandle, c_void_p(userData))

## \brief Get roadrunner instance handle from plugin
## \param pluginHandle Handle to a Plugin instance
## \return Returns a handle to a rrInstance if available, returns None otherwise
## \ingroup plugins
rrpLib.getRRHandleFromPlugin.restype = c_void_p
def getRRHandleFromPlugin(pluginHandle):
    return rrpLib.getRRHandleFromPlugin(pluginHandle)

## \brief Get a Plugins capabilities as a string
## \param pluginHandle Handle to a plugin
## \return Returns available capabilities for a particular plugin as a pointer to a string, None otherwise.
## \ingroup plugins
rrpLib.getPluginCapabilities.restype = c_char_p
def getPluginCapabilities(pluginHandle):
    return rrpLib.getPluginCapabilities(pluginHandle)

## \brief Get a Plugins capabilities as a xml document. The string returned from this function is formated as xml.
## \param pluginHandle Handle to a plugin
## \return Returns available capabilities and parameter in the capability, for a particular plugin as a pointer to a string, None otherwise
## \ingroup plugins
rrpLib.getPluginCapabilitiesAsXML.restype = c_char_p
def getPluginCapabilitiesAsXML(pluginHandle):
    return rrpLib.getPluginCapabilitiesAsXML(pluginHandle)


#================ Plugin Parameter functionality ======================
## \brief Get plugin parameters for a specific capability. 
## \param pluginHandle Handle to a plugin
## \param capabilityName Pointer to a string, holding the name of a capability. If None, returna parameters in all capabilities.
## \return Returns available parameters for a particular capability in a plugin, None otherwise
## \ingroup plugins
rrpLib.getPluginParameters.restype = c_char_p
def getPluginParameters(pluginHandle, capabilityName):
    return rrpLib.getPluginParameters(pluginHandle, capabilityName)

## \brief Get a parameter handle to a parameter, located in a specific capability. If the capability argument is None
## the function will look into all capabilites and return the first parameter matching "parameterName".
## \param pluginHandle Handle to a plugin
## \param parameterName Name of the parameter
## \param capabilitiesName Name of a capability containing the parameter.
## \return Returns a handle to a parameter. Returns None if not found
## \ingroup plugins
def getPluginParameter(pluginHandle, parameterName, capabilitiesName = None):
    return rrpLib.getPluginParameter(pluginHandle, parameterName, capabilitiesName)

## \brief Set the value of a PluginParameter by a string.
## \param pluginHandle Handle to a plugin
## \param parameterName Name of parameter
## \param paraValue Value of parameter, as string
## \return true if succesful, false otherwise
## \ingroup plugins
def setPluginParameter(pluginHandle, parameterName, paraValue):
    return rrpLib.setPluginParameter(pluginHandle, parameterName, c_char_p(paraValue))

## \brief Create a parameter of type "type"
## \param name The parameters name as a string
## \param the_type  The parameters type as string. Possible values can be 'double', 'int', 'char*' etc,
## \return Returns a handle to a new parameter, if succesful, None otherwise
## \ingroup plugin_parameters
rrpLib.createParameter.restype = c_void_p
def createParameter(name, the_type):
    return rrpLib.createParameter(name, the_type, None)

## \brief Add a parameter to a parameters container, from a parameter pointer.
## \param listHandle Handle to a parameters container
## \param paraHandle Handle to a roadrunner parameter
## \return Returns a booelan indicating success
## \ingroup plugin_parameters
rrpLib.addParameterToList.restype = c_bool
def addParameterToList(listHandle, paraHandle):
    return rrpLib.addParameterToList(listHandle, paraHandle)

## \brief Set a parameter by a string
## \param paraHandle to a Parameter instance
## \param value Pointer to string holding the value to assign to the parameter, e.g. "0.01" to set a double to 0.01
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
def setParameterByString(paraHandle, value):
    return rrpLib.setParameterByString(paraHandle, value)

## \brief Set an int parameter
## \param paraHandle to a Parameter instance
## \param value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
def setIntParameter(paraHandle, value):
    return rrpLib.setIntParameter(paraHandle, c_int(value))

## \brief Set a double parameter
## \param paraHandle to a Parameter instance
## \param value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
#rrpLib.setDoubleParameter.args = [c_void_p, c_double]
def setDoubleParameter(paraHandle, value):
    return rrpLib.setDoubleParameter(paraHandle, c_double(value))

## \brief Set a string (char*) parameter
## \param paraHandle to a Parameter instance
## \param value Value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
def setStringParameter(paraHandle, value):
    return rrpLib.setStringParameter(paraHandle, c_char_p(value))

#def setEnumParameter(parameter, value):
#    return rrpLib.setIntParameter(parameter, value)

## \brief Get a parameters info
## \param paraHandle Handle to a parameter instance
## \return Returns informational text about the parameter if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterInfo.restype = c_char_p
def getParameterInfo(paraHandle):
    return rrpLib.getParameterInfo(paraHandle)

## \brief Get a parameters value as char*
## \param paraHandle to a Parameter instance
## \return Returns the parameters value if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterValueAsString.restype = c_char_p
def getParameterValueAsString(paraHandle):
    return rrpLib.getParameterValueAsString(paraHandle)

## \brief Get a handle to a parameters value
## \param paraHandle to a Parameter instance
## \return Returns a Handle to the parameters value if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterValueHandle.restype = c_void_p
def getParameterValueHandle(paraHandle):
    return rrpLib.getParameterValueHandle(paraHandle)

## \brief Get a parameters name
## \param paraHandle to a Parameter instance
## \return Returns the parameters name if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterName.restype = c_char_p
def getParameterName(paraHandle):
    return rrpLib.getParameterName(paraHandle)

## \brief Get a parameters hint
## \param paraHandle to a Parameter instance
## \return Returns the parameters hint if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterHint.restype = c_char_p
def getParameterType(paraHandle):
    return rrpLib.getParameterHint(paraHandle)

## \brief Get a parameters type
## \param paraHandle to a Parameter instance
## \return Returns the parameters type if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterType.restype = c_char_p
def getParameterType(paraHandle):
    return rrpLib.getParameterType(paraHandle)

## \brief Get the value of a parameter
## \param paraHandle A Handle to a parameter
## \return Returns the value of the parameter if succesful, None otherwise
## \note This function works only primitive data types, such as int, double, string etc.
## \ingroup plugin_parameters
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

## \brief Get a handle to a roadrunner object
## \param rrInstance A Python RoadRunner instance, as returned from roadrunner.RoadRunner()
## \return Returns a handle to a roadrunner instance that can be used as an argument to the Python Plugin API 
## library
## \ingroup utilities
def getRoadRunnerHandle(rrInstance):
    return cast(int(rrInstance.this), c_void_p)
    
## \brief Retrieve a handle to RoadRunners internal data object
## \param rrHandle Handle to a RoadRunner instance
## \return Returns an handle to roadrunners internal data object
## \ingroup utilities
def getRoadRunnerDataHandle(rrHandle):
    return rrpLib.getRoadRunnerDataHandle(rrHandle)

## \brief Get a handle to a roadrunner data object from a RoadRunner instance
## \param rrInstance A Python RoadRunner instance, as returned from roadrunner.RoadRunner()
## \return Returns a handle to a roadrunner data object that can be used as an argument to the Python Plugin API 
## library
## \ingroup utilities
rrpLib.getRoadRunnerDataHandle.restype = c_void_p
def getRoadRunnerDataHandleFromInstance(rrInstance):
    handle = getRoadRunnerHandle(rrInstance)
    return rrpLib.getRoadRunnerDataHandle(handle)

## \brief Create a Handle to a simplified roadrunner data object
## \param rrDataHandle A handle to a roadrunner data object, as returned from getRoadRunnerDataHandle()
## \return Returns a handle to a simplified roadrunner data object that can be used as an argument to the getNumpyData() function 
## \ingroup utilities
rrpLib.createRRCData.restype = c_void_p
def createRRCData(rrDataHandle):
    return rrpLib.createRRCData(rrDataHandle)

## \brief Create a string from a RoadRunner stringlist handle
## \param aList A handle to a roadrunner string list object
## \return Returns a string on success, None otherwise 
## \ingroup utilities
rrpLib.stringArrayToStringFWD.restype = c_char_p
def stringArrayToString(aList):
    return rrpLib.stringArrayToStringFWD(aList)

## \brief Convert roadrunner data to Numpy data
## \param rrcDataHandle A handle to a simplified roadrunner data object
## \return Returns a numpy data object
## \ingroup utilities
def getNumpyData(rrcDataHandle):
    rowCount = rrpLib.getRRDataNumRows(rrcDataHandle)
    colCount = rrpLib.getRRDataNumCols(rrcDataHandle)
    resultArray = np.zeros([rowCount, colCount])
    for row in range(rowCount):
        for col in range(colCount):
                value = c_double()
                if rrpLib.getRRCDataElementF(rrcDataHandle, row, col, pointer(value)) == True:
                    resultArray[row, col] = value.value
    return resultArray

## \brief Unload the plugins api shared library
## \ingroup utilities
def unloadAPI():
    windll.kernel32.FreeLibrary(rrpLib._handle)

    
##\mainpage Front page for RoadRunners PluginLib Python wrapper
#\section Introduction
#Roadrunners plugin library exposes a simple framework for adding functionality to RoadRunner core, by means of 
#external plugins.
#The code fragment below shows briefly how to load plugins, check for plugins, and use an individual plugin.
#
#@code
#import sys
#from rrPlugins import *
#
##Create a plugin manager
#pm = createPluginManager()
#
## Load plugins from the plugin folder
#result = loadPlugins(pm)
#if not result:
#    print 'Failed to load any plugins'
#    exit()
#
#print 'Number of Plugins: ' + `getNumberOfPlugins(pm)`
#namesHandle = getPluginNames(pm)
#names = stringArrayToString(namesHandle)
#print names
#
## Get a plugin handle
#aPlugin = getFirstPlugin(pm)
#
#if(aPlugin == None):
#    print 'Failed to get a handle to a plugin'
#    exit()
#
#info = getPluginInfo(aPlugin)
#
#print info
#print `unLoadPlugins(pm)`
#print "done"
#@endcode
#
# The above code produces the following output:
#@code
## *** Python 2.7.3 (default, Apr 10 2012, 23:31:26) [MSC v.1500 32 bit (Intel)] on win32. ***
## >>> 
## *** Remote Interpreter Reinitialized  ***
## >>> 
## Number of Plugins: 2
## AddNoise Levenberg-Marquardt
## Name..........................AddNoise
## Author........................Totte Karlsson
## Category......................Signal Processing
## Version.......................0.1
## Copyright.....................Totte Karlsson, Herbert Sauro, Systems Biology, UW 2012
## 
## True
## done
## >>> 
#@endcode
#    \section plugins_overview Overview
#    The libRoadRunner Plugin API is centered around three important concepts:
#    - A Plugin Manager (RRPluginManagerHandle)
#    - A Plugin (RRPlugin)
#    - A Plugin Parameter (RRParameter)
#
#    \section plugins_usage How to use plugins
#    A typical use case of the Plugin API may be as follows:
#
#    -# Client creates a PluginManager.
#    -# Client load plugins using the Plugin Manager.
#    -# Client get a handle to a plugin.
#    -# Client get a handle to a specific parameter in the plugin.
#    -# Client set the value of the parameter.
#    -# Client excutes the plugin.
#    -# Client retrieve the value of a plugins parameter, e.g. a "result" parameter.
#
#   See the examples page that provide exampl code on how to use plugins.
#    \section plugins_writing How to write plugins
#    \note Writing plugins in Python is not yet supported
#
# \section main_section Using rrPlugins.py
# In order to use this wrapper (rrPlugins.py), the Python path needs to inlcude the folder where the wrapper script is located, e.g.
# "c:\\roadrunner-1.0.0\\plugins\\python"
#
# \defgroup plugin_manager Plugin Manager
# \brief Plugin Manager Library Functions
# The Plugin manager loads and manages one or more plugins. Handles to individual plugins are obtained 
# by the getFirstPlugin, getNextPlugin etc functions, or the getPlugin(pluginName) function. 
#
# \defgroup plugins Plugin Functions
# \brief Functions operating on Plugin Handles.
#
# \defgroup plugin_parameters Plugin Parameters
# \brief Plugins Parameter related functions
#
# \defgroup utilities Utility Functions
# \brief Functions to help and assist in the use of the Plugins framework

# \defgroup examples Python Example Scripts
# \brief Scripts illuminating concepts regarding RoadRunner Plugins

## \example rrPluginTester.py
## This Example shows
## -# How to create a plugin manager
## -# Get Plugin Names
## -# Get a handle to a plugin
## -# Obtain some info from the plugin

## \example rrPluginParameterDemo.py
## This Example shows
## -# Get a handle to a parameter in a Plugin
## -# Obtain some info about the parameter
## -# Getting the value of the parameter
## -# Setting the value of the parameter

## \example rrPluginDocumentationDemo.py
## This Example shows
## -# Get a plugins Capabilities as XML
## -# Obtain and view a Plugins documentaiton as PDF (Need a system PDF reader)

## \example rrCallBackFunctionDemo.py
## This Example shows
## -# How to define Python callback functions and passing them to a plugin

