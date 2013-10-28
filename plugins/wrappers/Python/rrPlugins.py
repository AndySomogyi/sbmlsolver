##@Module rrPython
#This module allows access to the rr_c_api.dll from python"""

import sys
import os
#import ctypes
import numpy
from ctypes import *

np = numpy

if len(os.path.dirname(__file__)):
    os.chdir(os.path.dirname(__file__))
sharedLib=''
rrLib=None
libHandle=None
if sys.platform.startswith('win32'):
    rrInstallFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'bin'))
    os.environ['PATH'] = rrInstallFolder + ';' + "c:\\Python27" + ';' + "c:\\Python27\\Lib\\site-packages" + ';' + os.environ['PATH']
    sharedLib = os.path.join(rrInstallFolder, 'rrp_api.dll')
    libHandle=windll.kernel32.LoadLibraryA(sharedLib)
    rrLib = WinDLL (None, handle=libHandle)

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
    rrLib = cdll.LoadLibrary(sharedLib)


##\mainpage notitle
#\section Introduction
#RoadRunner is a high performance and portable simulation engine for systems and synthetic biology. To run a simple SBML model and generate time series data we would call:
#
#@code
#
#import rrPython
#
#rrPython.loadSBMLFromFile('C:\\Models\\mymodel.xml')
#
#rrPython.simulate()
#@endcode
#
#\section Setup
#In order to import the python module, the python folder within the roadRunner install folder must be in the system's python path. To make sure it is, do the following in Windows:
#
#Open the control panel and click on 'System'
#The following will appear; click on 'Advanced System Settings'
#\image html http://i.imgur.com/bvn9c.jpg
#
#Click on the 'Environment Variables' button highlighted in the image below
#\image html http://i.imgur.com/jBCfn.jpg
#
#Highlight the python path entry and click edit. The prompt shown below will appear. Enter the location of the python folder within the install folder with a semicolon between any other entries.
#\image html http://i.imgur.com/oLC32.jpg

##\defgroup initialization Library initialization and termination methods
# \brief Initialize library and terminate library instance
#
# \defgroup loadsave Read and Write models
# \brief Read and write models to files or strings. Support for SBML formats.
#
# \defgroup utility Utility functions
# \brief Various miscellaneous routines that return useful information about the library
#
# \defgroup errorfunctions Error handling functions
# \brief Error handling routines
#
# \defgroup state Current state of system
# \brief Compute derivatives, fluxes, and other values of the system at the current state
#
# \defgroup steadystate Steady State Routines
# \brief Compute and obtain basic information about the steady state
#
# \defgroup reaction Reaction group
# \brief Get information about reaction rates
#
# \defgroup rateOfChange Rates of change group
# \brief Get information about rates of change
#
# \defgroup boundary Boundary species group
# \brief Get information about boundary species
#
# \defgroup floating Floating species group
# \brief Get information about floating species
#
# \defgroup initialConditions Initial conditions group
# \brief Set or get initial conditions
#
# \defgroup parameters Parameter group
# \brief Set and get global and local parameters
#
# \defgroup compartment Compartment group
# \brief Set and Get information on compartments
#
# \defgroup simulation Time-course simulation
# \brief Deterministic, stochastic, and hybrid simulation algorithms
#
# \defgroup mca Metabolic Control Analysis
# \brief Calculate control coefficients and sensitivities
#
# \defgroup stoich Stoichiometry analysis
# \brief Linear algebra based methods for analyzing a reaction network
#
# \defgroup helperRoutines Helper Routines
# \brief Helper routines for acessing the various C API types, eg lists and arrays
#
# \defgroup toString ToString Routines
# \brief Render various result data types as strings
#
# \defgroup freeRoutines Free memory routines
# \brief Routines that should be used to free various data structures generated during the course of using the library


#=======================rr_c_api=======================#
charptr = POINTER(c_char)

rrpLib.createRRPluginManager.restype = c_void_p
rrpLib.createRRPluginManagerEx.restype = c_void_p
rrpLib.freeRRPluginManager.restype = c_bool

#===== The Python API allocate an internal global handle to ONE instance of the Roadrunner API
gHandle = rrpLib.createRRInstance()
gPluginManager = rrpLib.createRRPluginManager(gHandle)


#Plugin functionality
rrpLib.loadPlugins.restype = c_bool
rrpLib.unLoadPlugins.restype = c_bool
rrpLib.getNumberOfPlugins.restype = c_int
rrpLib.getPluginInfo.restype = c_char_p
rrpLib.executePlugin.restype = c_bool


#Unload roadrunner dll from python
def unloadAPI():
    del gHandle
    return windll.kernel32.FreeLibrary(libHandle)

def freeAPI():
    return windll.kernel32.FreeLibrary(libHandle)



def loadPlugin(libraryName):
    return rrpLib.loadPlugin(gPluginManager, libraryName)

def loadPlugins():
    return rrpLib.loadPlugins(gPluginManager)

def getPluginNames():
    return rrpLib.getPluginNames(gPluginManager)

def unLoadPlugins():
    return rrpLib.unLoadPlugins(gPluginManager)

def getNumberOfPlugins():
    return rrpLib.getNumberOfPlugins(gPluginManager)

rrpLib.getPluginStatus.restype = c_char_p
def getPluginInfo(pluginHandle):
    return rrpLib.getPluginInfo(pluginHandle)

def getPluginStatus(pluginHandle):
    return rrpLib.getPluginStatus(pluginHandle)

def executePlugin(pluginHandle):
    return rrpLib.executePlugin(pluginHandle)

def executePluginEx(pluginHandle, userData):
    return rrpLib.executePluginEx(pluginHandle, c_void_p(userData))

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


