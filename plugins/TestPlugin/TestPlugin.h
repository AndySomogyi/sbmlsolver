#ifndef TestPluginH
#define TestPluginH
#include "rrPlugin.h"
#include "Poco/SharedLibrary.h"
//---------------------------------------------------------------------------

class TestPlugin : public rr::Plugin
{
    public:
    					TestPlugin(rr::RoadRunner* aRR = NULL);
					   ~TestPlugin();
		bool			execute();
};

/////////////////////////////////////////////////////////////////////////
// Plugin "factory" functions. This is for plugins that create a class
//If we are to create a plugin not in C++, then another set of API functions need to be
//exposed..

extern "C"
{
#define EXP_FUNC __declspec(dllexport)
EXP_FUNC rr::Plugin* __stdcall	createRRPlugin(rr::RoadRunner* aRR);

// Plugin cleanup function
EXP_FUNC bool		__stdcall	destroyRRPlugin(rr::Plugin *plugin);

}

#endif
