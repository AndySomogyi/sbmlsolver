#pragma hdrstop
#include "TestPlugin.h"
#include "rrRoadRunner.h"
#pragma comment(lib, "roadrunner-static.lib")
//---------------------------------------------------------------------------

TestPlugin::TestPlugin(rr::RoadRunner* aRR)
:
rr::Plugin("TestPlugin", "No Category", aRR)
{
}

TestPlugin::~TestPlugin()
{

}

bool TestPlugin::Execute()
{
	cout<<"Executing plugin...\n";
	if(mRR)
    {
    	mRR->setTimeCourseSelectionList("S2, S1");
    }
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createRRPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new TestPlugin(aRR);
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


