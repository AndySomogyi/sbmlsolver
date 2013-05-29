#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrUtils.h"
#include "rrModelState.h"
//---------------------------------------------------------------------------

namespace rr
{
ModelState::ModelState(ExecutableModel& model)
{
    InitializeFromModel(model);
}

void ModelState::InitializeFromModel(ExecutableModel& model)
{
	ModelData &modelData = model.getModelData();
    model.convertToConcentrations();
//    CopyCArrayToStdVector(model.y,                        mFloatingSpeciesConcentrations,       *model.ySize);
    copyCArrayToStdVector(modelData.bc,                       mBoundarySpeciesConcentrations,       modelData.bcSize);
    copyCArrayToStdVector(modelData.c,                        mCompartmentVolumes,                  modelData.cSize);
    copyCArrayToStdVector(modelData.gp,                       mGlobalParameters,                    modelData.gpSize);
    copyCArrayToStdVector(modelData.ct,                       mConservedTotals,                     modelData.ctSize);
    copyCArrayToStdVector(modelData.dydt,                     mDyDt,                                modelData.dydtSize);
    copyCArrayToStdVector(modelData.rates,                    mRates,                               modelData.ratesSize);
    copyCArrayToStdVector(modelData.rateRules,                mRateRules,                           modelData.rateRulesSize);
    copyCArrayToStdVector(modelData.sr,                       mModifiableSpeciesReferences,         modelData.srSize);
    copyCArrayToStdVector(modelData.eventStatusArray,         mEventStatusArray,                    modelData.eventStatusArraySize);
    copyCArrayToStdVector(modelData.eventTests,               mEventTests,                          modelData.eventTestsSize);
    copyCArrayToStdVector(modelData.previousEventStatusArray, mPreviousEventStatusArray,            modelData.previousEventStatusArraySize);
    mTime = modelData.time;
}

void ModelState::AssignToModel(ExecutableModel& model)
{
	ModelData &modelData = model.getModelData();
//    CopyStdVectorToCArray(mFloatingSpeciesConcentrations,   model.y,                        *model.ySize                        );
    copyStdVectorToCArray(mBoundarySpeciesConcentrations,   modelData.bc,                       modelData.bcSize                       );
    copyStdVectorToCArray(mCompartmentVolumes,              modelData.c,                        modelData.cSize                        );
    copyStdVectorToCArray(mGlobalParameters,                modelData.gp,                       modelData.gpSize                       );
    copyStdVectorToCArray(mConservedTotals,                 modelData.ct,                       modelData.ctSize                       );
    copyStdVectorToCArray(mDyDt,                            modelData.dydt,                     modelData.dydtSize                     );
    copyStdVectorToCArray(mRates,                           modelData.rates,                    modelData.ratesSize                    );
    copyStdVectorToCArray(mRateRules,                       modelData.rateRules,                modelData.rateRulesSize                );
    copyStdVectorToCArray(mEventTests,                      modelData.eventTests,               modelData.eventTestsSize               );
    copyStdVectorToCArray(mEventStatusArray,                modelData.eventStatusArray,         modelData.eventStatusArraySize         );
    copyStdVectorToCArray(mPreviousEventStatusArray,        modelData.previousEventStatusArray, modelData.previousEventStatusArraySize );
    copyStdVectorToCArray(mModifiableSpeciesReferences,     modelData.sr,                       modelData.srSize                        );
    model.convertToAmounts();
    model.setTime(mTime);
}

}


