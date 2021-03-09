#ifndef rrModelStateH
#define rrModelStateH

#include "rrOSSpecifics.h"
#include <vector>


using std::vector;


namespace rr
{

class CompiledExecutableModel;

/**
 * Saves the 'state' of a Model.
 *
 * TODO: GET RID OF THIS! Give the a proper clone() method if we need
 * modify it! Or at least, write a ModelData clone function!.
 *
 * This class re-implements ModelData, and copies the values out of and into
 * a ModelData struct.
 */
class RR_DECLSPEC CompiledModelState
{
    protected:
    public:
        //Todo:  Lot of stuff is wrong here.. ??
        double                          mTime;
        std::vector<double>                  mBoundarySpeciesConcentrations;
        std::vector<double>                  mCompartmentVolumes;
        std::vector<double>                  mConservedTotals;
        std::vector<double>                  mDyDt;
        std::vector<double>                  mFloatingSpeciesConcentrations;
        std::vector<double>                  mGlobalParameters;
        std::vector<bool>                    mPreviousEventStatusArray;
        std::vector<double>                  mRateRules;
        std::vector<double>                  mRates;
        std::vector<double>                  mModifiableSpeciesReferences;
        std::vector<bool>                    mEventStatusArray;
        std::vector<double>                  mEventTests;

        void                            InitializeFromModel(CompiledExecutableModel& model);

    public:
                                        CompiledModelState(CompiledExecutableModel& model);
        void                            AssignToModel(CompiledExecutableModel& model);
};

}
#endif
