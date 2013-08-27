#ifndef TestPluginH
#define TestPluginH
#include "rrCPPPlugin.h"
#include "rrCapability.h"
#include "rrParameter.h"
//---------------------------------------------------------------------------

namespace TestPlugin
{
using namespace rr;

class TestPlugin : public CPPPlugin
{
    private:
        Capability        mTestCapability;
        Parameter<int>    mTestParameter;

    public:
                        TestPlugin(rr::RoadRunner* aRR = NULL, int testParameter = 0);
                       ~TestPlugin();
        bool            execute(void* userData);

};

/////////////////////////////////////////////////////////////////////////
// Plugin factory functions. This is for plugins that create a class
//If we are to create a plugin not in C++, then another set of API functions need to be
//exposed..

extern "C"
{

PLUGIN_DECLSPEC const char*    rrCallConv    getImplementationLanguage();
rr::Plugin* PLUGIN_DECLSPEC    rrCallConv    createPlugin(rr::RoadRunner* aRR);

// Plugin cleanup function
PLUGIN_DECLSPEC bool           rrCallConv    destroyPlugin(rr::Plugin *plugin);

}

}    //Namespace




#endif
