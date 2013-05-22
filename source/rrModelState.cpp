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
    CopyCArrayToStdVector(modelData.bc,                       mBoundarySpeciesConcentrations,       modelData.bcSize);
    CopyCArrayToStdVector(modelData.c,                        mCompartmentVolumes,                  modelData.cSize);
    CopyCArrayToStdVector(modelData.gp,                       mGlobalParameters,                    modelData.gpSize);
    CopyCArrayToStdVector(modelData.ct,                       mConservedTotals,                     modelData.ctSize);
    CopyCArrayToStdVector(modelData.dydt,                     mDyDt,                                modelData.dydtSize);
    CopyCArrayToStdVector(modelData.rates,                    mRates,                               modelData.ratesSize);
    CopyCArrayToStdVector(modelData.rateRules,                mRateRules,                           modelData.rateRulesSize);
    CopyCArrayToStdVector(modelData.sr,                       mModifiableSpeciesReferences,         modelData.srSize);
    CopyCArrayToStdVector(modelData.eventStatusArray,         mEventStatusArray,                    modelData.eventStatusArraySize);
    CopyCArrayToStdVector(modelData.eventTests,               mEventTests,                          modelData.eventTestsSize);
    CopyCArrayToStdVector(modelData.previousEventStatusArray, mPreviousEventStatusArray,            modelData.previousEventStatusArraySize);
    mTime = modelData.time;
}

void ModelState::AssignToModel(ExecutableModel& model)
{
	ModelData &modelData = model.getModelData();
//    CopyStdVectorToCArray(mFloatingSpeciesConcentrations,   model.y,                        *model.ySize                        );
    CopyStdVectorToCArray(mBoundarySpeciesConcentrations,   modelData.bc,                       modelData.bcSize                       );
    CopyStdVectorToCArray(mCompartmentVolumes,              modelData.c,                        modelData.cSize                        );
    CopyStdVectorToCArray(mGlobalParameters,                modelData.gp,                       modelData.gpSize                       );
    CopyStdVectorToCArray(mConservedTotals,                 modelData.ct,                       modelData.ctSize                       );
    CopyStdVectorToCArray(mDyDt,                            modelData.dydt,                     modelData.dydtSize                     );
    CopyStdVectorToCArray(mRates,                           modelData.rates,                    modelData.ratesSize                    );
    CopyStdVectorToCArray(mRateRules,                       modelData.rateRules,                modelData.rateRulesSize                );
    CopyStdVectorToCArray(mEventTests,                      modelData.eventTests,               modelData.eventTestsSize               );
    CopyStdVectorToCArray(mEventStatusArray,                modelData.eventStatusArray,         modelData.eventStatusArraySize         );
    CopyStdVectorToCArray(mPreviousEventStatusArray,        modelData.previousEventStatusArray, modelData.previousEventStatusArraySize );
    CopyStdVectorToCArray(mModifiableSpeciesReferences,     modelData.sr,                       modelData.srSize                        );
    model.convertToAmounts();
    model.setTime(mTime);
}

}


