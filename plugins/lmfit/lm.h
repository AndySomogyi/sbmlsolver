#ifndef lmH
#define lmH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
#include "rrc_types.h"
#include "lm_thread.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;
using namespace std;

class LM : public Plugin
{
    friend class LMFitThread;

    protected:
        Capability                              mLMFit;
        Parameter<string>                       mTempFolder;
        Parameter<string>                       mSBML;                    //This is the model
        Parameter<MinimizationData>             mMinimizationData;        //Generate its own

        //Utility functions for the thread
        string                                  getTempFolder();
        string                                  getSBML();
        Parameter<string>                       getParameterToFit();
        MinimizationData&                       getMinimizationData();

        //The thread is doing the work
        LMFitThread                             mLMFitThread;

    public:
                                                LM(rr::RoadRunner* aRR = NULL);
                                                ~LM();
        bool                                    execute(void* inputData);
        string                                  getResult();
        bool                                    resetPlugin();
        bool                                    setInputData(void* data);
        string                                  getImplementationLanguage();
        string									getStatus();
		bool 									isWorking();
};

extern "C"
{
PLUGIN_DECLSPEC rr::Plugin* rrCallConv    createPlugin(rr::RoadRunner* aRR);
PLUGIN_DECLSPEC char* 		rrCallConv    getImplementationLanguage();
}


#endif
