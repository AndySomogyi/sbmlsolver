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

    *Arguments*
        compiler (string)
            if LLVM build is enabled, the compiler defaults to LLVM.
        tempDir (string)
            typically ignored, only used by the old C RoadRunner.
        supportCodeDir (string)
            typically ignored, only used by the old C RoadRunner";

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
";

%feature("docstring") rr::ExecutableModel::getFloatingSpeciesConcentrations "
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesAmounts "
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesConcentrations "
";

%feature("docstring") rr::ExecutableModel::getGlobalParameterValues "
";

%feature("docstring") rr::ExecutableModel::getCompartmentVolumes "
";

%feature("docstring") rr::ExecutableModel::getConservedSums "
";

%feature("docstring") rr::ExecutableModel::getReactionRates "
";

%feature("docstring") rr::ExecutableModel::getBoundarySpeciesIds "
";

%feature("docstring") rr::ExecutableModel::getCompartmentIds "
";

%feature("docstring") rr::ExecutableModel::getReactionIds "
";

%feature("docstring") rr::ExecutableModel::setFloatingSpeciesAmounts "
";

%feature("docstring") rr::ExecutableModel::setFloatingSpeciesConcentrations "
";

%feature("docstring") rr::ExecutableModel::setBoundarySpeciesConcentrations "
";

%feature("docstring") rr::ExecutableModel::setGlobalParameterValues "
";

%feature("docstring") rr::ExecutableModel::setCompartmentVolumes "
";

%feature("docstring") rr::ExecutableModel::setConservedSums "
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


    *Arguments*
        sbmlSettingFilePath (optional)
            if this is given, the settings are read from this settings file,
            if not, the default values are set.
";


%feature("docstring") rr::SimulateOptions::ResetModel "
reset the model to the initial state.
";

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
The duration of the simulation run, in the model's units of time.";

%feature("docstring") rr::SimulateOptions::absolute "
A number representing the absolute difference permitted.";

%feature("docstring") rr::SimulateOptions::relative "
A float-point number representing the relative difference permitted.
Defaults 0.0001";

%feature("docstring") rr::SimulateOptions::variables "
The variables (in addition to time) whose values will be saved in the result.
These are SBML model id's. Order is significant, as this determines the order
of the columns in the result matrix.

Important: if a symbol in this list refers to a species in the model,
then that symbol will also be listed in either the amount or concentration
lists below.

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





