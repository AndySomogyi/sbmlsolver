#ifndef rrModelStateH
#define rrModelStateH
#include <ostream>
#include <istream>
#include <vector>
#include "rrObject.h"
#include "rrExecutableModel.h"

using std::vector;
using std::istream;
using std::ostream;

namespace rr
{

/**
 * Saves the 'state' of a Model.
 *
 * TODO: GET RID OF THIS! Give the a proper clone() method if we need
 * modify it! Or at least, write a ModelData clone function!.
 *
 * This class re-implements ModelData, and copies the values out of and into
 * a ModelData struct.
 */
class RR_DECLSPEC CompiledModelState : public rrObject
{
    protected:
    public:
        //Todo:  Lot of stuff is wrong here.. ??
        double                          mTime;
        vector<double>                  mBoundarySpeciesConcentrations;
        vector<double>                  mCompartmentVolumes;
        vector<double>                  mConservedTotals;
        vector<double>                  mDyDt;
        vector<double>                  mFloatingSpeciesConcentrations;
        vector<double>                  mGlobalParameters;
        vector<bool>                    mPreviousEventStatusArray;
        vector<double>                  mRateRules;
        vector<double>                  mRates;
        vector<double>                  mModifiableSpeciesReferences;
        vector<bool>                    mEventStatusArray;
        vector<double>                  mEventTests;

        void                            InitializeFromModel(ExecutableModel& model);

    public:
                                        CompiledModelState(ExecutableModel& model);
        void                            AssignToModel(ExecutableModel& model);
};

}
#endif
