#ifndef add_noiseH
#define add_noiseH
#include "rrCapability.h"
#include "rrParameter.h"
#include "rrCPPPlugin.h"
#include "add_noise_thread.h"

//---------------------------------------------------------------------------

namespace addNoise
{

using namespace rr;
using namespace rrc;
using namespace rrp;

class AddNoise : public CPPPlugin
{
    public:
        enum NoiseType {ntGaussian = 0};

    private:
        Capability              mAddNoise;
        Parameter<NoiseType>    mNoiseType;
        Parameter<double>       mSigma;
        AddNoiseThread          mAddNoiseThread;

    public:
                                AddNoise(RoadRunner* aRR = NULL, PluginCallBackFnc fn1 = NULL, PluginCallBackFnc fn2 = NULL, PluginCallBackFnc fn3 = NULL);
                               ~AddNoise();

                                //user data is
        bool                    execute(void* userData, bool runInThread = true);
        bool                    isWorking(); //Returns true as long the thread is active..

        virtual _xmlNode*       createConfigNode();
        virtual void            loadConfig(const _xmlDoc* doc);
};

extern "C"
{
RR_PLUGIN_DECLSPEC Plugin*      plugins_cc createPlugin(rr::RoadRunner* aRR);
RR_PLUGIN_DECLSPEC const char*  plugins_cc getImplementationLanguage();
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
