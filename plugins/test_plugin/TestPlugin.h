#ifndef TestPluginH
#define TestPluginH
#include "rrPlugin.h"
#include "rrCapability.h"
#include "rrParameter.h"
//---------------------------------------------------------------------------

namespace TestPlugin
{
using namespace rr;

class TestPlugin : public Plugin
{
	private:
        Capability		mTestCapability;
        Parameter<int>	mTestParameter;

    public:
    					TestPlugin(rr::RoadRunner* aRR = NULL, int testParameter = 0);
					   ~TestPlugin();
		bool			execute(void* userData);

};

/////////////////////////////////////////////////////////////////////////
// Plugin factory functions. This is for plugins that create a class
//If we are to create a plugin not in C++, then another set of API functions need to be
//exposed..

extern "C"
{

PLUGIN_DECLSPEC rr::Plugin* __stdcall	createPlugin(rr::RoadRunner* aRR);

// Plugin cleanup function
PLUGIN_DECLSPEC bool		__stdcall	destroyPlugin(rr::Plugin *plugin);

}

}

#endif
