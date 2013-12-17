##@Module rrPlugins
#This module allows access to the rrplugins_api.dll from python"""
import os
import sys
import numpy as np
import roadrunner
import tempfile
import time
from ctypes import *

"""
CTypes Python Bindings to the RoadRunner Plugin API.

Currently this is a fairly raw implementation with few Pythonic refinements
"""

__version__ = "1.0.0"

# Get current folder and construct an absolute path to
# the plugins.
dirPath = os.path.dirname(os.path.realpath(__file__))
gDefaultPluginsPath  = os.path.split(dirPath)[0]

sharedLib='rrplugins_c_api'
rrpLib=None
try:
    if sys.platform.startswith('win32'):
        sharedLib = sharedLib + '.dll'
        rrpLib=CDLL(sharedLib)
    elif sys.platform.startswith('Linux'):
        sharedLib = sharedLib + '.a'
        rrpLib = cdll.LoadLibrary(sharedLib)
except:
    print 'ERROR: Unable to locate shared library: ' + sharedLib
    exit()


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


#=======================rrp_api========================#
#Type of plugin callbacks, first argument is return type

## \brief Plugin Function callback type definition
## This is a helper object that a client can use as an argument to a roadrunner plugin.
## The exact number of plugins callback functions required arguments, and their type, is plugin dependent. A client of the
## the plugin need to get this information from plugin specific documentation. 
## An example of using this particular function, pluginCallbackType1 is shown below. As can see, this python function 
##don't take any arguments.
##@code
##def myPluginFunction():
##    print 'The plugin can call this function!'
## #After loading the plugin, the user can assign this function as a plugin callback as follows
## cb_func1 =  pluginCallBackType1(myPluginFunction)
## assignPluginStartedCallBack(plugin,  cb_func1)
##@endcode
pluginCallBackType1  = CFUNCTYPE(None)

## \brief Plugin Function callback type definition
## This is a helper object that a client can use as an argument to a roadrunner plugin.
## The exact number of plugins callback functions required arguments, and their type, is plugin dependent. A client of the
## the plugin need to get this information from plugin specific documentation. 
## An example of using this particular function, pluginCallbackType2 is shown below. As can see, this python function 
## do take two arguments. The first argument is an integer, indicating progress, the second argument is not used in this particular case.
##@code
##def pluginIsProgressing(progress, dummy):
##    nr = progress[0]
##    print '\nPlugin progress:' + `nr` +' %'
## #After loading the plugin, the user can assign this function as a plugin callback as follows
## cb_func2 =  pluginCallBackType1(pluginIsProgressing)
## assignPluginStartedCallBack(plugin,  cb_func2)
##@endcode
pluginCallBackType2  = CFUNCTYPE(None, POINTER(c_int), c_void_p)

## \brief Create a new instance of a plugin manager.
## \brief A PluginManager manages a collection of plugins, loaded and unloaded by
##  the load and unload API functions respectively.
## \param pluginDir Full path to folder containing plugins. If None, uses default folder.
## \return On success, a handle to a Plugin manager, on failure, None.
#
## @code
## pm = rrPlugins.createPluginManager()
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
## \ingroup plugin_manager

rrpLib.createPluginManager.restype = c_void_p
def createPluginManager(pluginDir = None):
    if pluginDir == None:
        pluginDir = gDefaultPluginsPath
    return rrpLib.createPluginManager(pluginDir)

## \brief Free the plugin manager. A call to this function will also unload any loaded plugins.
## \param pm Handle to a plugin manager.
## \return true if success, false otherwise.
##
## \ingroup plugin_manager
rrpLib.freePluginManager.restype = c_bool
def freePluginManager(pm):
    return rrpLib.freePluginManager(pm)

##
## \brief Load plugins. The function will look in the default plugin folder for plugins, and load them.
## \param pm Handle to a PluginManager instance
## \return Returns true if Plugins are loaded, false otherwise
##
## @code sucess = rrPlugins.loadPlugins (pm) 
## @endcode
## \htmlonly  <br/> 
## \endhtmlonly 
## \ingroup plugin_manager
##
rrpLib.loadPlugins.restype = c_bool
def loadPlugins(pm):
    return rrpLib.loadPlugins(pm)

##
## \brief Unload plugins
## \param pm Handle to a PluginManager instance
## \return Returns true if Plugins are unloaded succesfully, false otherwise
##
## \ingroup plugin_manager
##
rrpLib.unLoadPlugins.restype = c_bool
def unLoadPlugins(pm):
    return rrpLib.unLoadPlugins(pm)

##
## \brief Load a particular plugin
## \param pm Handle to a PluginManager instance
## \param pluginName Name of the plugin to load. The plugin name is the plugin's shared library name, without path and extension. 
## The library names for the plugins can be obtained by calling getPluginNames()
## \return Returns a handle to a plugin, None if unsuccesful
##
## @code
## lmPlugin = rrPlugins.loadPlugin(pm, "rrp_lm")
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
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
## \return Returns the number of loaded plugins, -1 if a problem is encountered. Call rrPlugins.getLastError() to obtain error message.
## \ingroup plugin_manager
rrpLib.getNumberOfPlugins.restype = c_int
def getNumberOfPlugins(pm):
    return rrpLib.getNumberOfPlugins(pm)

## \brief Function to retrieve the names of all currently loaded plugins.
## \param pm Handle to a PluginManager instance
## \return Returns names for all loaded plugins as a string, None otherwise
##
## @code
## names = rrPlugins.getPluginNames(pm)
## print names
## ['AddNoise', 'Levenberg-Marquardt']
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
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
##
## @code
## names = rrPlugins.getPluginLibraryNames(pm)
## print names
## ['rrp_add_noise', 'rrp_lm']
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
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
## \return Returns the internal name of a plugin. None otherwise
##
## @code
## name = rrPlugins.getPluginName(pluginHandle)
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
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
##
## @code
## ptr = getPluginManualAsPDF(pluginHandle)
## numOfBytes = getPluginManualNrOfBytes(pluginHandle)
## manual = cast(ptr, POINTER(c_char * numOfBytes))[0]
## if numOfBytes == 0:
##    print 'This plugin does not have a manual.'
##    exit()
## outFName = getPluginName (pluginHandle) + '.pdf'
## with open(outFName, 'wb') as output:
##    output.write(manual)
## os.system('start ' + outFName)
##
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
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


## \brief If a plugin has a built-in PDF manual, display it.
## \param pluginHandle Handle to a plugin
## \return Returns False is the plugin has no manual
##
## @code
## success = displayPluginManual(pluginHandle)
##
## @endcode 
## \htmlonly  <br/> 
## \endhtmlonly 
## \ingroup plugins
def displayPluginManual(pluginHandle):
    ptr = getPluginManualAsPDF(pluginHandle)
    numOfBytes = getPluginManualNrOfBytes(pluginHandle)
    manual = cast(ptr, POINTER(c_char * numOfBytes))[0]
    if numOfBytes == 0:
       print 'This plugin does not have a manual.'
       return False
    outFName = tempfile.gettempdir() + '\\' + getPluginName (pluginHandle) + '.pdf'
    print outFName
    with open(outFName, 'wb') as output:
      output.write(manual)
    os.system('start ' + outFName) 
    
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
## \param inAThread bool indicating if the plugin should be executed in the background (in a thread)
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.executePluginEx.restype = c_bool
def executePluginEx(pluginHandle, inAThread=False):
    return rrpLib.executePluginEx(pluginHandle, c_bool(inAThread))

## \brief Get status information from a plugin. This call is plugin dependent, see the plugin documentation for details
## \param pluginHandle Handle to a plugin
## \return Returns plugin status if available, as a string. None otherwise
## \ingroup plugins
rrpLib.getPluginStatus.restype = c_char_p
def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

## \brief Returns a plugins result, as a string. This is plugin dependent, and a plugin designer may, or may not, implement
## this function. See the plugin documentation for details.
## \note If a plugin wants to returns specific result, e.g. an Array, or a a float, these are better communicated 
## through the use of Plugin Parameters.
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

## \brief Hand external data to a plugin: THIS METHOD IS UNDER REVIEW
## \param pluginHandle Handle to a plugin
## \param userData void* pointer to user data. Plugin dependent, see the specific documentation on the plugin for details.
## \return Returns true or false indicating success/failure
## \ingroup plugins
rrpLib.assignPluginInput.restype = c_bool
def assignPluginInput(pluginHandle, userData):
    return rrpLib.assignPluginInput(pluginHandle, c_void_p(userData))

## \brief Get the roadrunner instance handle from plugin assuming the plugin has one
## \param pluginHandle Handle to a Plugin instance
## \return Returns a handle to a rrInstance if available, returns None otherwise
## \ingroup plugins
rrpLib.getRRHandleFromPlugin.restype = c_void_p
def getRRHandleFromPlugin(pluginHandle):
    return rrpLib.getRRHandleFromPlugin(pluginHandle)

#================ Plugin Parameter functionality ======================
## \brief Get a handle to the list of parameters for a plugin
## \param pluginHandle Handle to a plugin
## \return Returns available parameters for a particular, None otherwise
## \ingroup plugin_parameters
rrpLib.getPluginParameters.restype = c_void_p
def getPluginParameters(pluginHandle):
    return rrpLib.getPluginParameters(pluginHandle)

## \brief Get a list of parameter names in a plugin
## \param pluginHandle Handle to a plugin
## \return Returns a list of parameter names, None otherwise
## \ingroup plugin_parameters
rrpLib.getListOfPluginParameterNames.restype = c_char_p
def getListOfPluginParameterNames(pluginHandle):
    paraNames =  rrpLib.getListOfPluginParameterNames(pluginHandle)
    if not paraNames:
        return list()
    else:        
        names = paraNames.split(',')     
        return names 

## \brief Clear a list of parameters 
## \param parasHandle Handle to a list of parameters
## \return True or false, indicating result. Observe there are parameterLists that are not 'clearable', e.g. a plugins primary parameterList
## \ingroup plugin_parameters
rrpLib.clearParameterList.restype = c_bool
def clearParameterList(parameterListHandle):
    return rrpLib.clearParameterList(parameterListHandle)

## \brief Get a list of a plugins parameter names
## \param pluginHandle Handle to a plugin
## \return Returns names for all parameters in the plugin
## \ingroup plugin_parameters
rrpLib.getNamesFromParameterList.restype = c_char_p
def getNamesFromParameterList(paraHandle):
    paras = rrpLib.getNamesFromParameterList(paraHandle)
    if not paras:
        return list()
    else:        
        names = paras.split(',')     
        return names            

## \brief Get a plugins properties as formatted xml 
## \param pluginHandle Handle to a plugin
## \return Returns a string on success, None otherwise
## \ingroup plugin_parameters
rrpLib.getPluginPropertiesAsXML.restype = c_char_p
def getPluginPropertiesAsXML(pluginHandle):
    return rrpLib.getPluginPropertiesAsXML(pluginHandle)

## \brief Get the 'first' parameter handle to a parameter in a parameters list
## \param paraListHandle Handle to a parameterList
## \return Returns a handle to a parameter. Returns None if not found
## \ingroup plugin_parameters
def getFirstParameter(paraListHandle):
    return rrpLib.getFirstParameter(paraListHandle)

## \brief Get 'next' parameter handle to a parameter in a parameters list
## \param paraListHandle Handle to a parameterList
## \return Returns a handle to a parameter. Returns None if not found
## \ingroup plugin_parameters
def getNextParameter(paraListHandle):
    return rrpLib.getNextParameter(paraListHandle)

## \brief Get a parameter handle to a parameter, with a specific name.
## \param pluginHandle Handle to a plugin
## \param parameterName Name of the parameter
## \return Returns a handle to a parameter. Returns None if not found
## \ingroup plugin_parameters
def getPluginParameter(pluginHandle, parameterName):
    return rrpLib.getPluginParameter(pluginHandle, parameterName)

## \brief Set the value of a PluginParameter by a string.
## \param pluginHandle Handle to a plugin
## \param parameterName Name of parameter
## \param paraValue Value of parameter, as string
## \return true if succesful, false otherwise
## \ingroup plugin_parameters
rrpLib.setPluginParameter.restype = c_bool
def setPluginParameter(pluginHandle, parameterName, paraValue):
    return rrpLib.setPluginParameter(pluginHandle, parameterName, c_char_p(paraValue))

## \brief Set a parameters description
## \param paraHandle to a Parameter instance
## \param descr String holding the description
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setParameterDescription.restype = c_bool
def setParameterDescription(paraHandle, descr):
    return rrpLib.setParameterDescription(paraHandle, descr)

## \brief Set a parameters Hint property
## \param paraHandle to a Parameter instance
## \param descr String holding the hint text
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.setParameterHint.restype = c_bool
def setParameterHint(paraHandle, descr):
    return rrpLib.setParameterHint(paraHandle, descr)

    
    
## \brief Create a parameter of type "type" with a name and hint property
##  Valid types include: 'bool', 'int', 'double', 'string', and 'listOfParameters'
## \param name The parameters name as a string
## \param the_type  The parameters type as string. Possible values: 'bool', 'int', 'double', 'string', and 'listOfParameters'
## \param hint The parameters hint as a string.
## \param value This is an optional argument to set the parameter (supports int, double and string).
## \return Returns a handle to a new parameter, if succesful, None otherwise
##
## @code
## parameterHandle = rrPlugins.createParameter ("k1", string", "A message")
##  
##parameterHandle = rrPlugins.createParameter ("k1", "double", "A rate constant", 0.3)
## @endcode
## \htmlonly  <br/> 
## \endhtmlonly 
## \ingroup plugin_parameters
rrpLib.createParameter.restype = c_void_p
def createParameter(name, the_type, hint, value=None):    
    if value == None:
       return rrpLib.createParameter(name, the_type, hint, value)
    else:
        ptr = rrpLib.createParameter(name, the_type, hint)
        if the_type is "bool":
           setBoolParameter (ptr, value)
        elif the_type is "int":
           setIntParameter (ptr, value)
        elif the_type is "float":
           setDoubleParameter (ptr, value)
        elif the_type is "double":
           setDoubleParameter (ptr, value)           
        elif the_type is "string":
           setStringParameter (ptr, value)
        else:
            print "Error: Can't set the value of this parameter!"     
        return ptr     

## \brief Free memory for a parameter
## \param paraHandle Handle to a Parameter instance
## \return Returns true if successful, false otherwise
## \ingroup plugin_parameters
rrpLib.freeParameter.restype = c_bool
def freeParameter(paraHandle):
    return rrpLib.freeParameter(paraHandle)


## \brief Add a parameter to a list of parameters. The handle to the parameter list can be obtained using getParameterValueHandle.
## \param listHandle Handle to a parameter list
## \param paraHandle Handle to a parameter (see createParameter)
## \return Returns a Boolean indicating success
##
## @code
## paraList = getParameterValueHandle(paraHandle);
## if rrPlugins.getParameterType (paraList) != "listOfParameters":
##    print "Parameter is not a list"
##    exit()
## para1 = createParameter("k1", "double", "A Hint", 0.2)
## addParameterToList(paraList, para1)
## @endcode
## \htmlonly  <br/> 
## \endhtmlonly 
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

## \brief Get a handle to a parameter value. Such parameters could be any type, including a list of parameters.
## \param paraHandle to a Parameter instance
## \return Returns a Handle to the parameter value if successful, None otherwise
#
## @code
## parameterHandle = rrPlugins.getParameterValueHandle (parhandle)
## @endcode
## \htmlonly  <br/> 
## \endhtmlonly 
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
    if paraType == 'bool':
        paraVoidPtr = getParameterValueHandle(paraHandle)
        ptr = cast(paraVoidPtr, POINTER(c_bool))
        return ptr[0]
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
rrpLib.getRoadRunnerDataElement.restype = c_bool
def getNumpyData(rrcDataHandle):
    rowCount = rrpLib.getRoadRunnerDataNumRows(rrcDataHandle)
    colCount = rrpLib.getRoadRunnerDataNumCols(rrcDataHandle)
    resultArray = np.zeros([rowCount, colCount])
    for row in range(rowCount):
        for col in range(colCount):
                val = c_double()
                if rrpLib.getRoadRunnerDataElement(rrcDataHandle, row, col, byref(val)) == True:
                    resultArray[row, col] = val.value
                else:
                    print "problem"
    return resultArray

#Note, a hard bug in the above function was the initial absence of the c_bool restype. Removing that, make the function works up to 128 rows,
#and after that it will fail!!



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
#\include rrPluginTester.py
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
# The basic approach to using the plugin system is:
#
# 1. Create a plugin manager first
#
# 2. Load the plugins using loadPlugins (all) or loadPlugin (specific one)
# 
# 3. Obtain the plugin handle using getPlugin or directly from loadPlugin (singular)
#
# 4. Using the plugin handle, set values to the plugin parameters
#
# 5. Run the plugin method ExecutePlugin
#
# 6. Retrieve results from plugin parameters
# \defgroup plugins Plugin Functions
# \brief Functions operating on Plugin Handles.
#
# \defgroup plugin_parameters Plugin Parameters
# \brief Plugins Parameter related functions
# The plugin system supports parameter objects, these objects contain a variety of information about a given parameter, these include:
# the name, value, type, hint, and a description. The following types are currently supported, Booleans, integers, doubles, strings,
# list of parameter objects and the roadRunner data array format. Parameters can also grouped into convenient categories which can be useful 
# for GUI applications. A plugin normally exposes the parameters to a user, however it is also possible to create new parameters 
# using the createParameter call. Such parameters are usally used to populate a plugin parameter that is a list of
# other parameters using the method addParameterToList. Note that the plugin system only supports two layers of parameter. 
# In the followng a paraHandle points to a single parameter object.
#
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

