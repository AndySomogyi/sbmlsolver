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

    If options is null, then the current simulation settings (start time,
    end time, n steps) are used. If options is not null, then the
    current simulation settings are set to the values specified by
    options and they are used.

    *Arguments*
        options (SimulateOptions)
            An optional SimulateOptions class where additional simulation options
            may be specified

    *Returns*
        (2D numpy array)
            a numpy array with each selected output timeseries being a
            column vector, and the 0'th column is the simulation time.";



%feature("docstring") rr::ExecutableModel "
The sbml model.
";

%feature("docstring") rr::ExecutableModel::getModelName "
Get the model name specified in the sbml
";
%feature("docstring") rr::ExecutableModel::setTime "
Set the time.
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

*Arguments*
    index (numpy int32 array) optional
        an array of desired floating species indices. i.e. if this model has
        4 floating species and we want the amounts for the last and first, we
        would use [3,0] for the index array.
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







