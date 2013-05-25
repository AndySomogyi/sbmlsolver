#ifndef full_space_minimizationH
#define full_space_minimizationH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "../../Wrappers/C/rrc_types.h"
#include "full_space_fit_thread.h"
//---------------------------------------------------------------------------

namespace fullSpaceFit
{
using namespace rr;
using namespace rrc;
using namespace std;

class FullSpaceMinimization : public Plugin
{
	friend FullSpaceFitThread;

	private:
    	Capability						mOneParameterFit;
		Parameter<string> 				mTempFolder;
		Parameter<string> 				mSBML;					//This is the model
		Parameter< vector<string> > 	mParametersToFit;
        Parameter<int>					mStepsPerDimension;
        Parameter<double>				mParameterSweepRange;	//In percent

        Parameter<RRData*>  			mInputData;         	//Assign from the outside.
        Parameter<RRData*>  			mResultData;			//Generate its own
        Parameter<int>					mNumberOfThreads;
   		Parameter<double>		        mChiSquare;

        //Utility functions for the thread
        string							getTempFolder();
        string							getSBML();
		vector<string>					getParametersToFit();

        //This thread is doing the work
		FullSpaceFitThread				mFullSpaceFitThread;

    public:
    							        FullSpaceMinimization(rr::RoadRunner* aRR = NULL);
					   		   	       ~FullSpaceMinimization();
		bool					        execute(void* inputData);
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);
}

}
#endif
