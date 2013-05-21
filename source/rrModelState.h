#ifndef rrModelStateH
#define rrModelStateH
#include <ostream>
#include <istream>
#include <vector>
#include "rrObject.h"
#include "rrModelFromC.h"

using std::vector;
using std::istream;
using std::ostream;

namespace rr
{
class RR_DECLSPEC ModelState : public rrObject
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
                                        ModelState(ExecutableModel& model);
        void                            WriteTo(const string& fileName);
        static ModelState               ReadFrom(istream& stream);
        static ModelState               ReadFrom(const string& fileName);
        void                            WriteTo(ostream& stream);
        void                            AssignToModel(ExecutableModel& model);
};

}
#endif
