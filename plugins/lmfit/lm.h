#ifndef lmH
#define lmH
#include <vector>
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrRoadRunner.h"
#include "rrMinimizationData.h"
#include "rrc_types.h"
#include "LMWorker.h"
//---------------------------------------------------------------------------

namespace lmfit
{
using namespace rrc;
using namespace rrp;
using rr::RoadRunner;
using std::string;

class LM : public Plugin
{
    friend class LMWorker;

    protected:
        Capability                              mLMFit;
        Parameter<string>                       mTempFolder;
        Parameter<string>                       mSBML;                          //This is the model

        Parameter<RoadRunnerData>               mObservedData;
        Parameter<RoadRunnerData>               mModelData;
        Parameter<RoadRunnerData>               mResidualsData;
        Parameter<MinimizationData>             mMinimizationData;              //Generate its own
        Parameter<Parameters>                   mInputParameterList;            //Parameters to fit
        Parameter<Parameters>                   mOutputParameterList;           //Parameters that was fitted
        Parameter<StringList>                   mObservedDataSelectionList;     //Species selection list for observed data
        Parameter<StringList>                   mModelDataSelectionList;        //Species selection list for observed data

        //Utility functions for the thread
        string                                  getTempFolder();
        string                                  getSBML();

        MinimizationData&                       getMinimizationData();

        //The worker is doing the work
        LMWorker                                mLMWorker;

    public:
                                                LM(RoadRunner* aRR = NULL);
                                               ~LM();

        bool                                    execute(void* data, bool inThread = false);
        string                                  getResult();
        bool                                    resetPlugin();
        bool                                    setInputData(void* data);
        string                                  getImplementationLanguage();
        string                                  getStatus();
        bool                                    isWorking();

        unsigned char*                          getManualAsPDF() const;
        unsigned int                            getPDFManualByteSize();
        StringList                              getObservedDataSelectionList();
        virtual _xmlNode*                       createConfigNode(){return NULL;}
        virtual void                            loadConfig(const _xmlDoc* doc){}
};

extern "C"
{
RR_PLUGIN_DECLSPEC LM*         plugins_cc    createPlugin(rr::RoadRunner* aRR);
RR_PLUGIN_DECLSPEC const char* plugins_cc    getImplementationLanguage();
}
}
#endif
