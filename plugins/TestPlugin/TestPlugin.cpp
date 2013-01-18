#pragma hdrstop
#include "TestPlugin.h"
#pragma comment(lib, "roadrunner.lib")
//---------------------------------------------------------------------------

TestPlugin::TestPlugin()
:
rr::Plugin("TestPlugin")
{
}

TestPlugin::~TestPlugin()
{}

bool TestPlugin::Execute()
{
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createRRPlugin()
{
    //allocate a new object and return it
	return new TestPlugin();
}

// Plugin cleanup function
bool __stdcall releaseRRPlugin(rr::Plugin *plugin)
{
	//we allocated in the factory with new, delete the passed object
    delete plugin;
    return true;
}


