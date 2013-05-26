#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrUtils.h"
#include "rrModelState.h"
//---------------------------------------------------------------------------

namespace rr
{
ModelState::ModelState(ModelFromC& model)
{
    InitializeFromModel(model);
}

void ModelState::InitializeFromModel(ModelFromC& model)
{
    model.convertToConcentrations();
//    copyCArrayToStdVector(model.y,                        mFloatingSpeciesConcentrations,       *model.ySize);
    copyCArrayToStdVector(model.mData.bc,                       mBoundarySpeciesConcentrations,       model.mData.bcSize);
    copyCArrayToStdVector(model.mData.c,                        mCompartmentVolumes,                  model.mData.cSize);
    copyCArrayToStdVector(model.mData.gp,                       mGlobalParameters,                    model.mData.gpSize);
    copyCArrayToStdVector(model.mData.ct,                       mConservedTotals,                     model.mData.ctSize);
    copyCArrayToStdVector(model.mData.dydt,                     mDyDt,                                model.mData.dydtSize);
    copyCArrayToStdVector(model.mData.rates,                    mRates,                               model.mData.ratesSize);
    copyCArrayToStdVector(model.mData.rateRules,                mRateRules,                           model.mData.rateRulesSize);
    copyCArrayToStdVector(model.mData.sr,                       mModifiableSpeciesReferences,         model.mData.srSize);
    copyCArrayToStdVector(model.mData.eventStatusArray,         mEventStatusArray,                    model.mData.eventStatusArraySize);
    copyCArrayToStdVector(model.mData.eventTests,               mEventTests,                          model.mData.eventTestsSize);
    copyCArrayToStdVector(model.mData.previousEventStatusArray, mPreviousEventStatusArray,            model.mData.previousEventStatusArraySize);
    mTime = model.mData.time;
}

void ModelState::AssignToModel(ModelFromC& model)
{
//    copyStdVectorToCArray(mFloatingSpeciesConcentrations,   model.y,                        *model.ySize                        );
    copyStdVectorToCArray(mBoundarySpeciesConcentrations,   model.mData.bc,                       model.mData.bcSize                       );
    copyStdVectorToCArray(mCompartmentVolumes,              model.mData.c,                        model.mData.cSize                        );
    copyStdVectorToCArray(mGlobalParameters,                model.mData.gp,                       model.mData.gpSize                       );
    copyStdVectorToCArray(mConservedTotals,                 model.mData.ct,                       model.mData.ctSize                       );
    copyStdVectorToCArray(mDyDt,                            model.mData.dydt,                     model.mData.dydtSize                     );
    copyStdVectorToCArray(mRates,                           model.mData.rates,                    model.mData.ratesSize                    );
    copyStdVectorToCArray(mRateRules,                       model.mData.rateRules,                model.mData.rateRulesSize                );
    copyStdVectorToCArray(mEventTests,                      model.mData.eventTests,               model.mData.eventTestsSize               );
    copyStdVectorToCArray(mEventStatusArray,                model.mData.eventStatusArray,         model.mData.eventStatusArraySize         );
    copyStdVectorToCArray(mPreviousEventStatusArray,        model.mData.previousEventStatusArray, model.mData.previousEventStatusArraySize );
    copyStdVectorToCArray(mModifiableSpeciesReferences,     model.mData.sr,                       model.mData.srSize                        );
    model.convertToAmounts();
    model.setTime(mTime);
}

}


