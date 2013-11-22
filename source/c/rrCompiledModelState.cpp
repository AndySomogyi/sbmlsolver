#pragma hdrstop
#include "rrUtils.h"
#include "rrCompiledExecutableModel.h"
#include "rrCompiledModelState.h"
//---------------------------------------------------------------------------

namespace rr
{
CompiledModelState::CompiledModelState(CompiledExecutableModel& model)
{
    InitializeFromModel(model);
}

void CompiledModelState::InitializeFromModel(CompiledExecutableModel& model)
{
    ModelData &modelData = model.mData;
    model.convertToConcentrations();
//    CopyCArrayToStdVector(model.y,                        mFloatingSpeciesConcentrations,       *model.ySize);
    copyCArrayToStdVector(modelData.boundarySpeciesConcentrations,        mBoundarySpeciesConcentrations,       modelData.numBoundarySpecies);
    copyCArrayToStdVector(modelData.compartmentVolumes,                   mCompartmentVolumes,                  modelData.numCompartments);
    copyCArrayToStdVector(modelData.globalParameters,                     mGlobalParameters,                    modelData.numGlobalParameters);
    copyCArrayToStdVector(modelData.dependentSpeciesConservedSums,        mConservedTotals,                     modelData.numDependentSpecies);
    copyCArrayToStdVector(modelData.floatingSpeciesAmountRates,           mDyDt,                                modelData.numFloatingSpecies);
    copyCArrayToStdVector(modelData.reactionRates,                        mRates,                               modelData.numReactions);
    copyCArrayToStdVector(modelData.rateRuleRates,                        mRateRules,                           modelData.numRateRules);
    copyCArrayToStdVector(modelData.sr,                                   mModifiableSpeciesReferences,         modelData.srSize);
    copyCArrayToStdVector(modelData.eventStatusArray,                     mEventStatusArray,                    modelData.eventStatusArraySize);
    copyCArrayToStdVector(modelData.eventTests,                           mEventTests,                          modelData.eventTestsSize);
    copyCArrayToStdVector(modelData.previousEventStatusArray,             mPreviousEventStatusArray,            modelData.previousEventStatusArraySize);
    mTime = modelData.time;
}

void CompiledModelState::AssignToModel(CompiledExecutableModel& model)
{
    ModelData &modelData = model.mData;
//    CopyStdVectorToCArray(mFloatingSpeciesConcentrations,   model.y,                        *model.ySize                        );
    copyStdVectorToCArray(mBoundarySpeciesConcentrations,   modelData.boundarySpeciesConcentrations,        modelData.numBoundarySpecies           );
    copyStdVectorToCArray(mCompartmentVolumes,              modelData.compartmentVolumes,                   modelData.numCompartments              );
    copyStdVectorToCArray(mGlobalParameters,                modelData.globalParameters,                     modelData.numGlobalParameters          );
    copyStdVectorToCArray(mConservedTotals,                 modelData.dependentSpeciesConservedSums,        modelData.numDependentSpecies          );
    copyStdVectorToCArray(mDyDt,                            modelData.floatingSpeciesAmountRates,           modelData.numFloatingSpecies           );
    copyStdVectorToCArray(mRates,                           modelData.reactionRates,                        modelData.numReactions                 );
    copyStdVectorToCArray(mRateRules,                       modelData.rateRuleRates,                        modelData.numRateRules                 );
    copyStdVectorToCArray(mEventTests,                      modelData.eventTests,                           modelData.eventTestsSize               );
    copyStdVectorToCArray(mEventStatusArray,                modelData.eventStatusArray,                     modelData.eventStatusArraySize         );
    copyStdVectorToCArray(mPreviousEventStatusArray,        modelData.previousEventStatusArray,             modelData.previousEventStatusArraySize );
    copyStdVectorToCArray(mModifiableSpeciesReferences,     modelData.sr,                                   modelData.srSize                       );
    model.convertToAmounts();
    model.setTime(mTime);
}

}


