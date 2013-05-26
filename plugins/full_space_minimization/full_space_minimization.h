#ifndef full_space_minimizationH
#define full_space_minimizationH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "rrMinimizationResult.h"
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

	protected:
    	Capability						    mOneParameterFit;
		Parameter<string> 				    mTempFolder;
		Parameter<string> 				    mSBML;					//This is the model
		Parameter<string> 	   	 			mParameterToFit;
        Parameter<int>					    mStepsPerDimension;
        Parameter<double>				    mParameterSweepRange;	//In percent

        Parameter<RRData*>  				mInputData;         	//Assign from the outside.
        MinimizationResult  				mResult;				//Generate its own
        Parameter<int>						mNumberOfThreads;
   		Parameter<double>		       	 	mChiSquare;

        //Utility functions for the thread
        string							    getTempFolder();
        string							    getSBML();
		Parameter<string>				    getParameterToFit();

        //This thread is doing the work
		FullSpaceFitThread					mFullSpaceFitThread;

    public:
    							            FullSpaceMinimization(rr::RoadRunner* aRR = NULL);
					   		   	           ~FullSpaceMinimization();
		bool					            execute(void* inputData);
        string								getResult();
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);
}

}
#endif
