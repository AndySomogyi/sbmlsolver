#ifndef add_noiseH
#define add_noiseH
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrPlugin.h"
#include "rrp_types.h"
#include "add_noise_thread.h"
#include "rrp_exporter.h"
//---------------------------------------------------------------------------

namespace addNoise
{

using namespace rr;
using namespace rrc;
using namespace rrp;

typedef void    (rrCallConv *WorkStartedCB)(void*);
typedef void    (rrCallConv *WorkFinishedCB)(void*);


class AddNoise : public Plugin
{
    public:
        enum NoiseType {ntGaussian = 0};

    private:
        Capability               mAddNoise;

        Parameter<NoiseType>    mNoiseType;
        Parameter<double>       mSigma;
        AddNoiseThread          mAddNoiseThread;

    public:
                                AddNoise(RoadRunner* aRR = NULL, WorkStartedCB fn1 = NULL, WorkFinishedCB fn2 = NULL);
                               ~AddNoise();

                                //user data is
        bool                    execute(void* userData);
        string                  getImplementationLanguage();
        bool                    isWorking(); //Returns true as long the thread is active..

        virtual _xmlNode*       createConfigNode();
        virtual void            loadConfig(const _xmlDoc* doc);
};

extern "C"
{
PLUGIN_DECLSPEC Plugin* rrCallConv    createPlugin(rr::RoadRunner* aRR);
PLUGIN_DECLSPEC const char* rrCallConv    getImplementationLanguage();
}

}

namespace rrp
{
template<>
string Parameter<addNoise::AddNoise::NoiseType>::getType() const
{
    return "NoiseType";
}

template<>
string Parameter<addNoise::AddNoise::NoiseType>::getValueAsString() const
{
    return "Gaussian";
}

template<>
void Parameter< addNoise::AddNoise::NoiseType >::setValueFromString(const string& val)
{
    mValue = addNoise::AddNoise::ntGaussian;
}

}

#endif
