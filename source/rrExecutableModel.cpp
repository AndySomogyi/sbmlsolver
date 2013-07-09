#include <stdlib.h>
#include <string.h>
#include "rrExecutableModel.h"
#include "rrStringUtils.h"

namespace rr
{

void initModelData(ModelData &data)
{
    //Zero data structure..
    memset(&data, 0, sizeof(rr::ModelData));
}

/**
 * many implementations have a rrCalloc which returns a non zero pointer
 * if a zero size data block is requested, this returns zero which
 * makes debugging easier.
 */
static void *rrCalloc(size_t nmemb, size_t size)
{
    return nmemb * size != 0 ? calloc(nmemb, size) : 0;
}

void allocModelDataBuffers(ModelData &data, const string& modelName)
{
    data.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new is alligned differently than
    // malloc, so just use rrCalloc here just to be safe, plus rrCalloc returns zero
    // initialized memory.


    data.floatingSpeciesAmounts = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.floatingSpeciesConcentrationRates = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.rateRules = (double*)rrCalloc(data.numRateRules, sizeof(double));
    data.floatingSpeciesConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.reactionRates = (double*)rrCalloc(data.numReactions, sizeof(double));
    data.dependentSpeciesConservedSums = (double*)rrCalloc(data.numDependentSpecies, sizeof(double));
    data.floatingSpeciesInitConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.globalParameters = (double*)rrCalloc(data.numGlobalParameters, sizeof(double));
    data.compartmentVolumes = (double*)rrCalloc(data.numCompartments, sizeof(double));
    data.boundarySpeciesConcentrations = (double*)rrCalloc(data.numBoundarySpecies, sizeof(double));
    data.boundarySpeciesAmounts = (double*)rrCalloc(data.numBoundarySpecies, sizeof(double));
    data.sr = (double*)rrCalloc(data.srSize, sizeof(double));
    data.eventPriorities = (double*)rrCalloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)rrCalloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)rrCalloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)rrCalloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)rrCalloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)rrCalloc(data.eventTypeSize, sizeof(bool));
    data.floatingSpeciesCompartments = (int*)rrCalloc(data.numFloatingSpecies, sizeof(int));
    data.boundarySpeciesCompartments = (int*)rrCalloc(data.numBoundarySpecies, sizeof(int));
    data.work = (double*)rrCalloc(data.workSize, sizeof(int));

    // allocate space for the symbolic names of things
    data.variableTable = (char**)rrCalloc(data.numFloatingSpecies, sizeof(char*));
    data.boundaryTable = (char**)rrCalloc(data.numBoundarySpecies, sizeof(char*));
    data.globalParameterTable = (char**)rrCalloc(data.numGlobalParameters, sizeof(char*));


    //Event function pointer stuff
    data.eventAssignments =
            (TEventAssignmentDelegate*)rrCalloc(data.numEvents, sizeof(TEventAssignmentDelegate*));
    data.computeEventAssignments =
            (TComputeEventAssignmentDelegate*)rrCalloc(data.numEvents, sizeof(TComputeEventAssignmentDelegate*));
    data.performEventAssignments =
            (TPerformEventAssignmentDelegate*)rrCalloc(data.numEvents, sizeof(TPerformEventAssignmentDelegate*));
    data.eventDelays =
            (TEventDelayDelegate*)rrCalloc(data.numEvents, sizeof(TEventDelayDelegate*));
}

void  freeModelDataBuffers(ModelData &data)
{
    free(data.modelName);

    free(data.floatingSpeciesAmounts);
    free(data.floatingSpeciesConcentrationRates);
    free(data.rateRules);
    free(data.floatingSpeciesConcentrations);
    free(data.floatingSpeciesCompartments);
    free(data.reactionRates);
    free(data.dependentSpeciesConservedSums);
    free(data.floatingSpeciesInitConcentrations);
    free(data.globalParameters);
    free(data.compartmentVolumes);
    free(data.boundarySpeciesConcentrations);
    free(data.boundarySpeciesCompartments);
    free(data.boundarySpeciesAmounts);
    free(data.sr);
    free(data.eventPriorities);
    free(data.eventStatusArray);
    free(data.previousEventStatusArray);
    free(data.eventPersistentType);
    free(data.eventTests);
    free(data.eventType);
    free(data.work);

    // free names
    free(data.variableTable);
    free(data.boundaryTable);
    free(data.globalParameterTable);

    //Event function pointer stuff
    free(data.eventAssignments);
    free(data.computeEventAssignments);
    free(data.performEventAssignments);
    free(data.eventDelays);
}

} // namespace rr
