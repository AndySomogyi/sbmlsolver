##@Module rrPlugins
#This module allows access to the rrplugins_api.dll from python"""
import os
import sys
import numpy as np
import roadrunner
from ctypes import *

# Get current folder and construct an absolute path to
# the plugins.
dirPath = os.path.dirname(os.path.realpath(__file__))
gDefaultPluginsPath  = os.path.split(dirPath)[0]

sharedLib='rrplugins_c_api'
rrpLib=None
if sys.platform.startswith('win32'):
        sharedLib = sharedLib + '.dll'
        rrpLib=CDLL(sharedLib)
elif sys.platform.startswith('Linux'):
    sharedLib = sharedLib + '.dll'
    rrpLib = cdll.LoadLibrary(sharedLib)


# Experimental parameter object class
# Usage:
# p = rrPlugins.ParameterObject ("k1", "hint", 0.1)
# p.value = 0.5
# print p.name

class ParameterObject:
    _parameterHandle = -1

    def __getValue (self):
        return getParameterValue (self._parameterHandle)

    def __setValue (self, value):
       if type (value) is int:
           setIntParameter (self._parameterHandle, value)
       if type (value) is float:
           setDoubleParameter (self._parameterHandle, value)
       if type (value) is str:
           setStringParameter (self._parameterHandle, value)
    value = property (__getValue, __setValue)

    def __getName (self):
        return getParameterName(self._parameterHandle)
    name = property (__getName)

    def __getHint (self):
        return getParameterHint(self._parameterHandle)
    hint = property (__getHint)

    def __getType (self):
        return getParameterType(self._parameterHandle)
    pType = property (__getType)

    def __init__(self, name, hint, value):
      if type (value) is int:
        self._parameterHandle = createParameter (name, "int", hint)
      if type (value) is float:
        self._parameterHandle = createParameter (name, "double", hint)
      if type (value) is str:
        self._parameterHandle = createParameter (name, "string", hint)
      self.__setValue (value)

    def __getHandle (self):
        return self._parameterHandle

    handle = property (__getHandle)


#=======================rrp_api=======================#
#Type of plugin callback, first argument is return type
pluginCallBackType1  = CFUNCTYPE(None)
pluginCallBackType2  = CFUNCTYPE(None, POINTER(c_int), c_void_p)

## \brief Create a new instance of a plugin manager.
## \brief A PluginManager manages a collection of plugins, loaded and unloaded by
##  the load and unload API functions respectively.
## \param pluginDir Full path to folder containing plugins. If None, uses default folder.
## \return On success, a handle to a Plugin manager, on failure, None.
#
## @code pm = rrPlugins.createPluginManager ()
## @endcode
#
## \ingroup plugin_manager

rrpLib.createPluginManager.restype = c_void_p
def createPluginManager(pluginDir = None):
    if pluginDir == None:
        pluginDir = gDefaultPluginsPath
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
## \param pluginName Name of the plugin to load. The plugin name is the plugin's shared library name, without path and extension.
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

## \brief Get number of loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns the number of loaded plugins, -1 if a problem is encountered
## \ingroup plugin_manager
rrpLib.getNumberOfPlugins.restype = c_int
def getNumberOfPlugins(pm):
    return rrpLib.getNumberOfPlugins(pm)

## \brief Function to retrieve the names of all currently loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns names for all loaded plugins as a string, None otherwise
## \ingroup plugin_manager
rrpLib.getPluginNames.restype = c_void_p
def getPluginNames(pm):
    namesHandle = rrpLib.getPluginNames(pm)
    if not namesHandle:
        return list()
    names = stringArrayToString(namesHandle)
    return names.split(" ")

## \brief Function to retrieve the library names of all currently loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns library names for all loaded plugins as a string, None otherwise
## \ingroup plugin_manager
rrpLib.getPluginLibraryNames.restype = c_void_p
def getPluginLibraryNames(pm):
    namesHandle = rrpLib.getPluginLibraryNames(pm)
    if not namesHandle:
        return list()
    names = stringArrayToString(namesHandle)
    return names.split(" ")

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


## \brief Get the plugin handle for the named plugin
## \param pm Handle to a PluginManager instance
## \param pluginName A string that holds the name of the plugin
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

## \brief Returns information about a Plugin.
## \param pluginHandle Handle to a plugin
## \return Returns information as a string for the plugin, None otherwise
## \ingroup plugins
rrpLib.getPluginInfo.restype = c_char_p
def getPluginInfo(pluginHandle):
    return rrpLib.getPluginInfo(pluginHandle)

## \brief Get Plugin manual as PDF. A plugin may embedd a help manual as a PDF.
## Use the function getPluginManualNrOfBytes to get the exact length of this string.
## \param pluginHandle Handle to a plugin
## \return Returns the plugin's manual pdf file as a unsigned char*. If not available, returns None.
## \ingroup plugins
rrpLib.getPluginManualAsPDF.restype =  POINTER(c_ubyte)
def getPluginManualAsPDF(pluginHandle):
    return rrpLib.getPluginManualAsPDF(pluginHandle)

## \brief Get the byte size for the PDF manual.
## \param pluginHandle Handle to a plugin
## \return Returns the number of bytes in the plugin's manual pdf file as an unsigned int.
## \ingroup plugins
def getPluginManualNrOfBytes(pluginHandle):
    return rrpLib.getPluginManualNrOfBytes(pluginHandle)

## \brief Assign a roadrunner instance handle for the plugin to use.
##    A plugin may use an externally created roadrunner instance for its internal work.
##  \param pluginHandle Handle to a plugin
##  \param rrHandle Handle to a roadrunner instance
##  \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.assignRoadRunnerInstance.restype = c_bool
def assignRoadRunnerInstance(pluginHandle, rrHandle):
    return rrpLib.assignRoadRunnerInstance(pluginHandle, rrHandle)

## \brief The executePlugin function is called to start the plugin so that it can carry out its
## function. The call is plugin dependent meaning that it could result in a calculation, starting up a GUI etc.
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating success/failure
## \note The execute function is blocking, this means it won't returns to the caller until the task is complete.
## If the plugin is asked to carry out a lengthy calculation, consider using
## the executePluginEx function that has the option to execute the plugin code in the background (in a thread);
## \ingroup plugins
rrpLib.executePlugin.restype = c_bool
def executePlugin(pluginHandle):
    return rrpLib.executePlugin(pluginHandle)

## \brief The executePluginEx is similar to the executePlugin function, except it takes two extra arguments.
## \param pluginHandle Handle to a plugin
## \param userData void* pointer to user data. This will be plugin dependent. See specific plugin documentation for how to use this argument.
## \param inAThread bool indicating if the plugin should be executed in the background (in a thread)
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.executePlugin.restype = c_bool
def executePluginEx(pluginHandle, userData, inAThread=False):
    return rrpLib.executePluginEx(pluginHandle, c_void_p(userData), c_bool(inAThread))

## \brief Get status information from a plugin. This call is plugin dependent, see the plugin documentation for details
## \param pluginHandle Handle to a plugin
## \return Returns plugin status if available, as a string. None otherwise
## \ingroup plugins
rrpLib.getPluginStatus.restype = c_char_p
def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

## \brief Returns a plugins result, as a string. This is plugin dependent, and a plugin designer may, or may not, implement
## this function. See the plugin documentation for details.
## \note If a plugin wants to returns specific result, e.g. an Array, or a a float, these are better communicated troughthe use of Plugin Paarameters.
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
rrpLib.resetPlugin.restype = c_bool
def resetPlugin(pluginHandle):
    return rrpLib.resetPlugin(pluginHandle)

## \brief Check if a plugin is actively working. This function is used when the work in the plugin is 
## executed in a thread (see executeEx). The isPluginWorking will return true as long work is being active
## and false when the work is done. This is useful in UI environments. Also, see the various callback functions on 
## how to get status back from a plugin during its execution.
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating if the plugin is busy or not
## \ingroup plugins
rrpLib.isPluginWorking.restype = c_bool
def isPluginWorking(pluginHandle):
    return rrpLib.isPluginWorking(pluginHandle)

## \brief Terminate any work that is in progress in a plugin. If the plugins worker is executed in a thread, this function
## will signal the internals of the plugin to terminate. This function is used when a plugins work is executed in a thread.
## \param pluginHandle Handle to a plugin
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.terminateWork.restype = c_bool
def terminateWork(pluginHandle):
    return rrpLib.terminateWork(pluginHandle)

## \brief Check if the work of a plugin is currently being terminated. This function is useful when a plugin is executed in a thread. 
## \param pluginHandle Handle to the plugin
## \return Returns true or false indicating if the work within the plugin is in the process of being terminated
## \ingroup plugins
rrpLib.isBeingTerminated.restype = c_bool
def isBeingTerminated(pluginHandle):
    return rrpLib.isBeingTerminated(pluginHandle)

## \brief wasTerminated. Query a plugin if work was terminated succesfully. This function may be used in combination with
## the terminateWork, and isBeingTerminated functions.
## \param pluginHandle Handle to the plugin
## \return Returns true or false indicating if the work in the plugin was terminated or not
## \ingroup plugins
rrpLib.wasTerminated.restype = c_bool
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

## \brief Assign a callback function that will be fired when a plugin finishes its work
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
## \param userData void* pointer to user data. Plugin dependent, see the specific documentation on the plugin for details.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.setPluginInputData.restype = c_bool
def setPluginInputData(pluginHandle, userData):
    return rrpLib.setPluginInputData(pluginHandle, c_void_p(userData))

## \brief Get roadrunner instance handle from plugin
## \param pluginHandle Handle to a Plugin instance
## \return Returns a handle to a rrInstance if available, returns None otherwise
## \ingroup plugins
rrpLib.getRRHandleFromPlugin.restype = c_void_p
def getRRHandleFromPlugin(pluginHandle):
    return rrpLib.getRRHandleFromPlugin(pluginHandle)

## \brief Get a Plugin's category list as a string
## \param pluginHandle Handle to a plugin
## \return Returns the available categories for a particular plugin as a string, None otherwise.
## \ingroup plugins
rrpLib.getPluginCapabilities.restype = c_char_p
def getPluginCapabilities(pluginHandle):
    return rrpLib.getPluginCapabilities(pluginHandle)

## \brief Get a Plugins categories as a xml document. The string returned from this function is formated as xml.
## \param pluginHandle Handle to a plugin
## \return Returns the categories of a plugin as an XML string. None otherwise
## \ingroup plugins
rrpLib.getPluginCapabilitiesAsXML.restype = c_char_p
def getPluginCapabilitiesAsXML(pluginHandle):
    return rrpLib.getPluginCapabilitiesAsXML(pluginHandle)


#================ Plugin Parameter functionality ======================
## \brief Get plugin parameters for a specific category.
## \param pluginHandle Handle to a plugin
## \param categoryName Pointer to a string, holding the name of a category. If None, returna parameters in all categories.
## \return Returns available parameters for a particular category in a plugin, None otherwise
## \ingroup plugin_parameters
rrpLib.getPluginParameters.restype = c_char_p
def getPluginParameters(pluginHandle, capabilityName):
    return rrpLib.getPluginParameters(pluginHandle, capabilityName)

## \brief Get a parameter handle to a parameter, located in a specific category. If the category argument is None
## the function will look into all categories and return the first parameter matching "parameterName".
## \param pluginHandle Handle to a plugin
## \param parameterName Name of the parameter
## \param categoriesName Name of a category containing the parameter.
## \return Returns a handle to a parameter. Returns None if not found
## \ingroup plugin_parameters
def getPluginParameter(pluginHandle, parameterName, capabilitiesName = None):
    return rrpLib.getPluginParameter(pluginHandle, parameterName, capabilitiesName)

## \brief Set the value of a PluginParameter by a string.
## \param pluginHandle Handle to a plugin
## \param parameterName Name of parameter
## \param paraValue Value of parameter, as string
## \return true if succesful, false otherwise
## \ingroup plugin_parameters
rrpLib.setPluginParameter.restype = c_bool
def setPluginParameter(pluginHandle, parameterName, paraValue):
    return rrpLib.setPluginParameter(pluginHandle, parameterName, c_char_p(paraValue))

## \brief Create a parameter of type "type" with a name and hint property
## \param name The parameters name as a string
## \param the_type  The parameters type as string. Possible values: 'bool', 'int', 'double', 'string'
## \param hint  The parameters hint as a string.
## \return Returns a handle to a new parameter, if succesful, None otherwise
#
## @code
## parameterHandle = rrPlugins.createParameter ("k1", "double", "A rate constant")
## @endcode
#
## \ingroup plugin_parameters
rrpLib.createParameter.restype = c_void_p
def createParameter(name, the_type, hint):
#    if the_type == "string":
#        the_type = "char*"
    return rrpLib.createParameter(name, the_type, hint)

## \brief Add a parameter to a list of parameters.
## \param listHandle Handle to a parameter list
## \param paraHandle Handle to a parameter (see createParameter)
## \return Returns a Boolean indicating success
#
## @code
## paraList = getParameterValueHandle(paraHandle);
## para1 = createParameter("k1", "double", "A Hint")
## setDoubleParameter(para1, 0.2)
## addParameterToList(paraList, para1)
## @endcode
#
## \ingroup plugin_parameters
rrpLib.addParameterToList.restype = c_bool
def addParameterToList(listHandle, paraHandle):
    return rrpLib.addParameterToList(listHandle, paraHandle)

## \brief Set a parameter by a string
## \param paraHandle to a Parameter instance
## \param value Pointer to string holding the value to assign to the parameter, e.g. "0.01" to set a double to 0.01
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setParameterByString.restype = c_bool
def setParameterByString(paraHandle, value):
    return rrpLib.setParameterByString(paraHandle, value)

## \brief Set an integer parameter
## \param paraHandle to a Parameter instance
## \param value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setIntParameter.restype = c_bool
def setIntParameter(paraHandle, value):
    return rrpLib.setIntParameter(paraHandle, c_int(value))

## \brief Set the value for a double parameter
## \param paraHandle Is a parameter instance
## \param value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setDoubleParameter.restype = c_bool
def setDoubleParameter(paraHandle, value):
    return rrpLib.setDoubleParameter(paraHandle, c_double(value))

## \brief Set a string parameter
## \param paraHandle to a Parameter instance
## \param value Value to assign to the parameter.
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setStringParameter.restype = c_bool
def setStringParameter(paraHandle, value):
    return rrpLib.setStringParameter(paraHandle, c_char_p(value))

#def setEnumParameter(parameter, value):
#    return rrpLib.setIntParameter(parameter, value)

## \brief Get inforamtion on a parameter
## \param paraHandle Handle to a parameter instance
## \return Returns informational text about the parameter if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterInfo.restype = c_char_p
def getParameterInfo(paraHandle):
    return rrpLib.getParameterInfo(paraHandle)

## \brief Get a parameter value in the form of a string
## \param paraHandle to a Parameter instance
## \return Returns the parameters value if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterValueAsString.restype = c_char_p
def getParameterValueAsString(paraHandle):
    return rrpLib.getParameterValueAsString(paraHandle)

## \brief Get a handle to a parameter value
## \param paraHandle to a Parameter instance
## \return Returns a Handle to the parameter value if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterValueHandle.restype = c_void_p
def getParameterValueHandle(paraHandle):
    return rrpLib.getParameterValueHandle(paraHandle)

## \brief Get the name of a parameter
## \param paraHandle to a Parameter instance
## \return Returns the parameters name if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterName.restype = c_char_p
def getParameterName(paraHandle):
    return rrpLib.getParameterName(paraHandle)

## \brief Get a parameter type
## \param paraHandle to a Parameter instance
## \return Returns the parameters type if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterHint.restype = c_char_p
def getParameterHint(paraHandle):
    return rrpLib.getParameterHint(paraHandle)

## \brief Get the type of a parameter
## \param paraHandle to a Parameter instance
## \return Returns the parameters type if successful, None otherwise
## \ingroup plugin_parameters
rrpLib.getParameterType.restype = c_char_p
def getParameterType(paraHandle):
    return rrpLib.getParameterType(paraHandle)

## \brief Get the value of a parameter no matter what type it is
## \param paraHandle A Handle to a parameter
## \return Returns the value of the parameter if succesful, None otherwise
## \note This function works only primitive data types, such as int, double, and string.
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
        return getParameterValueAsString(paraHandle)
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
#rrpLib.getRRCDataElementF.args =[c_void_p, c_int, c_int, POINTER(c_double)]
rrpLib.getRRCDataElementF.restype = c_bool
def getNumpyData(rrcDataHandle):
    rowCount = rrpLib.getRRDataNumRows(rrcDataHandle)
    colCount = rrpLib.getRRDataNumCols(rrcDataHandle)
    resultArray = np.zeros([rowCount, colCount])
    for row in range(rowCount):
        for col in range(colCount):
                val = c_double()
                if rrpLib.getRRCDataElementF(rrcDataHandle, row, col, byref(val)) == True:
                    resultArray[row, col] = val.value
                else:
                    print "problem"
    return resultArray

#Note, a hard bug in the above function was the initial absence of the c_bool restype. Removing that, make the function works up to 128 rows,
#and after that it will fail!!

## \brief Get a individual data element from a RRCData structure
## \param rrcDataHandle A handle to a simplified roadrunner data object (RRCData)
## \param r A row index
## \param c A col index
## \param value A double value
## \return Returns true or false
## \ingroup utilities

def getRRCDataElement(rrcDataHandle, r, c):
    val = c_double()
    if rrpLib.getRRCDataElementF(rrcDataHandle, r, c, byref(val)) == False:
        return float(NaN)
    else:
        return val.value


## \brief Get last (API) error. This returns the last error if any.
## \return Returns a string with an error success, None otherwise
## \ingroup utilities
rrpLib.getLastPluginError.restype = c_char_p
def getLastError():
    return rrpLib.getLastPluginError()

## \brief Unload the plugins api shared library
## \ingroup utilities
def unLoadAPI():
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
## #Create a plugin manager
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
## Version.......................1.0
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
# The plugin system supports parameter objects, these objects contain a variety of information about a given parameter, these include:
# name, value, type, hint, and a description. The following types are curerntly supported, Booleans, integers, doubles, strings and
# lists of strings. Parameters are also grouped into convenient categories.

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
## -# Get a plugin's categories in the form of an XML string
## -# Obtain and view a Plugin's documentation as a PDF (Needs a system PDF reader)

## \example rrCallBackFunctionDemo.py
## This Example shows
## -# How to define Python callback functions and passing them to a plugin

## \example rrNoisePluginDemo.py
## This Example Demonstrate the use of the AddNoise plugin

## \example rrLevenbergMarquardtDemo.py
## This Example Demonstrate the use of the Minimization Plugin, using the Levenberg-Marquardt algorithm.
