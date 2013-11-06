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

namespace lm
{
using namespace rrp;
using rr::RoadRunner;
using std::string;

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

        MinimizationData&                       getMinimizationData();

        //The thread is doing the work
        LMFitThread                             mLMFitThread;

    public:
                                                LM(RoadRunner* aRR = NULL);
                                               ~LM();
        bool                                    execute(void* inputData, bool runInThread = true);
        string                                  getResult();
        bool                                    resetPlugin();
        bool                                    setInputData(void* data);
        string                                  getImplementationLanguage();
        string                                  getStatus();
        bool                                    isWorking();

        virtual _xmlNode*                       createConfigNode();
        virtual void                            loadConfig(const _xmlDoc* doc);
};

extern "C"
{
RR_PLUGIN_DECLSPEC LM*         plugins_cc    createPlugin(rr::RoadRunner* aRR);
RR_PLUGIN_DECLSPEC const char* plugins_cc    getImplementationLanguage();
}
}
#endif
