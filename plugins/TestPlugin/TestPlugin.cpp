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
{

}

bool TestPlugin::Execute()
{
	cout<<"Executing plugin...\n";
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createRRPlugin()
{
    //allocate a new object and return it
	return new TestPlugin();
}

// Plugin cleanup function
bool __stdcall destroyRRPlugin(rr::Plugin *plugin)
{
	//we allocated in the factory with new, delete the passed object
    try
    {
    	delete plugin;
    	return true;
    }
    catch(...)
    {
    	//Bad stuff!
        clog<<"Failed deleting RoadRunner plugin..";
        return false;
    }
}


