#include "rrExecutableModel.h"
#include "rrStringUtils.h"
#include <stdlib.h>
#include <string.h>


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
    // for(int i = model->mData.rateRulesSize; i < model->mData.amountsSize + model->mData.rateRulesSize; i++) {
    //    dTemp[i] = model->getModelData().amounts[i];
    // so to fix the memory corruption issues, allocate it to the used, not the indicated size.
    // TODO fix this correctly!

    data.amounts = (double*)rrCalloc(data.amountsSize + data.rateRulesSize, sizeof(double));
    data.dydt = (double*)rrCalloc(data.dydtSize, sizeof(double));
    data.rateRules = (double*)rrCalloc(data.rateRulesSize, sizeof(double));
    data.y = (double*)rrCalloc(data.ySize, sizeof(double));
    data.rates = (double*)rrCalloc(data.ratesSize, sizeof(double));
    data.ct = (double*)rrCalloc(data.ctSize, sizeof(double));
    data.init_y = (double*)rrCalloc(data.init_ySize, sizeof(double));
    data.gp = (double*)rrCalloc(data.gpSize, sizeof(double));
    data.c = (double*)rrCalloc(data.cSize, sizeof(double));
    data.bc = (double*)rrCalloc(data.bcSize, sizeof(double));
    data.lp = (double*)rrCalloc(data.lpSize, sizeof(double));
    data.sr = (double*)rrCalloc(data.srSize, sizeof(double));
    data.localParameterDimensions = (int*)rrCalloc(data.localParameterDimensionsSize, sizeof(int));
    data.eventPriorities = (double*)rrCalloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)rrCalloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)rrCalloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)rrCalloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)rrCalloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)rrCalloc(data.eventTypeSize, sizeof(bool));

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
    free(data.dydt);
    free(data.rateRules);
    free(data.y);
    free(data.rates);
    free(data.ct);
    free(data.init_y);
    free(data.gp);
    free(data.c);
    free(data.bc);
    free(data.lp);
    free(data.sr);
    free(data.localParameterDimensions);
    free(data.eventPriorities);
    free(data.eventStatusArray);
    free(data.previousEventStatusArray);
    free(data.eventPersistentType);
    free(data.eventTests);
    free(data.eventType);

    //Event function pointer stuff
    free(data.eventAssignments);
    free(data.computeEventAssignments);
    free(data.performEventAssignments);
    free(data.eventDelays);
}

} // namespace rr
