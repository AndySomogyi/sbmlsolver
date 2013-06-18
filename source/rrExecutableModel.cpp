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

    // in rrNLEQInterface there is a loop like
    // for(int i = model->mData.rateRulesSize; i < model->mData.numFloatingSpecies + model->mData.rateRulesSize; i++) {
    //    dTemp[i] = model->getModelData().amounts[i];
    // so to fix the memory corruption issues, allocate it to the used, not the indicated size.
    // TODO fix this correctly!

    data.amounts = (double*)rrCalloc(data.numFloatingSpecies + data.rateRulesSize, sizeof(double));
    data.floatingSpeciesConcentrationRates = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.rateRules = (double*)rrCalloc(data.rateRulesSize, sizeof(double));
    data.floatingSpeciesConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.rates = (double*)rrCalloc(data.ratesSize, sizeof(double));
    data.ct = (double*)rrCalloc(data.ctSize, sizeof(double));
    data.floatingSpeciesInitConcentrations = (double*)rrCalloc(data.numFloatingSpecies, sizeof(double));
    data.gp = (double*)rrCalloc(data.gpSize, sizeof(double));
    data.compartmentVolumes = (double*)rrCalloc(data.numCompartments, sizeof(double));
    data.boundarySpeciesConcentrations = (double*)rrCalloc(data.numBoundarySpecies, sizeof(double));
    data.lp = (double*)rrCalloc(data.lpSize, sizeof(double));
    data.sr = (double*)rrCalloc(data.srSize, sizeof(double));
    data.localParameterDimensions = (int*)rrCalloc(data.localParameterDimensionsSize, sizeof(int));
    data.eventPriorities = (double*)rrCalloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)rrCalloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)rrCalloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)rrCalloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)rrCalloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)rrCalloc(data.eventTypeSize, sizeof(bool));
    data.floatingSpeciesCompartments = (int*)rrCalloc(data.numFloatingSpecies, sizeof(int));
    data.boundarySpeciesCompartments = (int*)rrCalloc(data.numBoundarySpecies, sizeof(int));

    // allocate space for the symbolic names of things
    data.variableTable = (char**)rrCalloc(data.numTotalVariables, sizeof(char*));
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

    free(data.amounts);
    free(data.floatingSpeciesConcentrationRates);
    free(data.rateRules);
    free(data.floatingSpeciesConcentrations);
    free(data.floatingSpeciesCompartments);
    free(data.rates);
    free(data.ct);
    free(data.floatingSpeciesInitConcentrations);
    free(data.gp);
    free(data.compartmentVolumes);
    free(data.boundarySpeciesConcentrations);
    free(data.boundarySpeciesCompartments);
    free(data.lp);
    free(data.sr);
    free(data.localParameterDimensions);
    free(data.eventPriorities);
    free(data.eventStatusArray);
    free(data.previousEventStatusArray);
    free(data.eventPersistentType);
    free(data.eventTests);
    free(data.eventType);

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
