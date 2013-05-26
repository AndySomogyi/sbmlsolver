#ifndef lmH
#define lmH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "rrMinimizationResult.h"
#include "../../Wrappers/C/rrc_types.h"
#include "lm_thread.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;
using namespace std;

class LM : public Plugin
{
	friend LMFitThread;

	protected:
    	Capability						    mLMFit;
		Parameter<string> 				    mTempFolder;
		Parameter<string> 				    mSBML;					//This is the model
        Parameter<MinimizationResult*>		mMinimizationData;				//Generate its own

        //Utility functions for the thread
        string							    getTempFolder();
        string							    getSBML();
		Parameter<string>				    getParameterToFit();
        MinimizationResult*					getMinimizationData();
        //This thread is doing the work
		LMFitThread							mLMFitThread;


    public:
    							            LM(rr::RoadRunner* aRR = NULL);
					   		   	           ~LM();
		bool					            execute(void* inputData);
        string								getResult();
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);
}


#endif
