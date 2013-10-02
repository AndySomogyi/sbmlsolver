/*
 * docstrings.i
 *
 *  Created on: Sep 23, 2013
 *      Author: andy
 */



%feature("docstring") rr::RoadRunner "
The main RoadRunner class.

All three of the RoadRunner options default to the empty string, in this
case, the default values are used.

 
:param compiler: if LLVM build is enabled, the compiler defaults to LLVM.
:type name: str (optional)

:param tempDir: (string) typically ignored, only used by the old C RoadRunner.
:type name: str (optional)

:param supportCodeDir: typically ignored, only used by the old C RoadRunner
:type name: str (optional)";


%feature("docstring") rr::RoadRunner::simulate "
simulate the current SBML model.

There are a number of ways to call simulate. 

1. With no arguments. In this case, the current set of `SimulateOptions` will
be used for the simulation. The current set may be changed either directly 
via setSimulateOptions() or with one of the two alternate ways of calling 
simulate. 

2: With single `SimulateOptions` argument. In this case, all of the settings 
in the given options are copied and will be used for the current and future
simulations. 

3: With the three positions arguments, `timeStart`, `timeEnd`, `steps`. In this case
these three values are copied and will be used for the current and future simulations.

The options given in the 2nd and 3rd forms will remain in effect until changed. So, if
one calls::

    r.simulate(0, 3.14, 100)

The start time of 0, end time of 3.14 and steps of 100 will remain in effect, so that if this
is followed by a call to::

    r.simulate()

This simulation will use the previous values. 

:returns: a numpy array with each selected output timeseries being a
          column vector, and the 0'th column is the simulation time.
:rtype: numpy.ndarray";


%feature("docstring") rr::RoadRunner::computeSteadyStateValue "

";

%feature("docstring") rr::RoadRunner::getCC "
getCC(variable, parameter)
Get scaled a single control coefficient with respect to a global parameter.

:param variable: The id of a dependent variable of the coefficient, for example a 
flux or species concentration.
:type name: str
:param parameter: The id of the independent parameter, for example a kinetic constant 
or boundary species
:returns: the value of the control coefficeint returned to the caller.
:rtype: double
";
                                             
%feature("docstring") rr::RoadRunner::getCompiler "
get the compiler used to build the ExecutableModel.
";
                                       
%feature("docstring") rr::RoadRunner::getConfigurationXML "
recurse through all of the child configurable objects that this
class ownes and build an assemble all of thier configuration parameters
into a single xml document which is returned as a string.

The value of this result depends on what child objects are presently loaded.
";
                               
%feature("docstring") rr::RoadRunner::getConservationAnalysis "
is conservation analysis enabled. This is set
";
                           
%feature("docstring") rr::RoadRunner::getConservationMatrix "
TODO docs
";
                             
%feature("docstring") rr::RoadRunner::getCopyright "
get the copyright string
";
                                      
%feature("docstring") rr::RoadRunner::getEE "
Retireve a single elasticity coefficient.

:param str variable: The dependent variable of the coefficient, for example a flux or 
	species concentration.
:param str parameter: The independent parameter, for example a kinetic constant or boundary species

";
                                             
%feature("docstring") rr::RoadRunner::getEigenvalueIds "
returns a list of floating species ids with thier names
prefixed with 'eigen_'. For example, if the model contained
the floating species 'S1' and 'S2', this would return a list
containing ['eigen_S1', 'eigen_S2'].
";
                                  
%feature("docstring") rr::RoadRunner::getEigenvalues "
Returns eigenvalues, first column real part, second column imaginary part

:rtype: numpy.ndarray
";
                                    
%feature("docstring") rr::RoadRunner::getExtendedVersionInfo "
getVersion plus info about dependent libs versions.
";
                            
%feature("docstring") rr::RoadRunner::getFloatingSpeciesAmountIds "
gets a list of the floating species ids, but with the ids surrounded
by square brackets, i.e. 'S1' -> '[S1]'
";
                       
%feature("docstring") rr::RoadRunner::getFullJacobian "
compute the full Jacobian at the current operating point. 

This is the Jacobian of ONLY the floating species.
";
                                   
%feature("docstring") rr::RoadRunner::getFullyReorderedStoichiometryMatrix "
Returns the stoichiometry matrix for the currently loaded model
";
              
%feature("docstring") rr::RoadRunner::getInfo "
return info about the current state of the object

:rtype: str
";
                                           
%feature("docstring") rr::RoadRunner::getInstanceCount "
Number of currently running RoadRunner instances.
";
                                  
%feature("docstring") rr::RoadRunner::getInstanceID "
When there are multiple instances of RoadRunner, this is the instance id.
";
                                     
%feature("docstring") rr::RoadRunner::getIntegrator "
get the integrator which is currently being used to
time evolve the system.
";
                                     
%feature("docstring") rr::RoadRunner::getL0Matrix "
TODO docs

:rtype: numpy.ndarray
";
                                       
%feature("docstring") rr::RoadRunner::getLinkMatrix "
TODO docs

:rtype: numpy.ndarray
";
                                     
%feature("docstring") rr::RoadRunner::getModelGenerator "
TODO docs

:rtype: numpy.ndarray
";
                                 
%feature("docstring") rr::RoadRunner::getNrMatrix "
TODO docs

:rtype: numpy.ndarray
";
                                       
%feature("docstring") rr::RoadRunner::getParamPromotedSBML "
Takes an sbml document (in textual form) and changes all of the local parameters
to be global parameters.

:param str sbml: the contents of an sbml document
:rtype: str
";
                              
%feature("docstring") rr::RoadRunner::getReducedJacobian "
get the *reduced* Jacobian for the independent species. 

:rtype: numpy.ndarray
";
                                
%feature("docstring") rr::RoadRunner::getReorderedStoichiometryMatrix "
TODO docs

:rtype: numpy.ndarray
";
                   
%feature("docstring") rr::RoadRunner::getSBML "
return the currently loaded sbml document as a string.

:rtype: str
";
                                           
%feature("docstring") rr::RoadRunner::getScaledConcentrationControlCoefficientMatrix "
TODO docs

:rtype: numpy.ndarray
";
    
%feature("docstring") rr::RoadRunner::getScaledFloatingSpeciesElasticity "
Compute the scaled elasticity for a given reaction and given species. 

:param str reactionId: the sbml id of a reaction.
:param str speciesId: the sbml id of a species.
:rtype: double
";
                
%feature("docstring") rr::RoadRunner::getScaledFluxControlCoefficientMatrix "
Calculate the the matrix of scaled flux control coefficients. The first column 
will contain real values and the second column the imaginary values.

:rtype: numpy.ndarray
";
             
%feature("docstring") rr::RoadRunner::getScaledReorderedElasticityMatrix "
Compute the unscaled elasticity matrix at the current operating point

:rtype: numpy.ndarray
";
                              
%feature("docstring") rr::RoadRunner::getSelectedValues "
returns the values selected with SimulateOptions for the current model time / timestep

:rtype: numpy.ndarray
";
                                               
%feature("docstring") rr::RoadRunner::getSimulationResult "
get the simulation result in case one forgot to hold on to the simulate return value.

:rtype: numpy.ndarray
";
                                             
%feature("docstring") rr::RoadRunner::getSteadyStateSelectionList "

";
                                     
%feature("docstring") rr::RoadRunner::getStoichiometryMatrix "
TODO docs
";
                                          
%feature("docstring") rr::RoadRunner::getTimeCourseSelectionList "
TODO docs
";
                                      
%feature("docstring") rr::RoadRunner::getUnscaledConcentrationControlCoefficientMatrix "
TODO docs
";
                
%feature("docstring") rr::RoadRunner::getUnscaledElasticityMatrix "
TODO docs
";
                                     
%feature("docstring") rr::RoadRunner::getUnscaledFluxControlCoefficientMatrix "
TODO docs
";
                         
%feature("docstring") rr::RoadRunner::getUnscaledSpeciesElasticity "
Get a single species elasticity value
IMPORTANT:
Assumes that the reaction rates have been precomputed at the operating point !!

:param int reactionIndx: index of reactionl
:param int speciesIndx: index of species.
";
                                    
%feature("docstring") rr::RoadRunner::getValue "
TODO docs
";
                                                        
%feature("docstring") rr::RoadRunner::getValueForRecord "
TODO docs
";
                                               
%feature("docstring") rr::RoadRunner::getVersion "
TODO docs
";
                                                      
%feature("docstring") rr::RoadRunner::getlibSBMLVersion "
TODO docs
";
                                               
%feature("docstring") rr::RoadRunner::isModelLoaded "
TODO docs
";
                                                   
%feature("docstring") rr::RoadRunner::loadSBML "
TODO docs
";
                                                        
%feature("docstring") rr::RoadRunner::loadSBMLFromFile "
Load an sbml document from disk.

:param str path: path of an sbml document.
:returns: True if successfull, False if failed.
";
                                                
%feature("docstring") rr::RoadRunner::model "
TODO docs
";
                                                           
%feature("docstring") rr::RoadRunner::oneStep "
TODO docs
";
                                                         
%feature("docstring") rr::RoadRunner::reset "
TODO docs
";
                                                           
%feature("docstring") rr::RoadRunner::setConfigurationXML "
given a xml document, which should have been returned from getConfigurationXML,
this method recurses though all the child configurable elements and sets thier
configuration to the values specified in the document.

:param str xml: the contents of an xml document.
";
                                             
%feature("docstring") rr::RoadRunner::setConservationAnalysis "
TODO docs
";
                                         
%feature("docstring") rr::RoadRunner::setSteadyStateSelectionList "

:param list(str) selectionList: a list of selection identifiers.
";
                                     
%feature("docstring") rr::RoadRunner::setTimeCourseSelectionList "

:param list(str) selectionList: a list of selection identifiers.
";
                                                                                              
%feature("docstring") rr::RoadRunner::simulateOptions "
Get the SimulateOptions object where simulation options may be set. 
";
                                                 
%feature("docstring") rr::RoadRunner::steadyState "
TODO docs
";
                                                     
%feature("docstring") rr::RoadRunner::this "
TODO docs
";
                                                            

/*************************************************************/
/***** ExecutableModel ***************************************/
/*************************************************************/

%feature("docstring") rr::ExecutableModel "
The sbml model.
";

%feature("docstring") rr::ExecutableModel::getModelName "
Get the model name specified in the sbml
";

%feature("docstring") rr::ExecutableModel::setTime "
Set the model time variable.

:param time: time the time value to set.
:returns: None
";
%feature("docstring") rr::ExecutableModel::getTime "
";
%feature("docstring") rr::ExecutableModel::getConservedSumChanged "
";
%feature("docstring") rr::ExecutableModel::setConservedSumChanged "
";
%feature("docstring") rr::ExecutableModel::evalInitialConditions "
";
%feature("docstring") rr::ExecutableModel::reset "
";
%feature("docstring") rr::ExecutableModel::getNumIndependentSpecies "
";
%feature("docstring") rr::ExecutableModel::getNumDependentSpecies "
";
%feature("docstring") rr::ExecutableModel::getNumFloatingSpecies "
";
%feature("docstring") rr::ExecutableModel::getNumBoundarySpecies "
";
%feature("docstring") rr::ExecutableModel::getNumGlobalParameters "
";
%feature("docstring") rr::ExecutableModel::getNumCompartments "
";
%feature("docstring") rr::ExecutableModel::getNumConservedSums "
";
%feature("docstring") rr::ExecutableModel::getNumRules "
";
%feature("docstring") rr::ExecutableModel::getNumReactions "
";
%feature("docstring") rr::ExecutableModel::getInfo "
";

%feature("docstring") rr::ExecutableModel::getFloatingSpeciesAmounts "
Get the list of floating species amounts. If no arguments are given, this
returns all floating species amounts.

:param index: an optional array of desired floating species indices. i.e. if this model has
        4 floating species and we want the amounts for the last and first, we
        would use [3,0] for the index array.
:type name: numpy.ndarray

get all the amounts:

>>> e.getFloatingSpeciesAmounts()
[15,2,3,20]

get amounts 3 and 0:

>>> getFloatingSpeciesAmounts([3,0])
[10,15]

";

%feature("docstring") rr::ExecutableModel::getFloatingSpeciesAmountRates "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of the rates of change of the floating species amounts.
:rtype: numpy.ndarray
";


%feature("docstring") rr::ExecutableModel::getFloatingSpeciesConcentrations "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of floating species concentrations.
:rtype: numpy.ndarray
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesAmounts "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of the boundary species amounts.
:rtype: numpy.ndarray.
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesConcentrations "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of the boundary species concentrations. 
:rtype: numpy.ndarray.
";

%feature("docstring") rr::ExecutableModel::getGlobalParameterValues "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of global parameter values.
:rtype: numpy.ndarray.
";

%feature("docstring") rr::ExecutableModel::getCompartmentVolumes "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of compartment volumes.
:rtype: numpy.ndarray.
";

%feature("docstring") rr::ExecutableModel::getConservedSums "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of conserved sums.
:rtype: numpy.ndarray.
";

%feature("docstring") rr::ExecutableModel::getReactionRates "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: an array of reaction rates.
:rtype: numpy.ndarray
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesIds "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: a list of boundary species ids.
";

%feature("docstring") rr::ExecutableModel::getCompartmentIds "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:retuns: a list of compartment ids.
";

%feature("docstring") rr::ExecutableModel::getReactionIds "

:param numpy.ndarray index: (optional) an index array indicating which items to return.
:returns: a list of reaction ids.
";

%feature("docstring") rr::ExecutableModel::setFloatingSpeciesAmounts "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.
";

%feature("docstring") rr::ExecutableModel::setFloatingSpeciesConcentrations "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.
";

%feature("docstring") rr::ExecutableModel::setBoundarySpeciesConcentrations "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.

";

%feature("docstring") rr::ExecutableModel::setGlobalParameterValues "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.
";

%feature("docstring") rr::ExecutableModel::setCompartmentVolumes "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.

";

%feature("docstring") rr::ExecutableModel::setConservedSums "

:param numpy.ndarray index: (optional) an index array indicating which items to set, 
	or if no index array is given, the first param should be an array of all the 
	values to set.
:param numpy.ndarray values' the values to set.

";


/*************************************************************/
/***** SelectionRecord ***************************************/
/*************************************************************/

%feature("docstring") rr::SelectionRecord "
RoadRunner provides a range of flexible ways of selecting values from 
a simulation. These values can not only be calculated directly via
RoadRunner.getSelectionValue, but any of these selections can be
used as columns in the simulate result matrix. 

The SectionRecord.selectionType should be one of the constants listed
here. 

Most selection types only require the first symbol id, p1 to be set, 
however certain ones such as [???] require both p1 and p2.
";

%feature("docstring") rr::SelectionRecord::BOUNDARY_AMOUNT "
A boundary species amount.
";
%feature("docstring") rr::SelectionRecord::BOUNDARY_CONCENTRATION "
A boundary species concentration.
";
%feature("docstring") rr::SelectionRecord::EIGEN_VALUE "
";
%feature("docstring") rr::SelectionRecord::ELASTICITY "
";
%feature("docstring") rr::SelectionRecord::FLOATING_AMOUNT "
";
%feature("docstring") rr::SelectionRecord::FLOATING_CONCENTRATION "
";
%feature("docstring") rr::SelectionRecord::FLUX "
";
%feature("docstring") rr::SelectionRecord::PARAMETER "
";
%feature("docstring") rr::SelectionRecord::RATE_OF_CHANGE "
";
%feature("docstring") rr::SelectionRecord::SCALED_ELASTICITY "
";
%feature("docstring") rr::SelectionRecord::STOICHIOMETRY "
";
%feature("docstring") rr::SelectionRecord::TIME "
";
%feature("docstring") rr::SelectionRecord::UNKNOWN "
";
%feature("docstring") rr::SelectionRecord::VOLUME "
";
%feature("docstring") rr::SelectionRecord::index "
";
%feature("docstring") rr::SelectionRecord::p1 "
";
%feature("docstring") rr::SelectionRecord::p2 "
";
%feature("docstring") rr::SelectionRecord::selectionType "
";


/** SimulateOptions **/


%feature("docstring") rr::SimulateOptions "

RoadRunner simulation options.

This is the full set of options that determines how RoadRunner performs
a simulation of an sbml model.

This is a superset of the values stored in a sbml test suite settings file, the
documentation of the fields which correspond to an sbml test suite settings was
taken from http://sbml.org

This object can be read from a sbml test suite options file by using a file
name in the constructor.

:param sbmlSettingFilePath: (optional) if this is given, the settings are read 
from this settings file, if not, the default values are set.
";

%feature("docstring") rr::SimulateOptions::RESET_MODEL "
bitfield option for flags which reset the model to the initial state.
Can be more convenient simply setting the .resetModel attribute
";

%feature("docstring") rr::SimulateOptions::STIFF "
bitfield options for integratorFlags which enables / disables the stiff
integrator, can be set directly with the .stiff attribute";

%feature("docstring") rr::SimulateOptions::resetModel "
Causes the model to be reset to the original conditions specified
in the sbml when the simulation is run.";

%feature("docstring") rr::SimulateOptions::stiff "
Use the stiff integrator.";

%feature("docstring") rr::SimulateOptions::flags "
can be set to ResetModel so that the model is reset to its intial state
when the simulation is run. ";

%feature("docstring") rr::SimulateOptions::steps "
The number of steps at which the output is sampled. The samples are evenly spaced.
When a simulation system calculates the data points to record, it will typically
divide the duration by the number of time steps. Thus, for X steps, the output
will have X+1 data rows.";

%feature("docstring") rr::SimulateOptions::start "
The start time of the simulation time-series data.
Often this is 0, but not necessarily.";

%feature("docstring") rr::SimulateOptions::duration "
The duration of the simulation run, in the model's units of time. Note, 
setting the duration automatically sets the end time and visa versa.";

%feature("docstring") rr::SimulateOptions::end "
The simulation end time. Note, setting the end time automatically sets the 
duration accoringly and visa versa.";

%feature("docstring") rr::SimulateOptions::absolute "
:type name: list(str)

A number representing the absolute difference permitted for the integrator
tolerence.";

%feature("docstring") rr::SimulateOptions::relative "
A float-point number representing the relative difference permitted.
Defaults 0.0001";

%feature("docstring") rr::SimulateOptions::variables "
The variables (in addition to time) whose values will be saved in the result.
These are SBML model id's. Order is significant, as this determines the order
of the columns in the result matrix.

Important: if a symbol in this list refers to a species in the model,
then that symbol should also be listed in either the amount or concentration
lists below. If a species symbol is listed in variables, but is not listed
in either amounts or concentrations, then it defaults to an amount value. 

The ordering of the symbols in variabls is what determines the output 
ordering. The order of symbols in either amounts or concetrations do not
effect the output ordering.

NOTE:If a listed variable has two underscores in it ('__'), that variable
is actually present only in a submodel of the main model, from the
Hierarchical Model Composition package, in the format submodelID__variableID.
If the model is flattened, the variable will appear automatically.";

%feature("docstring") rr::SimulateOptions::amounts "
A list of the variable whose output in the results file is in amount
(not concentration) units. This list of variables must be a subset of
the names listed in variables.";


%feature("docstring") rr::SimulateOptions::concentrations "
A list of the variable whose output in the results file is in concentration
(not amount) units. This list of variables must be a subset of the names
listed in variables.";





