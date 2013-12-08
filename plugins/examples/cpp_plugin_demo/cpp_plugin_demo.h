#ifndef cpp_plugin_demoH
#define cpp_plugin_demoH
#include "../source/rrCPPPlugin.h"
#include "../source/rrCapability.h"
#include "../source/rrParameter.h"
//---------------------------------------------------------------------------

namespace cpp_plugin_demo
{

class TestPlugin : public rrp::CPPPlugin
{
    private:
        rrp::Capability             mTestCapability;
        rrp::Parameter<int>         mTestParameter;

    public:
                                    TestPlugin(rr::RoadRunner* aRR = NULL, int testParameter = 0);
                                   ~TestPlugin();
        
        bool                        execute(void* userData, bool inThread = false);
};

extern "C"
{

RR_PLUGIN_DECLSPEC  const char*     plugins_cc    getImplementationLanguage();
RR_PLUGIN_DECLSPEC  TestPlugin*     plugins_cc    createPlugin(rr::RoadRunner* aRR);

// Plugin cleanup function
RR_PLUGIN_DECLSPEC bool             plugins_cc    destroyPlugin(rrp::Plugin *plugin);

}

}    //Plugin Namespace




#endif
