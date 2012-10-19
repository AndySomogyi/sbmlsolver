##@Module rrPython
#
#This module allows access to the rr_c_api.dll from python"""

import sys
import os
from ctypes import *
os.chdir(os.path.dirname(__file__))
rrInstallFolder = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'bin'))
os.environ['PATH'] = rrInstallFolder + ';' + "c:\\Python27" + ';' + "c:\\Python27\\Lib\\site-packages" + ';' + os.environ['PATH']
handle = WinDLL (rrInstallFolder + "\\rr_c_api.dll")
from numpy import *

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
rr = handle.getRRInstance()

#Utility and informational methods
handle.getVersion.restype = c_char_p
handle.getBuildDate.restype = c_char_p
handle.getCopyright.restype = c_char_p
handle.setTempFolder.restype = c_bool
handle.getTempFolder.restype = c_char_p
##\ingroup utility
#@{

##\brief Retrieve the current version number of the library
#\return char* version - Returns null if it fails, otherwise it returns the version number of the library
def getVersion():
    return handle.getVersion()

##\brief Retrieve the current build date of the library
#\return Returns null if it fails, otherwise it returns the build date
def getBuildDate():
    return handle.getBuildDate()

##\brief Retrieve the current copyright notice for the library
#\return Returns null if it fails, otherwise it returns the copyright string
def getCopyright():
    return handle.getCopyright()

##\brief Sets the write location for the temporary file
#
#When cRoadRunner is run in C generation mode its uses a temporary folder to store the
#generate C source code. This method can be used to set the temporary folder path if necessary.
#
#\return Returns true if succcessful
def setTempFolder(folder):
    return handle.setTempFolder(folder)

##\brief Returns the full path of the temporary folder
#
#When cRoadRunner is run in C generation mode its uses a temporary folder to store the
#generate C source code. This method can be used to get the current value
#for the the temporary folder path.
#
#\return Returns null if it fails, otherwise it returns the path
def getTempFolder():
    return handle.getTempFolder()

##\brief Retrieve a handle for the C code structure, RRCCode
#
#When cRoadRunner is run in C generation mode its uses a temporary folder to store the
#generate C source code. This method can be used to obtain the header and main source
#code after a model has been loaded by using the helper routines (getCCodeSource and getCCodeHeader())
#
#\return Returns null if it fails, otherwise it returns a pointer to the RRCode structure
def getCCode():
    return handle.getCCode()

##@}

#Logging
handle.enableLogging.restype = c_bool
handle.setLogLevel.restype = c_bool
handle.getLogLevel.restype = c_char_p
handle.getLogFileName.restype = c_char_p
handle.hasError.restype = c_bool
handle.getLastError.restype = c_char_p
handle.freeRRInstance.restype = c_bool

##\ingroup errorfunctions
#@{

##\brief Enables logging
#\return Returns true if succesful
def enableLogging():
    return handle.enableLogging()

##\brief Set the logging status level
#The logging level is determined by the following strings
#
#"ANY", "DEBUG5", "DEBUG4", "DEBUG3", "DEBUG2", "DEBUG1",
#"DEBUG", "INFO", "WARNING", "ERROR"
#
#Example: setLogLevel ("DEBUG4")
#
#\param lvl The logging level string
#\return Returns true if succesful
def setLogLevel(lvl):
    return handle.setLogLevel(lvl)

##\brief Returns the log level as a string
#The logging level can be one of the following strings
#
#"ANY", "DEBUG5", "DEBUG4", "DEBUG3", "DEBUG2", "DEBUG1",
#"DEBUG", "INFO", "WARNING", "ERROR"
#
#Example: str = rrPython.getLogLevel ()
#\return Returns False is it fails else returns the logging string
def getLogLevel():
    return handle.getLogLevel()

##\brief Returns the name of the log file
#\return Returns False if it fails else returns the full path to the logging file name
def getLogFileName():
    return handle.getLogFileName()

##\brief Check if there is an error string to retrieve
#
#Example: status = rrPython.hasError()
#
#\return status - Returns true if there is an error waiting to be retrieved
def hasError():
    return handle.hasError()

##\brief Returns the last error
#
#Example: str = rrPython.getLastError()
#
#\return Returns false if it fails, otherwise returns the error string
def getLastError():
    return handle.getLastError()

##\brief Initialize the roadRunner library and return an instance
#\return Returns an instance of the library, returns false if it fails
def getRRInstance():
    return handle.getRRInstance()

##\brief Free the roadRunner instance
#\param handle Free the roadRunner instance given in the argument
def freeRRInstance(handle):
    return handle.freeRRInstance(handle)

##@}

#Flags/Options
handle.setComputeAndAssignConservationLaws.restype = c_bool

##\ingroup utility
#@{

##\brief Enable/disable conservation analysis
#\param OnOrOff Set to 1 to switch on conservation analysis, 0 to switch it off
#\return Returns True if successful
def setComputeAndAssignConservationLaws(OnOrOff):
    return handle.setComputeAndAssignConservationLaws(OnOrOff)

##@}

#Load SBML methods
handle.loadSBML.restype = c_bool
handle.loadSBMLFromFile.restype = c_bool
handle.getCurrentSBML.restype = c_char_p
handle.getSBML.restype = c_char_p

##\ingroup loadsave
#@{

##\brief Create a model from an SBML string
#\param[in] sbml string
#\return Returns true if successful
def loadSBML(sbml):
    return handle.loadSBML(sbml)

##\brief Loads SBML model from a file
#\param fileName file name
#\return Returns true if successful
def loadSBMLFromFile(fileName):
    return handle.loadSBMLFromFile(fileName)

##\brief Return the current state of the model in the form of an SBML string
#\return Returns False if it fails or no model is loaded, otherwise returns the SBML string.
def getCurrentSBML():
    return handle.getCurrentSBML()

##\brief Retrieve the last SBML model that was loaded
#\return Returns False if it fails or no model is loaded, otherwise returns the SBML string
def getSBML():
    return handle.getSBML()

##@}

#SBML utility methods
handle.getParamPromotedSBML.restype = c_char_p

##\ingroup parameters
#@{

##\brief Promote any local parameters to global status
#\param sArg The string containing SBML model to promote
#\return Returns False if it fails, otherwise it returns the promoted SBML string
def getParamPromotedSBML(sArg):
    value = c_char_p(sArg)
    if handle.getParamPromotedSBML(byref(value)) == True:
        return value.value
    else:
        raise RuntimeError('Index out of range')

##@}

#More Utility Methods
handle.setCapabilities.restype = c_bool
handle.getCapabilities.restype = c_char_p
handle.setTimeStart.restype = c_bool
handle.setTimeEnd.restype = c_bool
handle.setNumPoints.restype = c_bool
handle.setTimeCourseSelectionList.restype = c_bool
handle.oneStep.restype = c_bool
handle.getTimeStart.restype = c_bool
handle.getTimeEnd.restype = c_bool
handle.getNumPoints.restype = c_bool
handle.reset.restype = c_bool

##\ingroup simulation
#@{

##\brief Sets simulator capabilities
#\param[out] caps An XML string that specifies the simulators capabilities
#\return Returns true if successful
def setCapabilities(caps):
    return handle.setCapabilities(caps)

##\brief Returns simulator capabilities
#\return Returns False if it fails, otherwise returns the simulator's capabilities in the form of an XML string
def getCapabilities():
    return handle.getCapabilities()

##\brief Sets the start time for the simulation
#\param timeStart
#\return Returns True if successful
def setTimeStart(timeStart):
    return handle.setTimeStart (byref (c_double(timeStart)))

##\brief Sets the end time for the simulation
#\param timeEnd
#\return Returns True if successful
def setTimeEnd(timeEnd):
    return handle.setTimeEnd (byref (c_double(timeEnd)))

##\brief Set the number of points to generate in a simulation
#\param numPoints Number of points to generate
#\return Returns True if successful
def setNumPoints(numPoints):
    return handle.setNumPoints(byref (c_int(numPoints)))

##\brief Sets the list of variables returned by simulate() or simulateEx()
#
#Example: rrPython.setTimeCourseSelectionList ("Time, S1, J1, J2")
#
#\param list A string of Ids separated by spaces or comma characters
#\return Returns True if successful
def setTimeCourseSelectionList(list):
    return handle.setTimeCourseSelectionList(list)


##\brief Returns the list of variables returned by simulate() or simulateEx()
#\return A list of symbol IDs indicating the currect selection list
def getTimeCourseSelectionList():
    value = handle.getTimeCourseSelectionList()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief Carry out a time-course simulation, use setTimeStart etc to set
#characteristics
#\return Returns a string containing the results of the simulation organized in rows and columns
def simulate():
    result = handle.simulate()
    #TODO: Check result
    rowCount = handle.getResultNumRows(result)
    colCount = handle.getResultNumCols(result)
    resultArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getResultElement(result, rvalue, cvalue, byref(value)) == True:
                    resultArray[m,n] = value.value
    handle.freeResult(result)
    return resultArray

#use getResultElement and other helper routines to build array that can be used in numpy to plot with matplotlib
#get num cols, get num rows, create array, fill array with two loops


##\brief Carry out a time-course simulation based on the given arguments
#
#Example: m = rrPython.simulateEx(0,25,200)
#
#\return Returns a string containing the results of the simulation organized in rows and columns
def simulateEx(timeStart,timeEnd,numberOfPoints):
    startValue = c_double(timeStart)
    endValue = c_double(timeEnd)
    pointsValue = c_int(numberOfPoints)
    result = handle.simulateEx(byref(startValue),byref(endValue),byref(pointsValue))
    #TODO: Check result
    rowCount = handle.getResultNumRows(result)
    colCount = handle.getResultNumCols(result)
    resultArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getResultElement(result, rvalue, cvalue, byref(value)) == True:
                    resultArray[m,n] = value.value
    handle.freeResult(result)
    return resultArray;

##\brief Carry out a one step integration of the model
#
#Example: status = rrPython.oneStep(currentTime, stepSize)
#
#\param[in] currentTime The current time in the simulation
#\param[in] stepSize The step size to use in the integration
#\param[in] value The new time (currentTime + stepSize)
#Takes (double, double) as an argument
#\return
def oneStep (currentTime, stepSize):                             #test this
    curtime = c_double(currentTime)
    stepValue = c_double(stepSize)
    value = c_double()
    if handle.oneStep(byref(curtime), byref(stepValue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Returns the simulation start time
#
#Example: status = rrPython.getTimeStart()
#
#\return Returns the simulation start time as a float
def getTimeStart():
    value = c_double()
    if handle.getTimeStart(byref(value)) == True:
        return value.value
    else:
        return ('Index out of Range')

##\brief Returns the simulation end time
#
#Example: status = rrPython.getTimeEnd()
#
#\return Returns the simulation end Time as a float
def getTimeEnd():
    value = c_double()
    if handle.getTimeEnd(byref(value)) == True:
        return value.value
    else:
        return ('Index out of Range')

##\brief Returns the value of the current number of points
#
#Example: status = rrPython.getNumPoints()
#
#\return Returns the value of the number of points
def getNumPoints():
    value = c_int()
    if handle.getNumPoints(byref(value)) == True:
        return value.value
    else:
        return ('Index out of Range')

##\brief Reset all floating species concentrations to their intial conditions
#
#Example: status = rrPython.reset()
#
#\return Returns True if successful
def reset():
    return handle.reset()

##@}


#Steady state methods
handle.steadyState.restype = c_bool
handle.setSteadyStateSelectionList.restype = c_bool

##\ingroup steadystate
#@{

##\brief Computes the steady state of the loaded model
#
#Example: status = rrPython.steadyState()
#
#\return Returns a value that is set during the call that indicates how close the solution is to the steady state. The smaller the value, the better.
def steadyState():
    value = c_double()
    if handle.steadyState(byref(value)) == True:
        return value.value
    else:
        return ('Index out of Range')

##\brief A convenient method for returning a vector of the steady state species concentrations
#
#Example: values = rrPython.computeSteadyStateValues()
#
#\return Returns the vector of steady state values or NONE if an error occurred.
def computeSteadyStateValues():
    values = handle.computeSteadyStateValues()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Set the selection list of the steady state analysis
#
#param[in] list The string argument should be a space-separated list of symbols in the selection list
#
#\return Returns True if successful
def setSteadyStateSelectionList(list):
    value = c_char_p(list)
    return handle.setSteadyStateSelectionList(byref(value))

##\brief Get the selection list for the steady state analysis
#\return Returns False if it fails, otherwise it returns a list of strings representing symbols in the selection list
def getSteadyStateSelectionList():
    values = handle.getSteadyStateSelectionList()
    result = handle.listToString(values)
    handle.freeList(values)
    return result

##@}

#State Methods
handle.getValue.restype = c_bool
handle.setValue.restype = c_bool

##\ingroup state
#@{

##\brief Get the value for a given symbol, use getAvailableSymbols() for a list of symbols
#
#Example: status = rrPython.getValue("S1")
#
#\param symbolId The symbol that we wish to obtain the value for
#\return Returns the value if successful, otherwise returns False
def getValue(symbolId):
    value = c_double()
    if handle.getValue(symbolId, byref(value)) == True:
        return value.value
    else:
        raise RuntimeError('Index out of Range')

##\brief Set the value for a given symbol, use getAvailableSymbols() for a list of symbols
#
#Example: status = rrPython.setValue("S1", 0.5)
#
#\param symbolId The symbol that we wish to set the value for
#\param value The value that the symbol will be set to
#\return Returns True if successful
def setValue(symbolId, value):
    value = c_double(value)
    if handle.setValue(symbolId, byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##@}

#Set and get family of methods
handle.setBoundarySpeciesByIndex.restype = c_bool
handle.setFloatingSpeciesByIndex.restype = c_bool
handle.setGlobalParameterByIndex.restype = c_bool
handle.getBoundarySpeciesByIndex.restype = c_bool
handle.getFloatingSpeciesByIndex.restype = c_bool
handle.getGlobalParameterByIndex.restype = c_bool
handle.getCompartmentByIndex.restype = c_bool
handle.setCompartmentByIndex.restype = c_bool

##\ingroup floating
#@{

##\brief Retrieve a string containing concentrations for all the floating species
#
#Example: values = rrPython.getFloatingSpeciesConcentrations()
#
#\return Returns a string of floating species concentrations or None if an error occured
def getFloatingSpeciesConcentrations():
    values = handle.getFloatingSpeciesConcentrations()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Sets the concentration for a floating species by its index. Species are indexed starting at 0.
#
#Example: rrPython.setFloatingSpeciesByIndex(0, .5)
#
#\param index The index to the floating species (corresponds to position in getFloatingSpeciesIds()) starting at 0
#\param value The concentration of the species to set
#\return Returns True if successful
def setFloatingSpeciesByIndex(index, value):
    value = c_double(value)
    ivalue = c_int(index)
    if handle.setFloatingSpeciesByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Returns the concentration of a floating species by its index. Species are indexed starting at 0.
#
#Example: value = rrPython.getFloatingSpeciesByIndex()
#
#\param index The index to the floating species (corresponds to position in getFloatingSpeciesIds()) starting at 0
#\return Returns the concentration of the species if successful
def getFloatingSpeciesByIndex(index):
    value = c_double()
    ivalue = c_int(index)
    if handle.getFloatingSpeciesByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Set the floating species concentration to the vector
#\param vec A vector of floating species concentrations
#\return Returns True if successful
def setFloatingSpeciesConcentrations(vector):
    return handle.setFloatingSpeciesConcentrations(vector)

##@}

##\ingroup boundary
#@{

##\brief Sets the concentration for a Boundary species by its index. Species are indexed starting at 0.
#
#Example: rrPython.setBoundarySpeciesByIndex(0, .5)
#
#\param index The index to the boundary species (corresponds to position in getBoundarySpeciesIds()) starting at 0
#\param value The concentration of the species to set
#\return Returns True if successful
def setBoundarySpeciesByIndex(index, value):
    value = c_double(value)
    ivalue = c_int(index)
    if handle.setBoundarySpeciesByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Returns the concentration of a boundary species by its index. Species are indexed starting at 0.
#
#Example: value = rrPython.getBoundarySpeciesByIndex()
#
#\param index The index to the Boundary species (corresponds to position in getBoundarySpeciesIds()) starting at 0
#\return Returns the concentration of the species if successful
def getBoundarySpeciesByIndex(index):
    value = c_double()
    ivalue = c_int(index)
    if handle.getBoundarySpeciesByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Set the boundary species concentration to the vector
#\param vec A vector of boundary species concentrations
#\return Returns True if successful
def setBoundarySpeciesConcentrations(vector):
    return handle.setBoundarySpeciesConcentrations(vector)

##\brief Returns a string with boundary species concentrations
#\return Returns the concentration of species if successful
def getBoundarySpeciesConcentrations():
    return handle.vectorToString(handle.getBoundarySpeciesConcentrations())

##@}

##\ingroup parameters
#@{

##\brief Retrieve a string containing values for all global parameters
#
#Example: values = rrPython.getGlobalParameterValues()
#
#\return Returns a string of global parameter values or None if an error occured
def getGlobalParameterValues():
    values = handle.getGlobalParameterValues()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Sets the value for a global parameter by its index. Parameters are indexed starting at 0.
#
#Example: rrPython.setGlobalParameterByIndex(0, .5)
#
#\param index The index to the global parameter (corresponds to position in getGlobalParameterIds()) starting at 0
#\param value The value of the global parameter to set
#\return Returns True if successful
def setGlobalParameterByIndex(index, value):
    value = c_double(value)
    ivalue = c_int(index)
    if handle.setGlobalParameterByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Returns the concentration of a global parameter by its index. Parameters are indexed starting at 0.
#
#Example: value = rrPython.getGlobalParameterByIndex()
#
#\param index The index to the global parameter (corresponds to position in getGlobalParameterIds()) starting at 0
#\return Returns the value of the global parameter if successful
def getGlobalParameterByIndex(index):
    value = c_double()
    ivalue = c_int(index)
    if handle.getGlobalParameterByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of Range')

##@}

##/ingroup compartment
#@{

##\brief Returns the volume of a compartment by its index. Compartments are indexed starting at 0.
#
#Example: value = rrPython.getCompartmentByIndex()
#
#\param index The index to the compartment (corresponds to position in getCompartmentIds()) starting at 0
#\return Returns the volume of the compartment if successful
def getCompartmentByIndex(index):
    value = c_double()
    ivalue = c_int(index)
    if handle.getCompartmentByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of Range')

##\brief Sets the value for a compartment by its index. Compartments are indexed starting at 0.
#
#Example: rrPython.setCompartmentByIndex(0, .5)
#
#\param index The index to the compartment (corresponds to position in getCompartmentIds()) starting at 0
#\param value The volume of the compartment to set
#\return Returns True if Successful
def setCompartmentByIndex(index, value):
    value = c_double(value)
    ivalue = c_int(index)
    if handle.setCompartmentByIndex(byref(ivalue), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##@}

#Stoichiometry Methods

##\ingroup stoich
#@{

##\brief Retrieve the full Jacobian for the current model
#\return Returns the full Jacobian matrix
def getFullJacobian():
    matrix = handle.getFullJacobian()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray

##\brief Retreive the reduced Jacobian for the current model
#\return Returns the reduced Jacobian matrix
def getReducedJacobian():
    matrix = handle.getReducedJacobian()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray

##\brief Retreive the eigenvalue matrix for the current model
#\return Returns a matrix of eigenvalues. The first column will contain the real values and te second column will contain the imaginary values.
def getEigenValues():
    matrix = handle.getEigenValues()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray

##\brief Retreive the stoichiometry matrix for the current model
#\return Returns the stoichiometry matrix
def getStoichiometryMatrix():
    matrix = handle.getStoichiometryMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(matrix)
    return matrixArray


##\brief Retreive the Link matrix for the current model
#\return Returns the Link matrix
def getLinkMatrix():
    matrix = handle.getLinkMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray

##\brief Retrieve the reduced stoichiometry matrix for the current model
#\return Returns the reduced stoichiometry matrix
def getNrMatrix():
    matrix = handle.getNrMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray


##\brief Retrieve the L0 matrix for the current model
#\return Returns the L0 matrix
def getL0Matrix():
    matrix = handle.getL0Matrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(result)
    return matrixArray

##\brief Retrieve the conservation matrix for the current model
#\return Returns the conservation matrix
def getConservationMatrix():
    matrix = handle.getConservationMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(matrix)
    return matrixArray

##@}

#Initial condition methods
handle.setFloatingSpeciesInitialConcentrations.restype = c_bool

##\addtogroup initialConditions
#@{

##\brief Set the initial floating species concentrations
#
#Example: status = rrPython.setFloatingSpeciesInitialConcentrations(vec)
#
#\param vec A vector of species concentrations: order given by getFloatingSpeciesIds
#\return Returns True if successful
def setFloatingSpeciesInitialConcentrations(vec):
    return handle.setFloatingSpeciesInitialConcentration(vec)

##\brief Get the initial floating species concentrations
#
#Example: vec = rrPython.getFloatingSpeciesInitialConcentrations()
#
#\return Returns a string containing the intial concentrations
def getFloatingSpeciesInitialConcentrations():
    values = handle.getFloatingSpeciesInitialConcentrations()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Get the initial floating species Ids
#
#Example: vec = rrPython.getFloatingSpeciesInitialConditionIds()
#
#\return Returns a string containing the initial conditions
def getFloatingSpeciesInitialConditionIds():
    values = handle.getFloatingSpeciesInitialConditionIds()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##@}

#Reaction rates
handle.getNumberOfReactions.restype = c_int
handle.getReactionRate.restype = c_bool

##\ingroup reaction
#@{

##\brief Obtain the number of reactions in the loaded model
#
#Example: number = rrPython.getNumberOfReactions()
#
#\return Returns -1 if it fails, returns 0 or more if it is successful (indicating the number of reactions)
def getNumberOfReactions():
    return handle.getNumberOfReactions()

##\brief Returns the reaction rate by index
#\return
def getReactionRate(index):
    ivalue = c_int(index)
    value = c_double()
    if handle.getReactionRate(byref(ivalue), byref(value)) == True:
        return value.value
    else:
        raise RuntimeError('Index out of Range')                                 #test this

##\brief Returns a string containing the current reaction rates
#\return Returns a string containing the current reaction rates
def getReactionRates():
    values = handle.getReactionRates()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Retrieve a string containing the reaction rates given a vector of species concentrations
#
#\param vec The vector of floating species concentrations
#\return Returns a string containing reaction rates
def getReactionRatesEx(vec):                                                        #FIX THIS
    return handle.vectorToString(handle.getReactionRatesEx(vec))

##@}

#Rates of change
handle.getRateOfChange.restype = c_bool
handle.evalModel.restype = c_bool

##\ingroup rateOfChange
#@{

##\brief Returns the rates of change in a string
#
#Example: values = rrPython.getRatesOfChange
#
#\return Returns a string containing rates of change values
def getRatesOfChange():
    values = handle.getRatesOfChange()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##\brief Retrieve the string list of rates of change Ids
#
#Example: Ids = rrPython.getRatesOfChangeIds
#
#\return Returns a list of rates of change Ids
def getRatesOfChangeIds():
    values = handle.getRatesOfChangeIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##\brief Retrieve the rate of change for a given floating species by its index. Species are indexed starting at 0
#
#Example: status = rrPython.getRateOfChange(0)
#
#\return Returns False if it fails, otherwise returns the rate of change.
def getRateOfChange(index):
    ivalue = c_int(index)
    value = c_double()
    if handle.getRateOfChange(byref(ivalue), byref(value)) == True:
        return value.value
    else:
        raise RuntimeError("Index out of range")

##\brief Retrieve the vector of rates of change in a string given a vector of floating species concentrations
#
#Example: values = rrPython.getRatesOfChangeEx(vector)
#
#\return Returns a string containing a vector with the rates of change
def getRatesOfChangeEx(vec):                                          #TEST
    values = handle.getRatesOfChangEx()
    result = handle.vectorToString(values)
    handle.freeVector(values)
    return result

##@}

##\ingroup state
#@{

##\brief Evaluate the current model, which updates all assignments and rates of change
#\return Returns False if it fails
def evalModel():
    return handle.evalModel()

##@}

#Get number family
handle.getNumberOfCompartments.restype = c_int
handle.getNumberOfBoundarySpecies.restype = c_int
handle.getNumberOfFloatingSpecies.restype = c_int
handle.getNumberOfGlobalParameters.restype = c_int
handle.getNumberOfDependentSpecies.restype = c_int
handle.getNumberOfIndependentSpecies.restype = c_int

##\ingroup floating
#@{

##\brief Returns the number of floating species in the model
#\return Returns the number of floating species in the model
def getNumberOfFloatingSpecies():
    return handle.getNumberOfFloatingSpecies()

##\brief Returns the number of dependent species in the model
#\return Returns the number of dependent species in the model
def getNumberOfDependentSpecies():
    return handle.getNumberOfDependentSpecies()

##\brief Returns the number of independent species in the model
#\return Returns the number of independent species in the model
def getNumberOfIndependentSpecies():
    return handle.getNumberOfIndependentSpecies()

##@}

##\ingroup compartment
#@{

##\brief Returns the number of compartments in the model
#\return Returns the number of compartments in the model
def getNumberOfCompartments():
    return handle.getNumberOfCompartments()

##@}

##\ingroup boundary

##\brief Returns the number of boundary species in the model
#\return Returns the number of boundary species in the model
def getNumberOfBoundarySpecies():
    return handle.getNumberOfBoundarySpecies()

##@}

##\ingroup parameters
#@{

##\brief Returns the number of global parameters in the model
#\return Returns the number of global parameters in the model
def getNumberOfGlobalParameters():
    return handle.getNumberOfGlobalParameters()

##@}


#Get Ids family

##\addtogroup reaction
#@{

##\brief Returns a list of reaction Ids
#\return Returns a string containing a list of reaction Ids
def getReactionIds():
    values = handle.getReactionIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##\brief Returns a string containing the list of rate of change Ids
#\return Returns a string containing the list of rate of change Ids
def getRateOfChangeIds():
    values = handle.getRateOfChangeIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##@}

##\ingroup compartment
#@{

##\brief Gets the list of compartment Ids
#\return Returns -1 if it fails, otherwise returns a string containing the list of compartment Ids
def getCompartmentIds():
    values = handle.getCompartmentIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##@}

##\ingroup boundary
#@{

##\brief Gets the list of boundary species Ids
#\return Returns a string containing the list of boundary species Ids
def getBoundarySpeciesIds():
    values = handle.getBoundarySpeciesIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##@}

##\ingroup floating
#@{

##\brief Gets the list of floating species Ids
#\return Returns a string containing the list of floating species Ids
def getFloatingSpeciesIds():
    values = handle.getFloatingSpeciesIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##@}

##\ingroup parameters
#@{

##\brief Gets the list of global parameter Ids
#\return Returns a string containing the list of global parameter Ids
def getGlobalParameterIds():
    values = handle.getGlobalParameterIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result

##@}

##\ingroup state
#@{

##\brief Returns the Ids of all floating species eigenvalues
#\return Returns a string containing the list of all floating species eigenvalues
def getEigenValueIds():
    values = handle.getEigenValueIds()
    result = handle.stringArrayToString(values)
    handle.freeStringArray(values)
    return result


##\brief Returns a string containing the list of all steady state simulation variables
#\return Returns a string containing the list of all steady state simulation variables
def getAvailableSteadyStateSymbols():
    value = handle.getAvailableSteadyStateSymbols()
    result = handle.listToString(value)
    return result

##\brief Returns a string containing the list of all time course simulation variables
#\return Returns a string containing the list of all time course simulation variables
def getAvailableTimeCourseSymbols():
    value = handle.getAvailableTimeCourseSymbols()
    result = handle.listToString(value)
    return result

##@}

#Get MCA methods

##\addtogroup mca
#@{

##\brief Returns the Ids of all elasticity coefficients
#\return Returns a string containing the list of elasticity coefficient Ids
def getElasticityCoefficientIds():
    value = handle.getElasticityCoefficientIds()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief Returns the Ids of all unscaled flux control coefficients
#\return Returns a string containing the list of all unscaled flux control coefficient Ids
def getUnscaledFluxControlCoefficientIds():
    value = handle.getUnscaledFluxControlCoefficientIds()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief Returns the Ids of all flux control coefficients
#\return Returns a string containing the list of all flux control coefficient Ids
def getFluxControlCoefficientIds():
    value = handle.getFluxControlCoefficientIds()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief Returns the Ids of all unscaled concentration control coefficients
#\return Returns a string containing the list of all unscaled concentration coefficient Ids
def getUnscaledConcentrationControlCoefficientIds():
    value = handle.getUnscaledConcentrationCoefficientIds()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief Returns the Ids of all concentration control coefficients
#\return Returns a string containing the list of all concentration control coefficient Ids
def getConcentrationControlCoefficientIds():
    value = handle.getConcentrationControlCoefficientIds()
    result = handle.stringArrayToString(value)
    handle.freeStringArray(value)
    return result

##\brief  Retrieve the unscaled elasticity matrix for the current model
#\return Returns a string containing the matrix of unscaled elasticities. The first column will contain the
#real values and the second column the imaginary values.
def getUnScaledElasticityMatrix():
    matrix = handle.getUnScaledElasticityMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(matrix)
    return matrixArray

##\brief Retrieve the scaled elasticity matrix for the current model
#\return Returns a string containing the matrix of scaled elasticities. The first column will contain
#real values and the second column the imaginary values.
def getScaledElasticityMatrix():
    matrix = handle.getScaledElasticityMatrix()
    if matrix == 0:
       return 0
    rowCount = handle.getMatrixNumRows(matrix)
    colCount = handle.getMatrixNumCols(matrix)
    result = handle.matrixToString(matrix)
    matrixArray = zeros((rowCount,colCount))
    for m in range(rowCount):
        for n in range(colCount):
                value = c_double()
                rvalue = m
                cvalue = n
                if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
                    matrixArray[m,n] = value.value
    handle.freeMatrix(matrix)
    return matrixArray

##\brief Retrieve the unscaled concentration control coefficient matrix for the current model
#\return Returns a string containing the matrix of unscaled concentration control coefficients. The first column will contain
#real values and the second column the imaginary values.
def getUnscaledConcentrationControlCoefficientMatrix():
    value = handle.getUnscaledConcentrationControlCoefficientMatrix()
    result = handle.matrixToString(value)
    handle.freeMatrix(value)
    return result

##\brief Retrieve the scaled concentration control coefficient matrix for the current model
#\return Returns a string containing the matrix of scaled concentration control coefficients. The first column will contain
#real values and the second column the imaginary values.
def getScaledConcentrationControlCoefficientMatrix():
    value = handle.getScaledConcentrationControlCoefficientMatrix()
    result = handle.matrixToString(value)
    handle.freeMatrix(value)
    return result

##\brief Retrieve the unscaled flux control coefficient matrix for the current model
#\return Returns a string containing the matrix of unscaled flux control coefficients. The first column will contain
#real values and the second column the imaginary values.
def getUnscaledFluxControlCoefficientMatrix():
    value = handle.getUnscaledFluxControlCoefficientMatrix()
    result = handle.matrixToString(value)
    handle.freeMatrix(value)
    return result

##\brief Retrieve the scaled flux control coefficient matrix for the current model
#\return Returns a string containing the matrix of scaled flux control coefficients. The first column will contain
#real values and the second column the imaginary values.
def getScaledFluxControlCoefficientMatrix():
    value = handle.getScaledFluxControlCoefficientMatrix()
    result = handle.matrixToString(value)
    handle.freeMatrix(value)
    return result

#more MCA
handle.getuCC.restype = c_bool
handle.getCC.restype = c_bool
handle.getEE.restype = c_bool
handle.getuEE.restype = c_bool
handle.getScaledFloatingSpeciesElasticity.restype = c_bool

##\brief Get unscaled control coefficient with respect to a global parameter
#
#Takes (variableName, parameterName) as an argument, where both arguments are strings
#\return
def getuCC(variable, parameter):
    variable = c_char_p()
    parameter = c_char_p()
    value = c_double()
    if handle.getuCC(byref(variable), byref(parameter), byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Retireve a single control coefficient
#
#\param[in] variable This is the dependent variable of the coefficient, for example a flux or species concentration
#\param[in] parameter This is the independent parameter, for example a kinetic constant or boundary species
#\param[out] value This is the value of the control coefficeint returned to the caller
#
#\return Returns a single control coefficient if successful
def getCC(variable, parameter):
    value = c_double()
    if handle.getCC(variable, parameter, byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Retireve a single elasticity coefficient
#
#\param[in] variable This is the dependent variable of the coefficient, for example a flux or species concentration
#\param[in] parameter This is the independent parameter, for example a kinetic constant or boundary species
#\param[out] value This is the value of the control coefficeint returned to the caller
#\return Returns a single elasticity coefficient if successful
def getEE(variable, parameter):
    value = c_double()
    if handle.getEE(variable, parameter,  byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Retrieve a single unscaled elasticity coefficient
#
#\param[in] name This is the reaction variable for the unscaled elasticity
#\param[in] species This is the independent parameter, for example a floating of boundary species
#\param[out] value This is the value of the unscaled elasticity coefficient returned to the caller
#Takes (reactionName, parameterName) as an argument, where both arguments are strings
#\return
def getuEE(name, species):
    value = c_double()
    if handle.getuEE(name, species, byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Compute the scaled elasticity for a given reaction and given species
#
#Takes (reactionName, parameterName) as an argument, where both arguments are strings
#\return
def getScaledFloatingSpeciesElasticity(reactionName, speciesName):
    value = c_double()
    if handle.getScaledFloatingSpeciesElasticity(reactionName, speciesName,  byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##@}

#NOM lib forwarded functions
handle.getNumberOfRules.restype = c_int

##\ingroup NOM functions
#@{

##\brief Returns the number of rules in the current model
#\return Returns an integer larger or equal to 0 if succesful, or -1 on failure
def getNumberOfRules():
    return handle.getNumberOfRules()

##@}

#Print/format functions
handle.resultToString.restype = c_char_p
handle.matrixToString.restype = c_char_p
handle.vectorToString.restype = c_char_p
handle.stringArrayToString.restype = c_char_p
#handle.printStringArrayList.restype = c_char_p
#handle.printArrayList.restype = c_char_p
handle.listToString.restype = c_char_p

##\ingroup toString
#@{

##\brief Returns a result struct in string form.
#\return Returns a result struct as a string
def resultToString(result):
    return handle.resultToString(result)

##\brief Returns a matrix in string form.
#\return Returns a matrix as a string
def matrixToString(matrix):
    return handle.matrixToString(matrix)

##\brief Returns a vector in string form.
#\return Returns a vector as a string
def vectorToString(vector):
    return handle.vectorToString(vector)

##\brief Returns a string list in string form.
#\return Returns a string list as a string
def stringArrayToString(list):
    return handle.stringArrayToString(list)

#def printStringArrayList(list):
#    return handle.printStringArrayList(list)

#def printArrayList(list):
#    return handle.printArrayList(list)

##\brief Returns a list in string form
#\return Returns a string array as a string
def listToString(list):
    return handle.listToString(list)

##@}

#Free memory functions

handle.Pause.restype = None

##\ingroup freeRoutines
#@{


##\brief Free the result structure handle returned by simulate() and simulateEx()
def freeResult(handle):
    return handle.freeresult(handle)

##\brief Free char* generated by the C library routines
def freeText(text):
    return handle.freeText(text)

##\brief Free RRStringArrayHandle structures
def freeStringArray(sl):
    return handle.freeStringArray(sl)

##\brief Free RRVectorHandle structures
def freeVector(vector):
    return handle.freeVector(vector)

##\brief Free RRMatrixHandle structures
def freeMatrix(matrix):
    return handle.freeMatrix(matrix)

##\brief Free RRCCodeHandle structures
def freeCCode(code):
    return handle.freeCCode(code)


##@}
##\brief Pause
#\return void
def Pause():
    return handle.Pause()

#Helper routines

handle.getVectorLength.restype = c_int
handle.getVectorElement.restype = c_bool
handle.setVectorElement.restype = c_bool
#handle.getStringListLength.restype = c_int
#handle.getStringListElement.restype = c_char_p
handle.getMatrixNumRows.restype = c_int
handle.getMatrixNumCols.restype = c_int
handle.getMatrixElement.restype = c_bool
handle.getResultNumRows.restype = c_int
handle.getResultNumCols.restype = c_int
handle.getResultElement.restype = c_bool
handle.getResultColumnLabel.restype = c_char_p
handle.getCCodeHeader.restype = c_char_p
handle.getCCodeSource.restype = c_char_p

##\ingroup helperRoutines
#@{

##\brief Get the number of elements in a vector type
#
#Example: count = rrPython.getVectorLength(myVector)
#
#\param vector A vector handle
#\return Returns -1 if it fails, otherwise returns the number of elements in the vector
def getVectorLength(vector):
    return handle.getVectorLength(vector)
##\brief
#
#Example: myVector = rrPython.createVector(10)
#
#\param size The number of elements in the new vector
#\return Returns NONE if it fails, otherwise returns a vector handle
def createVector(size):
#    value = c_int(size)
    return handle.createVector(size)


##\brief Get a particular element from a vector
#
#Example: status = rrPython.getVectorElement (myVector, 10, &value);
#
#\param vector A pointer to the vector variable type
#\param index An integer indicating the ith element to retrieve (indexing is from zero)
#\param value A pointer to the retrieved double value
#\return Returns the vector element if successful
def getVectorElement(vector, index):
    ivalue = c_int(index)
    value = c_double()
    if handle.getVectorElement(vector, byref(ivalue), byref(value)) == True:
        return value.value
    else:
        raise RuntimeError('Index out of Range')

##\brief Get a particular element from a vector
#
#Example: status = rrPython.setVectorElement (myVector, 10, 3.1415);
#
#\param vector A vector handle
#\param index An integer indicating the ith element to set (indexing is from zero)
#\param value The value to store in the vector at the given index position
#\return Returns true if succesful
def setVectorElement(vector, index, value):
    value = c_double(value)
    ivalue = c_int(index)
    if handle.setVectorElement(vector, byref(ivalue),  value) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

#def getStringListLength(stringList):
#    return handle.getStringListLength(stringList)

#def getStringListElement(stringList, index):
#    value = c_int()
#    if handle.getStringListElement(stringList, index, byref(value)) == True:
#        return value.value
#    else:
#        raise RuntimeError("Index out of range")


##\brief Retrieve the number of rows in the given matrix
#
#Example: nRows = rrPython.getMatrixNumRows(matrix)
#
#\param matrix A matrix handle
#\return Returns -1 if fails, otherwise returns the number of rows
def getMatrixNumRows(matrix):
    return handle.getMatrixNumRows(matrix)

##\brief Retrieve the number of columns in the given matrix
#
#Example: nRows = rrPython.getMatrixNumCols(matrix)
#
#\param matrix A matrix handle
#\return Returns -1 if fails, otherwise returns the number of columns
def getMatrixNumCols(matrix):
    return handle.getMatrixNumCols(matrix)

##\brief Retrieves an element at a given row and column from a matrix type variable
#
#Example: status = rrPython.getMatrixElement (matrix, 2, 4)
#
#\param matrix A matrix handle
#\param row The row index to the matrix
#\param column The column index to the matrix
#\return Returns the value of the element if successful
def getMatrixElement(matrix, row, column):
    value = c_double()
    rvalue = c_int(row)
    cvalue = c_int(column)
    if handle.getMatrixElement(matrix, rvalue, cvalue, byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Retrieve the number of rows in the given result data
#
#Example: nRows = rrPython.getResultNumRows(result)
#
#\param result A result handle
#\return Returns -1 if fails, otherwise returns the number of rows
def getResultNumRows(result):
    return handle.getResultNumRows(result)

##\brief Retrieve the number of columns in the given result data
#
#Example: nRows = rrPython.getResultNumCols(result);
#
#\param result A result handle
#\return Returns -1 if fails, otherwise returns the number of columns
def getResultNumCols(result):
    return handle.getResultNumCols(result)

##\brief Retrieves an element at a given row and column from a result type variable
#
#Example: status = rrPython.getResultElement(result, 2, 4);
#
#\param result A result handle
#\param row The row index to the result data
#\param column The column index to the result data
#\return Returns true if succesful
def getResultElement(result, row, column):
    value = c_double()
    rvalue = c_int(row)
    cvalue = c_int(column)
    if handle.getMatrixElement(result, rvalue, cvalue, byref(value)) == True:
        return value.value;
    else:
        raise RuntimeError('Index out of range')

##\brief Retrieves an element at a given row and column from a result type variable
#
#Example: str = getResultColumnLabel (result, 2, 4);
#
#\param result A result handle
#\param column The column index for the result data (indexing from zero)
#\return Returns NONE if fails, otherwise returns the string column label
def getResultColumnLabel(result, column):
    cvalue = c_int(column)
    return handle.getResultColumnLabel(result, cvalue)


##\brief Retrieve the header file code for the current model (if applicable)
#
#
#Example:   CCode = rrPython.getCCode()
#           header = rrPython.getCCodeHeader(CCode)
#
#\param code A handle for a string that stores the C code
#\return Returns the header for the C code handle used as an argument
def getCCodeHeader(code):
    return handle.getCCodeHeader(code)

##\brief Retrieve the source file code for the current model (if applicable)
#
#
#Example:   CCode = rrPython.getCCode()
#           header = rrPython.getCCodeSource(CCode)
#
#\param code A handle for a string that stores the C code
#\return Returns the source for the C code handle used as an argument
def getCCodeSource():
    return handle.getCCodeSource()

##@}

#=======================================================#
