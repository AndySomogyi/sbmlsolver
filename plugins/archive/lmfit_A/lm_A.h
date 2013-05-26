#ifndef lm_AH
#define lm_AH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
#include "../../Wrappers/C/rrc_types.h"
#include "lm_A_thread.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;
using namespace std;

class LM_A : public Plugin
{
	friend LM_A_Thread;

	protected:
    	Capability						    mLM_AFit;
		Parameter<string> 				    mTempFolder;
		Parameter<string> 				    mSBML;					//This is the model
        Parameter<MinimizationData>			mMinimizationData;		//Generate its own

        //Utility functions for the thread
        string							    getTempFolder();
        string							    getSBML();
		Parameter<string>				    getParameterToFit();
        MinimizationData&					getMinimizationData();

        //The thread is doing the work
		LM_A_Thread							mLM_AFitThread;

    public:
    							            LM_A(rr::RoadRunner* aRR = NULL);
					   		   	           ~LM_A();
		bool					            execute(void* inputData);
        string								getResult();
        bool								resetPlugin();
		bool								setInputData(void* data);
};

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);
}


#endif
