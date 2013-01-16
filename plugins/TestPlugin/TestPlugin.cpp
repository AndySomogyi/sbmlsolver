//---------------------------------------------------------------------------
#pragma hdrstop

#include "TestPlugin.h"
//---------------------------------------------------------------------------

#pragma comment(lib, "roadrunner-static.lib")
TestPlugin::TestPlugin()
:
rr::Plugin("TestPlugin")
{
}

TestPlugin::~TestPlugin()
{}

bool TestPlugin::Execute()
{

}

// Plugin factory function
rr::Plugin* Create_Plugin ()
{
    //allocate a new object and return it
	return new TestPlugin();
}

// Plugin cleanup function
bool Release_Plugin (rr::Plugin *plugin)
{
	//we allocated in the factory with new, delete the passed object
    delete plugin;
    return true;
}


