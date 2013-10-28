#ifndef TestPluginH
#define TestPluginH
#include "../source/rrCPPPlugin.h"
#include "../source/rrCapability.h"
#include "../source/rrParameter.h"
//---------------------------------------------------------------------------

namespace TestPlugin
{
class TestPlugin : public rrp::CPPPlugin
{
    private:
        rrp::Capability             mTestCapability;
        rrp::Parameter<int>         mTestParameter;

    public:
                                    TestPlugin(rr::RoadRunner* aRR = NULL, int testParameter = 0);
                                   ~TestPlugin();
        bool                        execute(void* userData);
        virtual _xmlNode*           createConfigNode(){return NULL;}
        virtual void                loadConfig(const _xmlDoc* doc){}

};

extern "C"
{

RR_PLUGIN_DECLSPEC  const char*     rrpCallConv    getImplementationLanguage();
RR_PLUGIN_DECLSPEC  TestPlugin*     rrpCallConv    createPlugin(rr::RoadRunner* aRR);

// Plugin cleanup function
RR_PLUGIN_DECLSPEC bool             rrpCallConv    destroyPlugin(rrp::Plugin *plugin);

}

}    //Plugin Namespace




#endif
