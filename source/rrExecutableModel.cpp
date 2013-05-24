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

void allocModelDataBuffers(ModelData &data, const string& modelName)
{
    data.modelName = createText(modelName);

    // in certain cases, the data returned by c++ new is alligned differently than
    // malloc, so just use calloc here just to be safe, plus calloc returns zero
    // initialized memory.

    data.amounts = (double*)calloc(data.amountsSize, sizeof(double));
    data.dydt = (double*)calloc(data.dydtSize, sizeof(double));
    data.rateRules = (double*)calloc(data.rateRulesSize, sizeof(double));
    data.y = (double*)calloc(data.ySize, sizeof(double));
    data.rates = (double*)calloc(data.ratesSize, sizeof(double));
    data.ct = (double*)calloc(data.ctSize, sizeof(double));
    data.init_y = (double*)calloc(data.init_ySize, sizeof(double));
    data.gp = (double*)calloc(data.gpSize, sizeof(double));
    data.c = (double*)calloc(data.cSize, sizeof(double));
    data.bc = (double*)calloc(data.bcSize, sizeof(double));
    data.lp = (double*)calloc(data.lpSize, sizeof(double));
    data.sr = (double*)calloc(data.srSize, sizeof(double));
    data.localParameterDimensions = (int*)calloc(data.localParameterDimensionsSize, sizeof(int));
    data.eventPriorities = (double*)calloc(data.eventPrioritiesSize, sizeof(double));
    data.eventStatusArray = (bool*)calloc(data.eventStatusArraySize, sizeof(bool));
    data.previousEventStatusArray = (bool*)calloc(data.previousEventStatusArraySize, sizeof(bool));
    data.eventPersistentType = (bool*)calloc(data.eventPersistentTypeSize, sizeof(bool));
    data.eventTests = (double*)calloc(data.eventTestsSize, sizeof(double));
    data.eventType = (bool*)calloc(data.eventTypeSize, sizeof(bool));

    if (data.numEvents > 0)
    {
        //Event function pointer stuff
        data.eventAssignments =
                (TEventAssignmentDelegate*)calloc(data.numEvents, sizeof(TEventAssignmentDelegate*));
        data.computeEventAssignments =
                (TComputeEventAssignmentDelegate*)calloc(data.numEvents, sizeof(TComputeEventAssignmentDelegate*));
        data.performEventAssignments =
                (TPerformEventAssignmentDelegate*)calloc(data.numEvents, sizeof(TPerformEventAssignmentDelegate*));
        data.eventDelays =
                (TEventDelayDelegate*)calloc(data.numEvents, sizeof(TEventDelayDelegate*));
    }
}

void  freeModelDataBuffers(ModelData &data)
{
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
