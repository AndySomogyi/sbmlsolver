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
    copyCArrayToStdVector(modelData.boundarySpeciesConcentrations,        mBoundarySpeciesConcentrations,       modelData.numBoundarySpecies);
    copyCArrayToStdVector(modelData.compartmentVolumes,                   mCompartmentVolumes,                  modelData.numCompartments);
    copyCArrayToStdVector(modelData.gp,                                   mGlobalParameters,                    modelData.gpSize);
    copyCArrayToStdVector(modelData.ct,                                   mConservedTotals,                     modelData.ctSize);
    copyCArrayToStdVector(modelData.floatingSpeciesConcentrationRates,    mDyDt,                                modelData.numFloatingSpecies);
    copyCArrayToStdVector(modelData.reactionRates,                        mRates,                               modelData.numReactions);
    copyCArrayToStdVector(modelData.rateRules,                            mRateRules,                           modelData.numRateRules);
    copyCArrayToStdVector(modelData.sr,                                   mModifiableSpeciesReferences,         modelData.srSize);
    copyCArrayToStdVector(modelData.eventStatusArray,                     mEventStatusArray,                    modelData.eventStatusArraySize);
    copyCArrayToStdVector(modelData.eventTests,                           mEventTests,                          modelData.eventTestsSize);
    copyCArrayToStdVector(modelData.previousEventStatusArray,             mPreviousEventStatusArray,            modelData.previousEventStatusArraySize);
    mTime = modelData.time;
}

void ModelState::AssignToModel(ExecutableModel& model)
{
    ModelData &modelData = model.getModelData();
//    CopyStdVectorToCArray(mFloatingSpeciesConcentrations,   model.y,                        *model.ySize                        );
    copyStdVectorToCArray(mBoundarySpeciesConcentrations,   modelData.boundarySpeciesConcentrations,        modelData.numBoundarySpecies           );
    copyStdVectorToCArray(mCompartmentVolumes,              modelData.compartmentVolumes,                   modelData.numCompartments              );
    copyStdVectorToCArray(mGlobalParameters,                modelData.gp,                                   modelData.gpSize                       );
    copyStdVectorToCArray(mConservedTotals,                 modelData.ct,                                   modelData.ctSize                       );
    copyStdVectorToCArray(mDyDt,                            modelData.floatingSpeciesConcentrationRates,    modelData.numFloatingSpecies           );
    copyStdVectorToCArray(mRates,                           modelData.reactionRates,                        modelData.numReactions                 );
    copyStdVectorToCArray(mRateRules,                       modelData.rateRules,                            modelData.numRateRules                 );
    copyStdVectorToCArray(mEventTests,                      modelData.eventTests,                           modelData.eventTestsSize               );
    copyStdVectorToCArray(mEventStatusArray,                modelData.eventStatusArray,                     modelData.eventStatusArraySize         );
    copyStdVectorToCArray(mPreviousEventStatusArray,        modelData.previousEventStatusArray,             modelData.previousEventStatusArraySize );
    copyStdVectorToCArray(mModifiableSpeciesReferences,     modelData.sr,                                   modelData.srSize                       );
    model.convertToAmounts();
    model.setTime(mTime);
}

}


